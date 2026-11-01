#include"icon_layer.h"
#include"click_counter.h"
#include"monitor_space.h"
#include"monitor_cursor.h"
#include"monitor_draw.h"

static DesktopIcon *icons[MONITOR_APP_COUNT];
// Draw order, bottom to top: the last one is drawn over the others
static DesktopIcon *order[MONITOR_APP_COUNT];

static IconLayerOpenFn open_callback;
static ClickCounter clicks;

// Press/drag in progress
static struct {
    DesktopIcon *icon;
    int grab_dx, grab_dy;       // cursor offset inside the icon cell, in units
    int press_x, press_y;       // where the press happened, in screen pixels
    int moved;                  // already turned into a real drag
    SDL_Point target;           // grid cell the icon would snap to right now
} drag;

static const SDL_Color COLOR_GHOST_FILL = {255, 180, 27, 35};
static const SDL_Color COLOR_GHOST_LINE = {255, 180, 27, 170};

//
// Icon lookup and ordering
//
static DesktopIcon* iconAt(int ux, int uy)
{
    int i;

    // Topmost first
    for(i = MONITOR_APP_COUNT - 1; i >= 0; --i){
        if(order[i] && DesktopIcon_Contains(order[i], ux, uy)){
            return order[i];
        }
    }
    return NULL;
}

static void bringToFront(DesktopIcon *icon)
{
    int i, pos;

    for(pos = 0; pos < MONITOR_APP_COUNT && order[pos] != icon; ++pos);
    if(pos == MONITOR_APP_COUNT){
        return;
    }
    for(i = pos; i < MONITOR_APP_COUNT - 1; ++i){
        order[i] = order[i + 1];
    }
    order[MONITOR_APP_COUNT - 1] = icon;
}

static void selectOnly(DesktopIcon *icon)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(icons[i]){
            icons[i]->selected = (icons[i] == icon);
        }
    }
}

static void updateHover(int ux, int uy)
{
    DesktopIcon *under;
    size_t i;

    under = iconAt(ux, uy);
    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(icons[i]){
            icons[i]->hovered = (icons[i] == under);
        }
    }
}

//
// Grid snapping
//
// Icons rest exactly on grid cells, so comparing positions tells if a cell is taken
static int cellTakenByOther(int col, int row, const DesktopIcon *ignored)
{
    SDL_Rect cell;
    size_t i;

    cell = IconGrid_CellRect(col, row);
    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(icons[i] && icons[i] != ignored
           && icons[i]->cell.x == cell.x && icons[i]->cell.y == cell.y){
            return 1;
        }
    }
    return 0;
}

// Free cell closest to (ux, uy), ignoring the cell the dragged icon came from
static SDL_Point findDropCell(int ux, int uy, const DesktopIcon *dragged)
{
    SDL_Point best;
    SDL_Rect cell;
    long best_distance, dx, dy, distance;
    int col, row;

    best = IconGrid_Nearest(ux, uy);
    best_distance = -1;

    for(row = 0; row < ICON_GRID_ROWS; ++row){
        for(col = 0; col < ICON_GRID_COLS; ++col){
            if(cellTakenByOther(col, row, dragged)){
                continue;
            }
            cell = IconGrid_CellRect(col, row);
            dx = cell.x - ux;
            dy = cell.y - uy;
            distance = dx * dx + dy * dy;
            if(best_distance < 0 || distance < best_distance){
                best_distance = distance;
                best = (SDL_Point){col, row};
            }
        }
    }
    return best;
}

static void snapToTarget(DesktopIcon *icon)
{
    SDL_Point cell;

    cell = findDropCell(icon->cell.x, icon->cell.y, icon);
    icon->cell = IconGrid_CellRect(cell.x, cell.y);
}

//
// Events
//
static int onMouseDown(const SDL_Event *e)
{
    DesktopIcon *icon;
    int ux, uy;

    if(e->button.button != SDL_BUTTON_LEFT){
        return 0;
    }

    ux = MonitorSpace_FromScreenX(e->button.x);
    uy = MonitorSpace_FromScreenY(e->button.y);
    icon = iconAt(ux, uy);

    selectOnly(icon); // NULL (empty desktop) clears the selection
    if(!icon){
        return 0;
    }

    bringToFront(icon);
    drag.icon = icon;
    drag.grab_dx = ux - icon->cell.x;
    drag.grab_dy = uy - icon->cell.y;
    drag.press_x = e->button.x;
    drag.press_y = e->button.y;
    drag.moved = 0;
    return 1;
}

static int dragStarted(const SDL_Event *e)
{
    int dx, dy;

    dx = e->motion.x - drag.press_x;
    dy = e->motion.y - drag.press_y;
    return dx * dx + dy * dy >= ICON_DRAG_THRESHOLD_PX * ICON_DRAG_THRESHOLD_PX;
}

static int onMouseMotion(const SDL_Event *e)
{
    SDL_Rect desktop;
    int ux, uy;

    ux = MonitorSpace_FromScreenX(e->motion.x);
    uy = MonitorSpace_FromScreenY(e->motion.y);

    if(!drag.icon){
        updateHover(ux, uy);
        return 0;
    }

    if(!drag.moved){
        if(!dragStarted(e)){
            return 1; // still just a press
        }
        drag.moved = 1;
        MonitorCursor_Set(MONITOR_CURSOR_MOVE);
    }

    desktop = MonitorSpace_Desktop();
    DesktopIcon_MoveTo(drag.icon, ux - drag.grab_dx, uy - drag.grab_dy, &desktop);
    drag.target = findDropCell(drag.icon->cell.x, drag.icon->cell.y, drag.icon);
    return 1;
}

static void registerClick(DesktopIcon *icon)
{
    if(ClickCounter_Register(&clicks, icon) >= ICON_OPEN_CLICKS){
        ClickCounter_Reset(&clicks);
        if(open_callback){
            open_callback(icon->app);
        }
    }
}

static int onMouseUp(const SDL_Event *e)
{
    DesktopIcon *icon;
    int was_moved;

    if(e->button.button != SDL_BUTTON_LEFT || !drag.icon){
        return 0;
    }

    icon = drag.icon;
    was_moved = drag.moved;
    drag.icon = NULL;
    drag.moved = 0;
    MonitorCursor_Set(MONITOR_CURSOR_ARROW);

    if(was_moved){
        snapToTarget(icon);
        ClickCounter_Reset(&clicks); // a drag never counts as part of a double click
    }
    else {
        registerClick(icon);
    }
    return 1;
}

int IconLayer_HandleEvent(const SDL_Event *e)
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

int IconLayer_IsDragging(void)
{
    return drag.icon != NULL;
}

void IconLayer_ClearHover(void)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(icons[i]){
            icons[i]->hovered = 0;
        }
    }
}

void IconLayer_CancelInteraction(void)
{
    // An icon released mid-drag still has to land on a cell
    if(drag.icon && drag.moved){
        snapToTarget(drag.icon);
    }
    drag.icon = NULL;
    drag.moved = 0;
    ClickCounter_Reset(&clicks);
    IconLayer_ClearHover();
}

//
// Life cycle / render
//
void IconLayer_Init(IconLayerOpenFn on_open)
{
    size_t i;

    open_callback = on_open;
    memset(&drag, 0, sizeof(drag));
    ClickCounter_Reset(&clicks);

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        icons[i] = DesktopIcon_Create((MonitorAppId)i);
        order[i] = icons[i];
    }
}

void IconLayer_Quit(void)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        DesktopIcon_Destroy(&icons[i]);
        order[i] = NULL;
    }
    open_callback = NULL;
}

// Outline of the cell the dragged icon will snap to
static void renderDropTarget(void)
{
    SDL_Rect cell, cell_px;

    if(!drag.icon || !drag.moved){
        return;
    }
    cell = IconGrid_CellRect(drag.target.x, drag.target.y);
    cell_px = MonitorSpace_RectToScreen(&cell);
    MonitorDraw_Fill(&cell_px, COLOR_GHOST_FILL);
    MonitorDraw_Outline(&cell_px, 2, COLOR_GHOST_LINE);
}

void IconLayer_Render(void)
{
    size_t i;

    renderDropTarget();
    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(order[i]){
            DesktopIcon_Draw(order[i]);
        }
    }
}
