#ifndef TASKBAR_H
#define TASKBAR_H

#include"window_manager.h"

/*
 * Taskbar: one button per open window, in the strip of the monitor art next to
 * the power button. Clicking a button works like in any desktop:
 *  - minimized window  -> restores it;
 *  - active window     -> minimizes it;
 *  - any other window  -> brings it to the front.
 */

// Area where the buttons live, in monitor units (between the power button and the divider)
#define TASKBAR_AREA_X           765
#define TASKBAR_AREA_Y           1345
#define TASKBAR_AREA_W           825
#define TASKBAR_AREA_H           125

#define TASKBAR_BUTTON_MAX_W    260
#define TASKBAR_BUTTON_GAP      10

SDL_Rect Taskbar_OnScreen(void);

// Returns 1 if the event was used (cursor over a button, or a press in progress)
int Taskbar_HandleEvent(const SDL_Event *e);
// 1 while a button is held: the taskbar then keeps the mouse until the release
int Taskbar_IsPressed(void);
void Taskbar_ClearHover(void);
void Taskbar_CancelInteraction(void);

void Taskbar_Render(void);

#endif
