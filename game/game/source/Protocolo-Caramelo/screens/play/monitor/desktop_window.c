#include"desktop_window.h"
#include"monitor_space.h"
#include"monitor_draw.h"
#include"game/game_globals.h"

static const SDL_Color COLOR_LINE           = {  0,   0,   0, 255};
static const SDL_Color COLOR_BODY           = {255, 255, 255, 255};
static const SDL_Color COLOR_TITLE_ACTIVE   = {255, 180,  27, 255};
static const SDL_Color COLOR_TITLE_INACTIVE = {210, 210, 210, 255};

#define TITLE_TEXT_PADDING  16      // units, between the window edge and the title
#define GRIP_STEP           10      // units between the resize grip's diagonal lines

// Title bar buttons, right to left; also the order they are tested in
static const DesktopWindowPart title_buttons[] = {
    DWIN_PART_CLOSE, DWIN_PART_MAXIMIZE, DWIN_PART_MINIMIZE
};
#define TITLE_BUTTON_COUNT ((int)(sizeof(title_buttons) / sizeof(title_buttons[0])))

//
// Create / destroy
//
DesktopWindow* DesktopWindow_Create(MonitorAppId app, const SDL_Rect *bounds, int cascade)
{
    const MonitorApp *def;
    DesktopWindow *win;

    def = MonitorApps_Get(app);
    if(!def){
        return NULL;
    }

    win = calloc(1, sizeof(DesktopWindow));
    if(!win){
        return NULL;
    }
    win->app = app;
    win->def = def;
    win->min_w = def->window_min_w;
    win->min_h = def->window_min_h;
    win->rect = (SDL_Rect){
        bounds->x + DWIN_SPAWN_MARGIN + cascade,
        bounds->y + DWIN_SPAWN_MARGIN + cascade,
        def->window_w,
        def->window_h
    };
    DesktopWindow_Clamp(win, bounds);

    win->title = MonitorText_Create(
        cache_fRegular,
        TextConfig_Create(
            .color        = (SDL_Color){0, 0, 0, 255},
            .scale_width  = 1 / 3.5,
            .scale_height = 1 / 3.5,
        ),
        def->window_title
    );
    win->placeholder = MonitorText_Create(
        cache_fRegular,
        TextConfig_Create(
            .color        = (SDL_Color){0, 0, 0, 255},
            .scale_width  = 1 / 4.0,
            .scale_height = 1 / 4.0,
        ),
        "Em construção..."
    );
    return win;
}

void DesktopWindow_Destroy(DesktopWindow **win)
{
    if(!win || !*win){
        return;
    }
    MonitorText_Destroy(&(*win)->title);
    MonitorText_Destroy(&(*win)->placeholder);
    free(*win);
    *win = NULL;
}

//
// Areas
//
SDL_Rect DesktopWindow_TitleRect(const DesktopWindow *win)
{
    return (SDL_Rect){win->rect.x, win->rect.y, win->rect.w, DWIN_TITLE_H};
}

SDL_Rect DesktopWindow_BodyRect(const DesktopWindow *win)
{
    return (SDL_Rect){
        win->rect.x + DWIN_BORDER,
        win->rect.y + DWIN_TITLE_H,
        win->rect.w - 2 * DWIN_BORDER,
        win->rect.h - DWIN_TITLE_H - DWIN_BORDER
    };
}

SDL_Rect DesktopWindow_ButtonRect(const DesktopWindow *win, DesktopWindowPart part)
{
    int slot;

    for(slot = 0; slot < TITLE_BUTTON_COUNT && title_buttons[slot] != part; ++slot);
    if(slot == TITLE_BUTTON_COUNT){
        return (SDL_Rect){0, 0, 0, 0};
    }

    return (SDL_Rect){
        win->rect.x + win->rect.w - DWIN_BUTTON_MARGIN - DWIN_BUTTON_SIZE
            - slot * (DWIN_BUTTON_SIZE + DWIN_BUTTON_GAP),
        win->rect.y + (DWIN_TITLE_H - DWIN_BUTTON_SIZE) / 2,
        DWIN_BUTTON_SIZE,
        DWIN_BUTTON_SIZE
    };
}

//
// Hit test
//
static DesktopWindowPart buttonAt(const DesktopWindow *win, int ux, int uy)
{
    SDL_Point p = {ux, uy};
    SDL_Rect button;
    int i;

    for(i = 0; i < TITLE_BUTTON_COUNT; ++i){
        button = DesktopWindow_ButtonRect(win, title_buttons[i]);
        if(SDL_PointInRect(&p, &button)){
            return title_buttons[i];
        }
    }
    return DWIN_PART_NONE;
}

// Edges (DWIN_EDGE_*) of the resize strip under the point, 0 if none.
// The strip extends slightly outside the window to make grabbing easier.
static int resizeEdgesAt(const DesktopWindow *win, int ux, int uy)
{
    const SDL_Rect *rc = &win->rect;
    int edges = 0;

    if(ux < rc->x - DWIN_RESIZE_OUTSIDE || ux >= rc->x + rc->w + DWIN_RESIZE_OUTSIDE
       || uy < rc->y - DWIN_RESIZE_OUTSIDE || uy >= rc->y + rc->h + DWIN_RESIZE_OUTSIDE){
        return 0;
    }
    if(ux < rc->x + DWIN_RESIZE_GRAB)               edges |= DWIN_EDGE_LEFT;
    if(ux >= rc->x + rc->w - DWIN_RESIZE_GRAB)      edges |= DWIN_EDGE_RIGHT;
    if(uy < rc->y + DWIN_RESIZE_GRAB)               edges |= DWIN_EDGE_TOP;
    if(uy >= rc->y + rc->h - DWIN_RESIZE_GRAB)      edges |= DWIN_EDGE_BOTTOM;
    return edges;
}

DesktopWindowHit DesktopWindow_HitTest(const DesktopWindow *win, int ux, int uy)
{
    DesktopWindowHit hit = {DWIN_PART_NONE, 0};
    SDL_Point p = {ux, uy};
    SDL_Rect title;

    if(win->minimized){
        return hit;
    }

    // Buttons win over the resize strip they overlap
    hit.part = buttonAt(win, ux, uy);
    if(hit.part != DWIN_PART_NONE){
        return hit;
    }

    // A maximized window fills the desktop: nothing to resize
    if(!win->maximized){
        hit.edges = resizeEdgesAt(win, ux, uy);
        if(hit.edges){
            hit.part = DWIN_PART_RESIZE;
            return hit;
        }
    }

    if(!SDL_PointInRect(&p, &win->rect)){
        return hit;
    }
    title = DesktopWindow_TitleRect(win);
    hit.part = SDL_PointInRect(&p, &title)? DWIN_PART_TITLE: DWIN_PART_BODY;
    return hit;
}

//
// Geometry and state
//
void DesktopWindow_Clamp(DesktopWindow *win, const SDL_Rect *bounds)
{
    SDL_Rect *rc = &win->rect;

    if(rc->w > bounds->w) rc->w = bounds->w;
    if(rc->h > bounds->h) rc->h = bounds->h;

    if(rc->x < bounds->x) rc->x = bounds->x;
    if(rc->y < bounds->y) rc->y = bounds->y;
    if(rc->x + rc->w > bounds->x + bounds->w) rc->x = bounds->x + bounds->w - rc->w;
    if(rc->y + rc->h > bounds->y + bounds->h) rc->y = bounds->y + bounds->h - rc->h;
}

void DesktopWindow_Maximize(DesktopWindow *win, const SDL_Rect *bounds)
{
    if(win->maximized){
        return;
    }
    DesktopWindow_EndDrag(win);
    win->restore_rect = win->rect;
    win->rect = *bounds;
    win->maximized = 1;
}

void DesktopWindow_Restore(DesktopWindow *win)
{
    if(!win->maximized){
        return;
    }
    DesktopWindow_EndDrag(win);
    win->rect = win->restore_rect;
    win->maximized = 0;
}

void DesktopWindow_SetMinimized(DesktopWindow *win, int minimized)
{
    win->minimized = minimized;
    if(minimized){
        DesktopWindow_EndDrag(win);
    }
}

//
// Dragging
//
static int clampInt(int v, int lo, int hi)
{
    return (v < lo)? lo: (v > hi)? hi: v;
}

int DesktopWindow_BeginMove(DesktopWindow *win, int ux, int uy)
{
    if(win->maximized){
        return 0;
    }
    win->drag = DWIN_DRAG_MOVE;
    win->start_rect = win->rect;
    win->grab_dx = ux - win->rect.x;
    win->grab_dy = uy - win->rect.y;
    return 1;
}

int DesktopWindow_BeginResize(DesktopWindow *win, int edges, int ux, int uy)
{
    if(win->maximized){
        return 0;
    }
    win->drag = DWIN_DRAG_RESIZE;
    win->resize_edges = edges;
    win->start_rect = win->rect;
    win->start_x = ux;
    win->start_y = uy;
    return 1;
}

static void resizeTo(DesktopWindow *win, int ux, int uy, const SDL_Rect *bounds)
{
    const SDL_Rect *s = &win->start_rect;
    int dx, dy;
    int left, right, top, bottom;

    dx = ux - win->start_x;
    dy = uy - win->start_y;

    left   = s->x;
    right  = s->x + s->w;
    top    = s->y;
    bottom = s->y + s->h;

    // Each held edge follows the cursor; the opposite edge stays where it was
    if(win->resize_edges & DWIN_EDGE_LEFT){
        left = clampInt(s->x + dx, bounds->x, right - win->min_w);
    }
    if(win->resize_edges & DWIN_EDGE_RIGHT){
        right = clampInt(s->x + s->w + dx, left + win->min_w, bounds->x + bounds->w);
    }
    if(win->resize_edges & DWIN_EDGE_TOP){
        top = clampInt(s->y + dy, bounds->y, bottom - win->min_h);
    }
    if(win->resize_edges & DWIN_EDGE_BOTTOM){
        bottom = clampInt(s->y + s->h + dy, top + win->min_h, bounds->y + bounds->h);
    }

    win->rect = (SDL_Rect){left, top, right - left, bottom - top};
}

void DesktopWindow_DragTo(DesktopWindow *win, int ux, int uy, const SDL_Rect *bounds)
{
    switch(win->drag){
        case DWIN_DRAG_MOVE:
            win->rect.x = ux - win->grab_dx;
            win->rect.y = uy - win->grab_dy;
            DesktopWindow_Clamp(win, bounds);
            break;

        case DWIN_DRAG_RESIZE:
            resizeTo(win, ux, uy, bounds);
            break;

        default:
            break;
    }
}

void DesktopWindow_EndDrag(DesktopWindow *win)
{
    win->drag = DWIN_DRAG_NONE;
    win->resize_edges = 0;
}

int DesktopWindow_DragMoved(const DesktopWindow *win)
{
    return win->rect.x != win->start_rect.x || win->rect.y != win->start_rect.y;
}

//
// Drawing: title bar buttons
//
static void drawMinimizeGlyph(const SDL_Rect *px, int line)
{
    int pad = px->w / 4;
    int y = px->y + px->h - pad;

    MonitorDraw_Line(px->x + pad, y, px->x + px->w - pad, y, line, COLOR_LINE);
}

static void drawMaximizeGlyph(const SDL_Rect *px, int line)
{
    int pad = px->w / 4;
    SDL_Rect square = {px->x + pad, px->y + pad, px->w - 2 * pad, px->h - 2 * pad};

    MonitorDraw_Outline(&square, line, COLOR_LINE);
}

// Two overlapping squares: shown while the window is maximized
static void drawRestoreGlyph(const SDL_Rect *px, int line)
{
    int size = px->w * 42 / 100;
    int shift = px->w * 14 / 100;
    int left = px->x + (px->w - size - shift) / 2;
    int top = px->y + (px->h - size - shift) / 2;
    SDL_Rect back = {left + shift, top, size, size};
    SDL_Rect front = {left, top + shift, size, size};

    MonitorDraw_Outline(&back, line, COLOR_LINE);
    MonitorDraw_Fill(&front, COLOR_BODY);
    MonitorDraw_Outline(&front, line, COLOR_LINE);
}

static void drawCloseGlyph(const SDL_Rect *px, int line)
{
    int pad = px->w / 4;

    MonitorDraw_Line(px->x + pad, px->y + pad, px->x + px->w - pad, px->y + px->h - pad, line, COLOR_LINE);
    MonitorDraw_Line(px->x + px->w - pad, px->y + pad, px->x + pad, px->y + px->h - pad, line, COLOR_LINE);
}

static void drawTitleButton(const DesktopWindow *win, DesktopWindowPart part, int line)
{
    SDL_Rect units, px;

    units = DesktopWindow_ButtonRect(win, part);
    px = MonitorSpace_RectToScreen(&units);
    MonitorDraw_Fill(&px, COLOR_BODY);
    MonitorDraw_Outline(&px, line, COLOR_LINE);

    switch(part){
        case DWIN_PART_MINIMIZE:
            drawMinimizeGlyph(&px, line);
            break;
        case DWIN_PART_MAXIMIZE:
            if(win->maximized){
                drawRestoreGlyph(&px, line);
            }
            else {
                drawMaximizeGlyph(&px, line);
            }
            break;
        case DWIN_PART_CLOSE:
            drawCloseGlyph(&px, line);
            break;
        default:
            break;
    }
}

//
// Drawing: window parts
//
static void drawResizeGrip(const DesktopWindow *win, int line)
{
    SDL_Rect rc = win->rect;
    int i, step;
    int cx, cy;

    // Three short diagonals in the bottom-right corner
    cx = MonitorSpace_ToScreenX(rc.x + rc.w - DWIN_BORDER - 4);
    cy = MonitorSpace_ToScreenY(rc.y + rc.h - DWIN_BORDER - 4);
    for(i = 1; i <= 3; ++i){
        step = MonitorSpace_ToScreenLenX(GRIP_STEP * i);
        MonitorDraw_Line(cx - step, cy, cx, cy - step, (line > 1)? line - 1: 1, COLOR_LINE);
    }
}

static void drawTitleText(DesktopWindow *win, const SDL_Rect *title_px)
{
    SDL_Rect leftmost_units, leftmost_px, clip, desktop;
    int text_h;
    double scale;

    // The text is clipped so it never runs under the buttons
    leftmost_units = DesktopWindow_ButtonRect(win, title_buttons[TITLE_BUTTON_COUNT - 1]);
    leftmost_px = MonitorSpace_RectToScreen(&leftmost_units);
    desktop = MonitorSpace_DesktopOnScreen();
    clip = (SDL_Rect){title_px->x, title_px->y, leftmost_px.x - title_px->x, title_px->h};
    SDL_IntersectRect(&clip, &desktop, &clip);
    SDL_RenderSetClipRect(r, &clip);

    scale = MonitorSpace_TextScale();
    MonitorText_Measure(win->title, scale, NULL, &text_h);
    MonitorText_Draw(
        win->title,
        title_px->x + MonitorSpace_ToScreenLenX(TITLE_TEXT_PADDING),
        title_px->y + (title_px->h - text_h) / 2,
        scale
    );

    SDL_RenderSetClipRect(r, &desktop);
}

static void drawTitleBar(DesktopWindow *win, int active, int line)
{
    SDL_Rect title_units, title_px;
    int i;

    title_units = DesktopWindow_TitleRect(win);
    title_px = MonitorSpace_RectToScreen(&title_units);
    MonitorDraw_Fill(&title_px, active? COLOR_TITLE_ACTIVE: COLOR_TITLE_INACTIVE);

    // Line between the title bar and the body
    MonitorDraw_Fill(&(SDL_Rect){title_px.x, title_px.y + title_px.h - line, title_px.w, line}, COLOR_LINE);

    for(i = 0; i < TITLE_BUTTON_COUNT; ++i){
        drawTitleButton(win, title_buttons[i], line);
    }
    drawTitleText(win, &title_px);
}

static void drawBody(DesktopWindow *win)
{
    SDL_Rect body_units, body_px, clip, desktop;
    int text_w, text_h;
    double scale;

    body_units = DesktopWindow_BodyRect(win);
    body_px = MonitorSpace_RectToScreen(&body_units);
    desktop = MonitorSpace_DesktopOnScreen();

    SDL_IntersectRect(&body_px, &desktop, &clip);
    SDL_RenderSetClipRect(r, &clip);

    if(win->def->draw_content){
        win->def->draw_content(&body_px);
    }
    else {
        scale = MonitorSpace_TextScale();
        MonitorText_Measure(win->placeholder, scale, &text_w, &text_h);
        MonitorText_Draw(
            win->placeholder,
            body_px.x + (body_px.w - text_w) / 2,
            body_px.y + (body_px.h - text_h) / 2,
            scale
        );
    }

    SDL_RenderSetClipRect(r, &desktop);
}

void DesktopWindow_Draw(DesktopWindow *win, int active)
{
    SDL_Rect px;
    int line;

    if(win->minimized){
        return;
    }

    px = MonitorSpace_RectToScreen(&win->rect);
    line = MonitorSpace_ToScreenLenY(DWIN_BORDER);
    if(line < 1){
        line = 1;
    }

    MonitorDraw_Fill(&px, COLOR_BODY);
    drawTitleBar(win, active, line);
    drawBody(win);
    if(!win->maximized){
        drawResizeGrip(win, line);
    }
    MonitorDraw_Outline(&px, line, COLOR_LINE);
}
