#include"window_input.h"
#include"click_counter.h"
#include"monitor_space.h"
#include"monitor_cursor.h"

// Windows are remembered by app id, not by pointer: a closed window just stops being found
static int drag_app = MONITOR_APP_NONE;         // window being moved/resized

static int pressed_app = MONITOR_APP_NONE;      // window whose title button is being held
static DesktopWindowPart pressed_part;

static int body_press_app = MONITOR_APP_NONE;   // window whose content got the press

static ClickCounter title_clicks;

//
// Cursor
//
static MonitorCursor cursorForEdges(int edges)
{
    int horizontal = edges & (DWIN_EDGE_LEFT | DWIN_EDGE_RIGHT);
    int vertical   = edges & (DWIN_EDGE_TOP | DWIN_EDGE_BOTTOM);
    int same_diagonal;

    if(horizontal && vertical){
        // Top-left and bottom-right corners share a diagonal; so do the other two
        same_diagonal = ((edges & DWIN_EDGE_LEFT) != 0) == ((edges & DWIN_EDGE_TOP) != 0);
        return same_diagonal? MONITOR_CURSOR_RESIZE_NWSE: MONITOR_CURSOR_RESIZE_NESW;
    }
    return horizontal? MONITOR_CURSOR_RESIZE_H: MONITOR_CURSOR_RESIZE_V;
}

static void updateHoverCursor(const DesktopWindow *win, const DesktopWindowHit *hit)
{
    if(win && hit->part == DWIN_PART_RESIZE){
        MonitorCursor_Set(cursorForEdges(hit->edges));
    }
    else {
        MonitorCursor_Set(MONITOR_CURSOR_ARROW);
    }
}

//
// Forwarding to the apps inside the windows
//
// Which windows care about this event: motion goes to all (so they can drop their
// hover), a press only to the window under it, a release to the window that got the press.
static int appWantsEvent(const SDL_Event *e, const DesktopWindow *win, int pointer_inside)
{
    switch(e->type){
        case SDL_MOUSEMOTION:
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            return pointer_inside;
        case SDL_MOUSEBUTTONUP:
            return (int)win->app == body_press_app;
        default:
            return 0;
    }
}

static void notifyApps(const SDL_Event *e, const DesktopWindow *top, const DesktopWindowHit *hit)
{
    DesktopWindow *snapshot[MONITOR_APP_COUNT];
    DesktopWindow *win;
    SDL_Rect body_units, body_px;
    int count, i, pointer_inside;

    // A handler may close windows (the task manager does), so work on a copy
    count = WindowStack_Copy(snapshot);
    for(i = 0; i < count; ++i){
        win = snapshot[i];
        if(!WindowStack_Contains(win) || win->minimized || !win->def->handle_event){
            continue;
        }

        pointer_inside = (win == top && hit->part == DWIN_PART_BODY);
        if(!appWantsEvent(e, win, pointer_inside)){
            continue;
        }
        body_units = DesktopWindow_BodyRect(win);
        body_px = MonitorSpace_RectToScreen(&body_units);
        win->def->handle_event(e, &body_px, pointer_inside);
    }
}

//
// Press
//
static void startTitlePress(DesktopWindow *win, int ux, int uy)
{
    if(ClickCounter_Register(&title_clicks, win) >= 2){
        ClickCounter_Reset(&title_clicks);
        WindowManager_ToggleMaximize(win);
        return;
    }
    if(DesktopWindow_BeginMove(win, ux, uy)){
        drag_app = win->app;
        MonitorCursor_Set(MONITOR_CURSOR_MOVE);
    }
}

static void startResize(DesktopWindow *win, const DesktopWindowHit *hit, int ux, int uy)
{
    if(DesktopWindow_BeginResize(win, hit->edges, ux, uy)){
        drag_app = win->app;
        MonitorCursor_Set(cursorForEdges(hit->edges));
    }
}

static int onMouseDown(const SDL_Event *e)
{
    DesktopWindowHit hit;
    DesktopWindow *win;
    int ux, uy;

    ux = MonitorSpace_FromScreenX(e->button.x);
    uy = MonitorSpace_FromScreenY(e->button.y);

    win = WindowStack_TopmostAt(ux, uy, &hit);
    if(!win){
        return 0;
    }

    // Any button focuses the window, but only the left one acts on it
    WindowManager_Focus(win);
    if(e->button.button == SDL_BUTTON_LEFT){
        switch(hit.part){
            case DWIN_PART_MINIMIZE:
            case DWIN_PART_MAXIMIZE:
            case DWIN_PART_CLOSE:
                pressed_app = win->app;
                pressed_part = hit.part;
                break;

            case DWIN_PART_TITLE:
                startTitlePress(win, ux, uy);
                break;

            case DWIN_PART_RESIZE:
                startResize(win, &hit, ux, uy);
                break;

            case DWIN_PART_BODY:
                body_press_app = win->app;
                break;

            default:
                break;
        }
    }

    notifyApps(e, win, &hit);
    return 1;
}

//
// Motion
//
static int onMouseMotion(const SDL_Event *e)
{
    SDL_Rect desktop;
    DesktopWindowHit hit;
    DesktopWindow *win;
    int ux, uy;

    ux = MonitorSpace_FromScreenX(e->motion.x);
    uy = MonitorSpace_FromScreenY(e->motion.y);

    win = WindowStack_Find(drag_app);
    if(win){
        desktop = MonitorSpace_Desktop();
        DesktopWindow_DragTo(win, ux, uy, &desktop);
        return 1;
    }
    drag_app = MONITOR_APP_NONE;

    win = WindowStack_TopmostAt(ux, uy, &hit);
    updateHoverCursor(win, &hit);
    notifyApps(e, win, &hit);
    return win != NULL;
}

//
// Release
//
static int finishDrag(void)
{
    DesktopWindow *win;

    win = WindowStack_Find(drag_app);
    drag_app = MONITOR_APP_NONE;
    if(!win){
        return 0;
    }

    // Moving the window is not part of a double click on its title bar
    if(win->drag == DWIN_DRAG_MOVE && DesktopWindow_DragMoved(win)){
        ClickCounter_Reset(&title_clicks);
    }
    DesktopWindow_EndDrag(win);
    return 1;
}

static void runButtonAction(DesktopWindow *win, DesktopWindowPart part)
{
    switch(part){
        case DWIN_PART_MINIMIZE:
            WindowManager_Minimize(win);
            break;
        case DWIN_PART_MAXIMIZE:
            WindowManager_ToggleMaximize(win);
            break;
        case DWIN_PART_CLOSE:
            WindowManager_Close(win);
            break;
        default:
            break;
    }
}

// The click only counts if the button is released over the button it was pressed on
static int finishButtonPress(int ux, int uy)
{
    DesktopWindow *win;
    DesktopWindowPart part;

    win = WindowStack_Find(pressed_app);
    part = pressed_part;
    pressed_app = MONITOR_APP_NONE;
    if(!win){
        return 0;
    }

    if(DesktopWindow_HitTest(win, ux, uy).part == part){
        runButtonAction(win, part);
    }
    return 1;
}

static int onMouseUp(const SDL_Event *e)
{
    DesktopWindowHit hit;
    DesktopWindow *top;
    int ux, uy, used;

    used = 0;
    ux = MonitorSpace_FromScreenX(e->button.x);
    uy = MonitorSpace_FromScreenY(e->button.y);

    // The app gets its release first; the actions below may close windows
    top = WindowStack_TopmostAt(ux, uy, &hit);
    notifyApps(e, top, &hit);
    body_press_app = MONITOR_APP_NONE;

    if(e->button.button == SDL_BUTTON_LEFT){
        used |= finishDrag();
        used |= finishButtonPress(ux, uy);
    }

    top = WindowStack_TopmostAt(ux, uy, &hit); // windows may have changed
    updateHoverCursor(top, &hit);
    return used || top != NULL;
}

//
// Public
//
int WindowInput_HandleEvent(const SDL_Event *e)
{
    switch(e->type){
        case SDL_MOUSEBUTTONDOWN:
            return onMouseDown(e);
        case SDL_MOUSEMOTION:
            return onMouseMotion(e);
        case SDL_MOUSEBUTTONUP:
            return onMouseUp(e);
        default:
            return 0;
    }
}

int WindowInput_IsBusy(void)
{
    return drag_app != MONITOR_APP_NONE;
}

void WindowInput_CancelInteraction(void)
{
    DesktopWindow *win;

    win = WindowStack_Find(drag_app);
    if(win){
        DesktopWindow_EndDrag(win);
    }
    drag_app = MONITOR_APP_NONE;
    pressed_app = MONITOR_APP_NONE;
    body_press_app = MONITOR_APP_NONE;
    ClickCounter_Reset(&title_clicks);
}
