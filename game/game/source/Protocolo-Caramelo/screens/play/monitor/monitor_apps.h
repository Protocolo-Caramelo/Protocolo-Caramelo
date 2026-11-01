#ifndef MONITOR_APPS_H
#define MONITOR_APPS_H

#include<graphical.h>

/*
 * App registry
 * ------------
 * One entry per application that exists on the monitor's desktop. To add an app:
 *  1. add an id to MonitorAppId (before MONITOR_APP_COUNT);
 *  2. fill its entry in monitor_apps.c (label, title, icon, grid cell, sizes);
 *  3. optionally give it init/quit, draw_content and handle_event functions.
 * Icons and windows are created from this table, nothing else needs to change.
 */
typedef enum MonitorAppId MonitorAppId;
enum MonitorAppId {
    MONITOR_APP_TRASH,
    MONITOR_APP_MINE,
    MONITOR_APP_DELIVERY,
    MONITOR_APP_TASKMGR,
    MONITOR_APP_COUNT
};
// "No app" marker for variables that remember an app id
#define MONITOR_APP_NONE (-1)

/*
 * Draws the inside of an app window. body is the content area in screen pixels
 * (already clipped to it). NULL draw_content shows a placeholder.
 */
typedef void (*MonitorAppDrawFn)(const SDL_Rect *body);

/*
 * Receives mouse events (motion, button down/up) for an app window.
 *  body            content area in screen pixels, same as in draw_content
 *  pointer_inside  1 if the mouse is over this window's content and nothing
 *                  (another window) is covering it
 * Button presses only arrive when pointer_inside is 1; the matching release
 * always arrives, even if the mouse left the window in the meantime.
 */
typedef void (*MonitorAppEventFn)(const SDL_Event *e, const SDL_Rect *body, int pointer_inside);

typedef struct MonitorApp MonitorApp;
struct MonitorApp {
    const char *icon_label;
    const char *window_title;
    const char *icon_path;

    SDL_Point icon_cell;                 // initial icon position: (col, row) of the icon grid
    int window_w, window_h;              // initial window size, in monitor units
    int window_min_w, window_min_h;      // smallest size the window can be resized to

    void (*init)(void);                  // optional: once, when the desktop starts
    void (*quit)(void);                  // optional: once, when the desktop stops
    MonitorAppDrawFn draw_content;       // optional
    MonitorAppEventFn handle_event;      // optional
};

const MonitorApp* MonitorApps_Get(MonitorAppId id);

// Run every app's init/quit hook
void MonitorApps_InitAll(void);
void MonitorApps_QuitAll(void);

#endif
