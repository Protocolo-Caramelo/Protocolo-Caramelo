#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include"window_stack.h"

/*
 * WindowManager: what can be done to windows: open, close, focus, minimize,
 * maximize/restore, and drawing them. It does not read the mouse (see
 * window_input) and does not list windows (see window_stack).
 */

// Space (in monitor units) between cascaded windows
#define WINDOW_CASCADE_STEP 48

void WindowManager_Init(void);
void WindowManager_Quit(void);

// Opens the app's window, or focuses it if it is already open
DesktopWindow* WindowManager_Open(MonitorAppId app);
void WindowManager_Close(DesktopWindow *win);

// Brings the window to the front, un-minimizing it if needed
void WindowManager_Focus(DesktopWindow *win);
void WindowManager_Minimize(DesktopWindow *win);
// Maximizes the window over the whole desktop, or restores its previous size
void WindowManager_ToggleMaximize(DesktopWindow *win);

void WindowManager_Render(void);

#endif
