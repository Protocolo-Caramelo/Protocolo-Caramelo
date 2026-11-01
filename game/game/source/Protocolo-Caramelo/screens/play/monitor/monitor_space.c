#include"monitor_space.h"

#include<math.h>

static int ready;
static int origin_x, origin_y;
static double scale_x, scale_y;

void MonitorSpace_Update(const SDL_Rect *tex_dest, double sx, double sy)
{
    origin_x = tex_dest->x;
    origin_y = tex_dest->y;
    scale_x = sx;
    scale_y = sy;
    ready = (sx > 0 && sy > 0);
}

int MonitorSpace_IsReady(void)
{
    return ready;
}

// Units -> screen pixels
int MonitorSpace_ToScreenX(int ux)
{
    return origin_x + (int)floor(ux * scale_x + 0.5);
}

int MonitorSpace_ToScreenY(int uy)
{
    return origin_y + (int)floor(uy * scale_y + 0.5);
}

int MonitorSpace_ToScreenLenX(int u)
{
    return (int)floor(u * scale_x + 0.5);
}

int MonitorSpace_ToScreenLenY(int u)
{
    return (int)floor(u * scale_y + 0.5);
}

SDL_Rect MonitorSpace_RectToScreen(const SDL_Rect *u)
{
    // Convert the corners (not the width/height) so that adjacent rects never leave gaps
    SDL_Rect px;
    px.x = MonitorSpace_ToScreenX(u->x);
    px.y = MonitorSpace_ToScreenY(u->y);
    px.w = MonitorSpace_ToScreenX(u->x + u->w) - px.x;
    px.h = MonitorSpace_ToScreenY(u->y + u->h) - px.y;
    return px;
}

// Screen pixels -> units
int MonitorSpace_FromScreenX(int px)
{
    return (int)floor((px - origin_x) / scale_x + 0.5);
}

int MonitorSpace_FromScreenY(int py)
{
    return (int)floor((py - origin_y) / scale_y + 0.5);
}

double MonitorSpace_TextScale(void)
{
    return scale_y;
}

// Desktop area
SDL_Rect MonitorSpace_Desktop(void)
{
    return (SDL_Rect){
        MONITOR_AREA_X, MONITOR_AREA_Y,
        MONITOR_AREA_W, MONITOR_AREA_H
    };
}

SDL_Rect MonitorSpace_DesktopOnScreen(void)
{
    SDL_Rect d = MonitorSpace_Desktop();
    return MonitorSpace_RectToScreen(&d);
}
