#include"play.h"

void Play_Render()
{
    switch(gamePlay_state){
        case GAMEPLAY_STATE_VIEW_ROOM_1:
            Room1_Render();
            break;

        case GAMEPLAY_STATE_VIEW_ROOM_2:
            Room2_Render();
            break;

        case GAMEPLAY_STATE_VIEW_MONITOR:
            Monitor_Render();
            break;

        default:
            break;
    }
}

