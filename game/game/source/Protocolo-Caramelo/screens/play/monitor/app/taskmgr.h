#ifndef APP_TASKMGR_H
#define APP_TASKMGR_H

#include"app_communs.h"

/*
 * Task manager app: lists the running apps (every open window except its own),
 * their state, and lets the player end them. Clicking a row brings that window
 * to the front; the "Encerrar" button closes it.
 * Registered in monitor_apps.c through these four functions.
 */
void AppTaskManager_Init(void);
void AppTaskManager_Quit(void);
void AppTaskManager_Draw(const SDL_Rect *body);
void AppTaskManager_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside);

#endif
