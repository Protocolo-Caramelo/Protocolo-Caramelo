#ifndef GAME_GLOBALS_STATE_H
#define GAME_GLOBALS_STATE_H

typedef enum GameState GameState;
typedef enum GamePlay_State GamePlay_State;
enum GameState {
    GAME_STATE_SCREEN_MENU,
    GAME_STATE_SCREEN_CONFIG,
    GAME_STATE_SCREEN_ABOUT,
    GAME_STATE_SCREEN_PLAY,
};
enum GamePlay_State {
    GAME_PLAY_STATE_VIEW_ROOM_1,
    GAME_PLAY_STATE_VIEW_ROOM_2,
    GAME_PLAY_STATE_VIEW_MONITOR,
};

extern GamePlay_State game_playState;
extern GameState game_state;

void GameState_Init();
void GameState_Quit();

#endif
