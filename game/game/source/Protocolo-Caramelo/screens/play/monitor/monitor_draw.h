#ifndef MONITOR_DRAW_H
#define MONITOR_DRAW_H

#include<graphical.h>

/*
 * Small drawing helpers used by the monitor UI. All of them work in screen pixels
 * and leave the renderer's draw color and blend mode as they found them.
 */
void MonitorDraw_Fill(const SDL_Rect *rect, SDL_Color color);
void MonitorDraw_Outline(const SDL_Rect *rect, int thickness, SDL_Color color);
void MonitorDraw_Line(int x1, int y1, int x2, int y2, int thickness, SDL_Color color);

#endif
