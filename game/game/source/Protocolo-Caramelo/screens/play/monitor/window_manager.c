#include"window_manager.h"
#include"monitor_space.h"

DesktopWindow* WindowManager_Open(MonitorAppId app)
{
    SDL_Rect desktop;
    DesktopWindow *win;

    win = WindowStack_Find(app);
    if(win){
        WindowManager_Focus(win);
        return win;
    }

    desktop = MonitorSpace_Desktop();
    win = DesktopWindow_Create(app, &desktop, WindowStack_Count() * WINDOW_CASCADE_STEP);
    if(!win){
        return NULL;
    }
    if(!WindowStack_Add(win)){
        DesktopWindow_Destroy(&win);
        return NULL;
    }
    return win;
}

void WindowManager_Close(DesktopWindow *win)
{
    if(!WindowStack_Contains(win)){
        return;
    }
    WindowStack_Remove(win);
    DesktopWindow_Destroy(&win);
}

void WindowManager_Focus(DesktopWindow *win)
{
    DesktopWindow_SetMinimized(win, 0);
    WindowStack_BringToFront(win);
}

void WindowManager_Minimize(DesktopWindow *win)
{
    // The stack order is kept: the next window below becomes the active one
    DesktopWindow_SetMinimized(win, 1);
}

void WindowManager_ToggleMaximize(DesktopWindow *win)
{
    SDL_Rect desktop;

    if(win->maximized){
        DesktopWindow_Restore(win);
        return;
    }
    desktop = MonitorSpace_Desktop();
    DesktopWindow_Maximize(win, &desktop);
}

void WindowManager_Init(void)
{
    // Nothing to set up: windows are created on demand
}

void WindowManager_Quit(void)
{
    while(WindowStack_Count() > 0){
        WindowManager_Close(WindowStack_AtZ(0));
    }
}

void WindowManager_Render(void)
{
    DesktopWindow *active;
    int i;

    active = WindowStack_Active();
    for(i = 0; i < WindowStack_Count(); ++i){
        DesktopWindow_Draw(WindowStack_AtZ(i), WindowStack_AtZ(i) == active);
    }
}
