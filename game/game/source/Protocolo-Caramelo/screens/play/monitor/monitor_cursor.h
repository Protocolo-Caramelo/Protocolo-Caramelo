#ifndef MONITOR_CURSOR_H
#define MONITOR_CURSOR_H

#include<graphical.h>

/*
 * Mouse cursor shapes used inside the monitor (resize arrows, move, default).
 */
typedef enum MonitorCursor MonitorCursor;
enum MonitorCursor {
    MONITOR_CURSOR_ARROW,
    MONITOR_CURSOR_MOVE,
    MONITOR_CURSOR_RESIZE_H,
    MONITOR_CURSOR_RESIZE_V,
    MONITOR_CURSOR_RESIZE_NWSE,
    MONITOR_CURSOR_RESIZE_NESW,
    MONITOR_CURSOR_COUNT
};

void MonitorCursor_Init(void);
void MonitorCursor_Quit(void);
// Cheap to call every frame: does nothing if the shape is already active
void MonitorCursor_Set(MonitorCursor cursor);

#endif
