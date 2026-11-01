#include"taskbar.h"
#include"monitor_space.h"
#include"monitor_draw.h"
#include"game/game_globals.h"

static const SDL_Color COLOR_LINE      = {  0,   0,   0, 255};
static const SDL_Color COLOR_NORMAL    = {255, 255, 255, 255};
static const SDL_Color COLOR_MINIMIZED = {225, 225, 225, 255};
static const SDL_Color COLOR_ACTIVE    = {255, 180,  27, 255};
static const SDL_Color COLOR_HOVER     = {255, 180,  27,  70};

#define BUTTON_TEXT_PADDING 14  // units

static int hovered_app = MONITOR_APP_NONE;
static int pressed_app = MONITOR_APP_NONE;

//
// Layout
//
// Rect of the index-th button (in open order) when `count` windows are open, in units
static SDL_Rect buttonRect(int index, int count)
{
    int width;

    width = (TASKBAR_AREA_W - TASKBAR_BUTTON_GAP * (count - 1)) / count;
    if(width > TASKBAR_BUTTON_MAX_W){
        width = TASKBAR_BUTTON_MAX_W;
    }
    return (SDL_Rect){
        TASKBAR_AREA_X + index * (width + TASKBAR_BUTTON_GAP),
        TASKBAR_AREA_Y,
        width,
        TASKBAR_AREA_H
    };
}

// Window whose button is under (ux, uy), or NULL
static DesktopWindow* windowAt(int ux, int uy)
{
    SDL_Point p = {ux, uy};
    SDL_Rect rect;
    int i, count;

    count = WindowStack_Count();
    for(i = 0; i < count; ++i){
        rect = buttonRect(i, count);
        if(SDL_PointInRect(&p, &rect)){
            return WindowStack_AtOpenOrder(i);
        }
    }
    return NULL;
}

SDL_Rect Taskbar_OnScreen(void)
{
    SDL_Rect area = {TASKBAR_AREA_X, TASKBAR_AREA_Y, TASKBAR_AREA_W, TASKBAR_AREA_H};

    return MonitorSpace_RectToScreen(&area);
}

//
// Events
//
static void activate(DesktopWindow *win)
{
    if(!win->minimized && WindowStack_Active() == win){
        WindowManager_Minimize(win);
    }
    else {
        WindowManager_Focus(win); // restores minimized windows too
    }
}

static int onMouseDown(const SDL_Event *e)
{
    DesktopWindow *win;

    if(e->button.button != SDL_BUTTON_LEFT){
        return 0;
    }
    win = windowAt(MonitorSpace_FromScreenX(e->button.x), MonitorSpace_FromScreenY(e->button.y));
    if(!win){
        return 0;
    }
    pressed_app = win->app;
    return 1;
}

static int onMouseMotion(const SDL_Event *e)
{
    DesktopWindow *win;

    win = windowAt(MonitorSpace_FromScreenX(e->motion.x), MonitorSpace_FromScreenY(e->motion.y));
    hovered_app = win? (int)win->app: MONITOR_APP_NONE;
    return win != NULL;
}

// The click only counts if the button is released over the button it was pressed on
static int onMouseUp(const SDL_Event *e)
{
    DesktopWindow *pressed, *released;

    if(e->button.button != SDL_BUTTON_LEFT || pressed_app == MONITOR_APP_NONE){
        return 0;
    }

    pressed = WindowStack_Find(pressed_app);
    released = windowAt(MonitorSpace_FromScreenX(e->button.x), MonitorSpace_FromScreenY(e->button.y));
    pressed_app = MONITOR_APP_NONE;

    if(pressed && pressed == released){
        activate(pressed);
    }
    return 1;
}

int Taskbar_HandleEvent(const SDL_Event *e)
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

int Taskbar_IsPressed(void)
{
    return pressed_app != MONITOR_APP_NONE;
}

void Taskbar_ClearHover(void)
{
    hovered_app = MONITOR_APP_NONE;
}

void Taskbar_CancelInteraction(void)
{
    pressed_app = MONITOR_APP_NONE;
    hovered_app = MONITOR_APP_NONE;
}

//
// Drawing
//
static SDL_Color buttonColor(const DesktopWindow *win)
{
    if(win->minimized){
        return COLOR_MINIMIZED;
    }
    return (WindowStack_Active() == win)? COLOR_ACTIVE: COLOR_NORMAL;
}

static void drawButtonLabel(DesktopWindow *win, const SDL_Rect *button_px)
{
    SDL_Rect taskbar_px, clip;
    int text_h, padding;
    double scale;

    padding = MonitorSpace_ToScreenLenX(BUTTON_TEXT_PADDING);
    taskbar_px = Taskbar_OnScreen();

    // Long titles are cut at the button edge
    clip = (SDL_Rect){button_px->x + padding, button_px->y, button_px->w - 2 * padding, button_px->h};
    SDL_RenderSetClipRect(r, &clip);

    scale = MonitorSpace_TextScale();
    MonitorText_Measure(win->title, scale, NULL, &text_h);
    MonitorText_Draw(win->title, clip.x, button_px->y + (button_px->h - text_h) / 2, scale);

    SDL_RenderSetClipRect(r, &taskbar_px);
}

static void drawButton(DesktopWindow *win, int index, int count)
{
    SDL_Rect units, px;
    int line;

    units = buttonRect(index, count);
    px = MonitorSpace_RectToScreen(&units);
    line = MonitorSpace_ToScreenLenY(DWIN_BORDER);
    if(line < 1){
        line = 1;
    }

    MonitorDraw_Fill(&px, buttonColor(win));
    if((int)win->app == hovered_app){
        MonitorDraw_Fill(&px, COLOR_HOVER);
    }
    MonitorDraw_Outline(&px, line, COLOR_LINE);
    drawButtonLabel(win, &px);
}

void Taskbar_Render(void)
{
    int i, count;

    count = WindowStack_Count();
    for(i = 0; i < count; ++i){
        drawButton(WindowStack_AtOpenOrder(i), i, count);
    }
}
