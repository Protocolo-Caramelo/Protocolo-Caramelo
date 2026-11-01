#include"monitor_desktop.h"
#include"monitor_cursor.h"
#include"icon_layer.h"
#include"window_manager.h"
#include"window_input.h"
#include"taskbar.h"
#include"game/game_globals.h"

static int initialized;

// An icon was activated: its app gets a window
static void onIconOpen(MonitorAppId app)
{
    WindowManager_Open(app);
}

void MonitorDesktop_Init(void)
{
    if(initialized){
        return;
    }
    MonitorCursor_Init();
    WindowManager_Init();
    IconLayer_Init(&onIconOpen);
    MonitorApps_InitAll();
    initialized = 1;
}

void MonitorDesktop_Quit(void)
{
    if(!initialized){
        return;
    }
    MonitorApps_QuitAll();
    IconLayer_Quit();
    WindowManager_Quit();
    MonitorCursor_Quit();
    initialized = 0;
}

void MonitorDesktop_SetSpace(const SDL_Rect *tex_dest, double scale_x, double scale_y)
{
    MonitorSpace_Update(tex_dest, scale_x, scale_y);
}

/*
 * Who gets a mouse event, in order:
 *  1. whoever is holding the mouse (a dragged icon, a pressed taskbar button);
 *  2. windows (they are above the icons);
 *  3. the taskbar;
 *  4. the icons.
 */
int MonitorDesktop_HandleEvent(const SDL_Event *e)
{
    int is_motion;

    if(!initialized || !MonitorSpace_IsReady()){
        return 0;
    }
    is_motion = (e->type == SDL_MOUSEMOTION);

    if(IconLayer_IsDragging()){
        return IconLayer_HandleEvent(e);
    }
    if(Taskbar_IsPressed()){
        return Taskbar_HandleEvent(e);
    }

    if(WindowInput_HandleEvent(e)){
        if(is_motion){
            // Things hidden under a window must not light up
            IconLayer_ClearHover();
            Taskbar_ClearHover();
        }
        return 1;
    }
    if(Taskbar_HandleEvent(e)){
        if(is_motion){
            IconLayer_ClearHover();
        }
        return 1;
    }
    return IconLayer_HandleEvent(e);
}

void MonitorDesktop_Render(void)
{
    SDL_Rect clip;

    if(!initialized || !MonitorSpace_IsReady()){
        return;
    }

    // Icons and windows may not leave the monitor's screen...
    clip = MonitorSpace_DesktopOnScreen();
    SDL_RenderSetClipRect(r, &clip);
    IconLayer_Render();
    WindowManager_Render();

    // ...and the taskbar stays in its strip
    clip = Taskbar_OnScreen();
    SDL_RenderSetClipRect(r, &clip);
    Taskbar_Render();

    SDL_RenderSetClipRect(r, NULL);
}

void MonitorDesktop_Leave(void)
{
    if(!initialized){
        return;
    }
    IconLayer_CancelInteraction();
    WindowInput_CancelInteraction();
    Taskbar_CancelInteraction();
    MonitorCursor_Set(MONITOR_CURSOR_ARROW);
}
