#ifndef WINDOW_STACK_H
#define WINDOW_STACK_H

#include"desktop_window.h"

/*
 * WindowStack: the list of open windows, kept in two orders at once:
 *  - z-order: bottom to top, what decides who is drawn over whom and who gets clicks;
 *  - open order: the order windows were opened, used by the taskbar and task manager.
 * It only stores and queries windows; creating, closing and states live in window_manager.
 * There is at most one window per app.
 */

int WindowStack_Count(void);
// i = 0 is the bottom window
DesktopWindow* WindowStack_AtZ(int i);
// i = 0 is the oldest window
DesktopWindow* WindowStack_AtOpenOrder(int i);

// app can be MONITOR_APP_NONE (nothing is found then)
DesktopWindow* WindowStack_Find(int app);
int WindowStack_Contains(const DesktopWindow *win);

// Adds a window on top. Returns 0 if the stack is full.
int WindowStack_Add(DesktopWindow *win);
void WindowStack_Remove(const DesktopWindow *win);
void WindowStack_BringToFront(const DesktopWindow *win);

// Topmost window that is not minimized: the one with the focus. NULL if none.
DesktopWindow* WindowStack_Active(void);
// Topmost window under (ux, uy), with what was hit. NULL (part NONE) if none.
DesktopWindow* WindowStack_TopmostAt(int ux, int uy, DesktopWindowHit *hit);

// Copies the windows (z-order) to out, which must hold MONITOR_APP_COUNT entries.
// Lets callers iterate safely while handlers may close windows.
int WindowStack_Copy(DesktopWindow **out);

#endif
