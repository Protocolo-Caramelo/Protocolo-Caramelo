#ifndef APP_MINER_H
#define APP_MINER_H

#include"app_communs.h"

void AppMiner_drawVertical(const SDL_Rect *body);
void AppMiner_drawHorizontal(const SDL_Rect *body);

void AppMiner_handleKeyDown(const SDL_Event *e);

//
void AppMiner_Init(void);
void AppMiner_Quit(void);
void AppMiner_Draw(const SDL_Rect *body);
void AppMiner_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside);

#endif
