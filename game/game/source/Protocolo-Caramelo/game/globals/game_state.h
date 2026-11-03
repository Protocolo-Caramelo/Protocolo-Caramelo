#ifndef GAME_GLOBALS_STATE_H
#define GAME_GLOBALS_STATE_H

typedef enum Game_State Game_State;
typedef enum Game_PlayState Game_PlayState;
enum Game_State {
    GAME_STATE_SCREEN_MENU,
    GAME_STATE_SCREEN_CONFIG,
    GAME_STATE_SCREEN_ABOUT,
    GAME_STATE_SCREEN_PLAY,

    GAME_STATE_COUMT,
};
enum Game_PlayState {
    GAME_PLAY_STATE_VIEW_ROOM_1,
    GAME_PLAY_STATE_VIEW_ROOM_2,
    GAME_PLAY_STATE_VIEW_MONITOR,
    GAME_PLAY_STATE_PAUSED,

    GAME_PLAY_STATE_COUNT
};

extern Game_State game_state;
extern Game_PlayState game_playState;

void GameState_Init();
void GameState_Quit();

#endif
