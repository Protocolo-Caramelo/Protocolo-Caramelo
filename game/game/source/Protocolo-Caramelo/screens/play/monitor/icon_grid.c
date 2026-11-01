#include"icon_grid.h"

// The grid must fit inside the desktop area, or snapped icons would be clamped
_Static_assert(ICON_GRID_ORIGIN_X >= MONITOR_AREA_X, "icon grid starts left of the desktop");
_Static_assert(ICON_GRID_ORIGIN_Y >= MONITOR_AREA_Y, "icon grid starts above the desktop");
_Static_assert(ICON_GRID_ORIGIN_X + ICON_GRID_COLS * ICON_GRID_CELL_W <= MONITOR_AREA_X + MONITOR_AREA_W,
               "icon grid ends right of the desktop");
_Static_assert(ICON_GRID_ORIGIN_Y + ICON_GRID_ROWS * ICON_GRID_CELL_H <= MONITOR_AREA_Y + MONITOR_AREA_H,
               "icon grid ends below the desktop");

SDL_Rect IconGrid_CellRect(int col, int row)
{
    return (SDL_Rect){
        ICON_GRID_ORIGIN_X + col * ICON_GRID_CELL_W,
        ICON_GRID_ORIGIN_Y + row * ICON_GRID_CELL_H,
        ICON_GRID_CELL_W,
        ICON_GRID_CELL_H
    };
}

static int clampIndex(int index, int count)
{
    return (index < 0)? 0: (index >= count)? count - 1: index;
}

// Nearest integer to value / divisor, also for negative values
static int divideRounded(int value, int divisor)
{
    return (value >= 0)? (value + divisor / 2) / divisor: -((-value + divisor / 2) / divisor);
}

SDL_Point IconGrid_Nearest(int ux, int uy)
{
    return (SDL_Point){
        clampIndex(divideRounded(ux - ICON_GRID_ORIGIN_X, ICON_GRID_CELL_W), ICON_GRID_COLS),
        clampIndex(divideRounded(uy - ICON_GRID_ORIGIN_Y, ICON_GRID_CELL_H), ICON_GRID_ROWS)
    };
}
