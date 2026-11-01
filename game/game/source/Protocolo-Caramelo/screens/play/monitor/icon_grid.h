#ifndef ICON_GRID_H
#define ICON_GRID_H

#include"monitor_space.h"

/*
 * IconGrid: the invisible grid desktop icons snap to. One icon per cell.
 * Cells are numbered (col, row) from the top-left; all values in monitor units.
 */
#define ICON_GRID_CELL_W    200
#define ICON_GRID_CELL_H    200
#define ICON_GRID_COLS      8
#define ICON_GRID_ROWS      5
// Top-left corner of the grid (the grid hugs the right side of the desktop)
#define ICON_GRID_ORIGIN_X  360
#define ICON_GRID_ORIGIN_Y  125

// Area occupied by a cell
SDL_Rect IconGrid_CellRect(int col, int row);
// Cell whose top-left corner is closest to (ux, uy); always a valid cell
SDL_Point IconGrid_Nearest(int ux, int uy);

#endif
