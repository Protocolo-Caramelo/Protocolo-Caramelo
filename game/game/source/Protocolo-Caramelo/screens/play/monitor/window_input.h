#ifndef WINDOW_INPUT_H
#define WINDOW_INPUT_H

#include"window_manager.h"

/*
 * WindowInput: turns mouse events into window actions:
 *  - press on a window focuses it;
 *  - title bar: drag to move, double click to maximize/restore;
 *  - edges and corners: drag to resize;
 *  - title bar buttons: minimize / maximize / close (on release over the same button);
 *  - content: forwarded to the app's handle_event.
 * Windows are tracked by app id, so closing a window mid-interaction is safe.
 */

// Returns 1 if the event was used (cursor over a window, or a drag in progress)
int WindowInput_HandleEvent(const SDL_Event *e);
int WindowInput_IsBusy(void);
// Ends any drag or pending button press (e.g. when leaving the monitor)
void WindowInput_CancelInteraction(void);

#endif
