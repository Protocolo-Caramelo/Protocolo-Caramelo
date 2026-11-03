#include"play.h"
#include"play/room.h"
#include"play/monitor.h"
#include"game/game.h"
#include"screens_communs.h"

void Play_Render()
{
    switch(game_playState){
        case GAME_PLAY_STATE_VIEW_ROOM_1:
            Room1_Render();
            break;

        case GAME_PLAY_STATE_VIEW_ROOM_2:
            Room2_Render();
            break;

        case GAME_PLAY_STATE_VIEW_MONITOR:
            Monitor_Render();
            break;

        default:
            break;
    }

    Game_PlayManageMathProblem();
}

