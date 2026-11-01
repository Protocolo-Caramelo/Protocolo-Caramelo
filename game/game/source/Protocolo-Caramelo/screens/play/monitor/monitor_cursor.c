#include"monitor_cursor.h"

static SDL_Cursor *cursors[MONITOR_CURSOR_COUNT];
static MonitorCursor current = MONITOR_CURSOR_ARROW;

static const SDL_SystemCursor system_cursors[MONITOR_CURSOR_COUNT] = {
    [MONITOR_CURSOR_ARROW]        = SDL_SYSTEM_CURSOR_ARROW,
    [MONITOR_CURSOR_MOVE]         = SDL_SYSTEM_CURSOR_SIZEALL,
    [MONITOR_CURSOR_RESIZE_H]     = SDL_SYSTEM_CURSOR_SIZEWE,
    [MONITOR_CURSOR_RESIZE_V]     = SDL_SYSTEM_CURSOR_SIZENS,
    [MONITOR_CURSOR_RESIZE_NWSE]  = SDL_SYSTEM_CURSOR_SIZENWSE,
    [MONITOR_CURSOR_RESIZE_NESW]  = SDL_SYSTEM_CURSOR_SIZENESW,
};

void MonitorCursor_Init(void)
{
    size_t i;

    for(i = 0; i < MONITOR_CURSOR_COUNT; ++i){
        // The arrow is the OS default cursor: nothing to create
        cursors[i] = (i == MONITOR_CURSOR_ARROW)? NULL: SDL_CreateSystemCursor(system_cursors[i]);
    }
    current = MONITOR_CURSOR_ARROW;
}

void MonitorCursor_Quit(void)
{
    size_t i;

    MonitorCursor_Set(MONITOR_CURSOR_ARROW);
    for(i = 0; i < MONITOR_CURSOR_COUNT; ++i){
        if(cursors[i]){
            SDL_FreeCursor(cursors[i]);
            cursors[i] = NULL;
        }
    }
}

void MonitorCursor_Set(MonitorCursor cursor)
{
    if(cursor == current){
        return;
    }
    current = cursor;
    // Missing cursors (creation failed) fall back to the default one
    SDL_SetCursor(cursors[cursor]? cursors[cursor]: SDL_GetDefaultCursor());
}
