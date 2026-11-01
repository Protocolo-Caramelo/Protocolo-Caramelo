#ifndef ICON_LAYER_H
#define ICON_LAYER_H

#include"desktop_icon.h"

/*
 * IconLayer: owns every desktop icon and handles the mouse for them:
 *  - press + drag moves an icon freely; on release it snaps to the nearest free
 *    cell of the icon grid (a ghost outline shows where it will land);
 *  - clicking (press and release without moving) selects it;
 *  - ICON_OPEN_CLICKS clicks in a row open its app through the on_open callback.
 */

// 1 = single click opens the app, 2 = double click (usual desktop behavior)
#define ICON_OPEN_CLICKS        2
// Mouse travel (screen pixels) before a press turns into a drag
#define ICON_DRAG_THRESHOLD_PX  4

typedef void (*IconLayerOpenFn)(MonitorAppId app);

void IconLayer_Init(IconLayerOpenFn on_open);
void IconLayer_Quit(void);

// Returns 1 if the event was used by the layer
int IconLayer_HandleEvent(const SDL_Event *e);
int IconLayer_IsDragging(void);
// Drops hover state and any drag in progress (e.g. when leaving the monitor)
void IconLayer_CancelInteraction(void);
void IconLayer_ClearHover(void);

void IconLayer_Render(void);

#endif
