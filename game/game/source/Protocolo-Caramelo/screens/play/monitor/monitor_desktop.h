#ifndef MONITOR_DESKTOP_H
#define MONITOR_DESKTOP_H

#include"monitor_space.h"
#include"monitor_apps.h"

/*
 * MonitorDesktop: the "operating system" shown on the monitor. It ties together
 * the icon layer and the window manager and is the only thing the monitor screen
 * (screens/play/monitor.c) needs to talk to.
 *
 *   Init/Quit        once, while the renderer and font caches exist
 *   SetSpace         every frame, after drawing the monitor background
 *   HandleEvent      for every SDL event while the monitor is on screen
 *   Render           every frame, after SetSpace
 *   Leave            when the player leaves the monitor view
 */
void MonitorDesktop_Init(void);
void MonitorDesktop_Quit(void);

void MonitorDesktop_SetSpace(const SDL_Rect *tex_dest, double scale_x, double scale_y);
// Returns 1 if the event was used by the desktop
int MonitorDesktop_HandleEvent(const SDL_Event *e);
void MonitorDesktop_Render(void);
void MonitorDesktop_Leave(void);

#endif
