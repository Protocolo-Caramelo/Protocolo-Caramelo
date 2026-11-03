#include"taskmgr.h"

// Layout of the window content, in monitor units
#define PADDING         24
#define HEADER_H        64
#define ROW_H           76
#define ROW_GAP         10
#define ROW_INSET       18
#define BUTTON_W        190
#define BUTTON_H        52
#define STATE_COLUMN    45      // % of the row width where the state column starts

static const SDL_Color COLOR_LINE  = {  0,   0,   0, 255};
static const SDL_Color COLOR_BODY  = {255, 255, 255, 255};
static const SDL_Color COLOR_HOVER = {255, 180,  27,  70};
static const SDL_Color COLOR_END   = {255, 150, 130, 255};

typedef enum RowState RowState;
enum RowState {
    ROW_STATE_OPEN,
    ROW_STATE_MINIMIZED,
    ROW_STATE_MAXIMIZED,
    ROW_STATE_COUNT
};
static const char *const state_names[ROW_STATE_COUNT] = {
    [ROW_STATE_OPEN]      = "Aberta",
    [ROW_STATE_MINIMIZED] = "Minimizada",
    [ROW_STATE_MAXIMIZED] = "Maximizada",
};

// Every text is constant, so they are created once
static MonitorText *app_names[MONITOR_APP_COUNT];
static MonitorText *state_texts[ROW_STATE_COUNT];
static MonitorText *header_app, *header_state, *end_label, *empty_label;

// What the mouse is over / holding: a row (identified by its app) and maybe its button
typedef struct RowHit RowHit;
struct RowHit {
    int app;
    int on_button;
};
static const RowHit NO_HIT = {MONITOR_APP_NONE, 0};
static RowHit hover = {MONITOR_APP_NONE, 0};
static RowHit pressed = {MONITOR_APP_NONE, 0};

//
// Texts
//
static MonitorText* createText(const char *str)
{
    return MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .color        = (SDL_Color){0, 0, 0, 255},
            .scale_width  = 1 / 4.0,
            .scale_height = 1 / 4.0,
        ),
        str
    );
}

void AppTaskManager_Init(void)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        app_names[i] = createText(MonitorApps_Get((MonitorAppId)i)->window_title);
    }
    for(i = 0; i < ROW_STATE_COUNT; ++i){
        state_texts[i] = createText(state_names[i]);
    }
    header_app   = createText("Aplicativo");
    header_state = createText("Estado");
    end_label    = createText("Encerrar");
    empty_label  = createText("Nenhum aplicativo em execução");
}

void AppTaskManager_Quit(void)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        MonitorText_Destroy(&app_names[i]);
    }
    for(i = 0; i < ROW_STATE_COUNT; ++i){
        MonitorText_Destroy(&state_texts[i]);
    }
    MonitorText_Destroy(&header_app);
    MonitorText_Destroy(&header_state);
    MonitorText_Destroy(&end_label);
    MonitorText_Destroy(&empty_label);
}

//
// Rows: the windows listed here, in open order, without the task manager itself
//
static int collectRows(DesktopWindow **rows)
{
    DesktopWindow *win;
    int i, n;

    n = 0;
    for(i = 0; i < WindowStack_Count(); ++i){
        win = WindowStack_AtOpenOrder(i);
        if(win->app != MONITOR_APP_TASKMGR){
            rows[n++] = win;
        }
    }
    return n;
}

static RowState stateOf(const DesktopWindow *win)
{
    if(win->minimized){
        return ROW_STATE_MINIMIZED;
    }
    return win->maximized? ROW_STATE_MAXIMIZED: ROW_STATE_OPEN;
}

//
// Layout, in screen pixels
//
static int lenX(int units)
{
    return MonitorSpace_ToScreenLenX(units);
}

static int lenY(int units)
{
    return MonitorSpace_ToScreenLenY(units);
}

static SDL_Rect rowRect(const SDL_Rect *body, int index)
{
    return (SDL_Rect){
        body->x + lenX(PADDING),
        body->y + lenY(PADDING + HEADER_H + index * (ROW_H + ROW_GAP)),
        body->w - 2 * lenX(PADDING),
        lenY(ROW_H)
    };
}

static SDL_Rect endButtonRect(const SDL_Rect *row)
{
    int width = lenX(BUTTON_W);
    int height = lenY(BUTTON_H);

    return (SDL_Rect){
        row->x + row->w - lenX(ROW_INSET) - width,
        row->y + (row->h - height) / 2,
        width,
        height
    };
}

static RowHit hitRow(const SDL_Rect *body, int x, int y)
{
    DesktopWindow *rows[MONITOR_APP_COUNT];
    SDL_Point p = {x, y};
    SDL_Rect row, button;
    int i, n;

    n = collectRows(rows);
    for(i = 0; i < n; ++i){
        row = rowRect(body, i);
        if(SDL_PointInRect(&p, &row)){
            button = endButtonRect(&row);
            return (RowHit){rows[i]->app, SDL_PointInRect(&p, &button)};
        }
    }
    return NO_HIT;
}

static int sameHit(RowHit a, RowHit b)
{
    return a.app == b.app && a.on_button == b.on_button;
}

//
// Events
//
static void perform(RowHit hit)
{
    DesktopWindow *win;

    win = WindowStack_Find(hit.app);
    if(!win){
        return;
    }
    if(hit.on_button){
        WindowManager_Close(win);
    }
    else {
        WindowManager_Focus(win);
    }
}

void AppTaskManager_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside)
{
    RowHit under;

    switch(e->type){
        case SDL_MOUSEMOTION:
            hover = pointer_inside? hitRow(body, e->motion.x, e->motion.y): NO_HIT;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(e->button.button == SDL_BUTTON_LEFT){
                pressed = hitRow(body, e->button.x, e->button.y);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(e->button.button != SDL_BUTTON_LEFT){
                break;
            }
            // Only counts if released over the same row/button that was pressed
            under = pointer_inside? hitRow(body, e->button.x, e->button.y): NO_HIT;
            if(pressed.app != MONITOR_APP_NONE && sameHit(pressed, under)){
                perform(pressed);
            }
            pressed = NO_HIT;
            break;

        default:
            break;
    }
}

//
// Drawing
//
static void drawTextVCentered(MonitorText *text, int x, const SDL_Rect *area)
{
    double scale = MonitorSpace_TextScale();
    int text_h;

    MonitorText_Measure(text, scale, NULL, &text_h);
    MonitorText_Draw(text, x, area->y + (area->h - text_h) / 2, scale);
}

static void drawTextCentered(MonitorText *text, const SDL_Rect *area)
{
    double scale = MonitorSpace_TextScale();
    int text_w, text_h;

    MonitorText_Measure(text, scale, &text_w, &text_h);
    MonitorText_Draw(text, area->x + (area->w - text_w) / 2, area->y + (area->h - text_h) / 2, scale);
}

static void drawHeader(const SDL_Rect *body)
{
    SDL_Rect row = rowRect(body, 0);
    SDL_Rect area = {row.x, body->y + lenY(PADDING), row.w, lenY(HEADER_H - 8)};
    int line = lenY(2) > 0? lenY(2): 1;

    drawTextVCentered(header_app, row.x + lenX(ROW_INSET), &area);
    drawTextVCentered(header_state, row.x + row.w * STATE_COLUMN / 100, &area);
    MonitorDraw_Fill(&(SDL_Rect){row.x, area.y + area.h, row.w, line}, COLOR_LINE);
}

static void drawRow(DesktopWindow *win, const SDL_Rect *row)
{
    SDL_Rect button = endButtonRect(row);
    int line = lenY(2) > 0? lenY(2): 1;

    MonitorDraw_Fill(row, COLOR_BODY);
    if(hover.app == (int)win->app){
        MonitorDraw_Fill(row, COLOR_HOVER);
    }
    MonitorDraw_Outline(row, line, COLOR_LINE);

    drawTextVCentered(app_names[win->app], row->x + lenX(ROW_INSET), row);
    drawTextVCentered(state_texts[stateOf(win)], row->x + row->w * STATE_COLUMN / 100, row);

    MonitorDraw_Fill(&button, COLOR_END);
    if(hover.app == (int)win->app && hover.on_button){
        MonitorDraw_Fill(&button, COLOR_HOVER);
    }
    MonitorDraw_Outline(&button, line, COLOR_LINE);
    drawTextCentered(end_label, &button);
}

void AppTaskManager_Draw(const SDL_Rect *body)
{
    DesktopWindow *rows[MONITOR_APP_COUNT];
    SDL_Rect row;
    int i, n;

    n = collectRows(rows);
    if(n == 0){
        drawTextCentered(empty_label, body);
        return;
    }

    drawHeader(body);
    for(i = 0; i < n; ++i){
        row = rowRect(body, i);
        drawRow(rows[i], &row);
    }
}
