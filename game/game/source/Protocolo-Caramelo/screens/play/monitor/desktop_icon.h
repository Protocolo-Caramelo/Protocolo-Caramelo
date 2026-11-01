#ifndef DESKTOP_ICON_H
#define DESKTOP_ICON_H

#include"monitor_apps.h"
#include"monitor_text.h"
#include"icon_grid.h"

/*
 * DesktopIcon: one app icon (sprite + label). Knows how to draw itself, test hits
 * and move inside given bounds. Mouse interaction lives in icon_layer.c.
 */

typedef struct DesktopIcon DesktopIcon;
struct DesktopIcon {
    MonitorAppId app;

    SDL_Texture *sprite;
    int sprite_w, sprite_h;
    MonitorText *label;

    SDL_Rect cell; // position and size, in monitor units (one icon grid cell)

    int hovered;
    int selected;
};

DesktopIcon* DesktopIcon_Create(MonitorAppId app);
void DesktopIcon_Destroy(DesktopIcon **icon);

int DesktopIcon_Contains(const DesktopIcon *icon, int ux, int uy);
// Moves the icon so that its cell stays inside bounds (monitor units)
void DesktopIcon_MoveTo(DesktopIcon *icon, int ux, int uy, const SDL_Rect *bounds);

void DesktopIcon_Draw(DesktopIcon *icon);

#endif
