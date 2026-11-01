#ifndef MONITOR_SPACE_H
#define MONITOR_SPACE_H

#include<graphical.h>

/*
 * Monitor space
 * -------------
 * Everything inside the monitor (windows, icons, sizes, margins) is described in
 * "monitor units": pixels of the Monitor_view_*.png texture. This keeps the layout
 * independent from the real window size. This module converts between units and
 * screen pixels, and must be updated once per frame with the placement of the
 * monitor texture (see MonitorSpace_Update).
 */

// Size of Monitor_view_*.png, in units
#define MONITOR_TEX_W 2283
#define MONITOR_TEX_H 1568

// Desktop area: where windows and icons live (inside the bezel, above the taskbar)
#define MONITOR_AREA_X 285
#define MONITOR_AREA_Y 118
#define MONITOR_AREA_W 1675
#define MONITOR_AREA_H 1180

// Called every frame, right after drawing the monitor background
void MonitorSpace_Update(const SDL_Rect *tex_dest, double scale_x, double scale_y);
// 0 until the first MonitorSpace_Update
int MonitorSpace_IsReady(void);

// Units -> screen pixels
int MonitorSpace_ToScreenX(int ux);
int MonitorSpace_ToScreenY(int uy);
int MonitorSpace_ToScreenLenX(int u);
int MonitorSpace_ToScreenLenY(int u);
SDL_Rect MonitorSpace_RectToScreen(const SDL_Rect *units);

// Screen pixels -> units
int MonitorSpace_FromScreenX(int px);
int MonitorSpace_FromScreenY(int py);

// Scale to apply to Text configs (the game scales text by the vertical factor)
double MonitorSpace_TextScale(void);

// Desktop area, in units and in screen pixels
SDL_Rect MonitorSpace_Desktop(void);
SDL_Rect MonitorSpace_DesktopOnScreen(void);

#endif
