#include"game_play.h"
#include"game_core.h"
#include"game_cache.h"
#include"game_menu.h"
#include"game_state.h"
#include"game_buttons.h"
#include"game_mechanics.h"
#include"game_others.h"

#include"screens/play/room.h"
#include"screens/play/monitor.h"


GamePlay_Texture gamePlay_tex;
//
TextConfig  *play_buttonsArrowConfig[INP_STATES_MAX];
TextConfig  *play_buttonMonitorPowerConfig[INP_STATES_MAX];
Text        *play_text[GPLAY_TEXT_COUNT];
SDL_Texture *play_tex[GPLAY_TEX_COUNT];
Button      *play_buttons[GPLAY_BUTTON_COUNT];

static const char* play_texPaths[] = GPLAY_TEX_PATHS;

void GamePlay_Init()
{
    size_t i;

    memcpy(play_buttonsArrowConfig, (TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG, sizeof(play_buttonsArrowConfig));
    memcpy(play_buttonMonitorPowerConfig, (TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_MONITOR_POWER_CONFIG, sizeof(play_buttonMonitorPowerConfig));

    // Texts
    memcpy(play_text, (Text*[GPLAY_TEXT_COUNT])GPLAY_TEXT, sizeof(play_text));

    // Textures
    SDL_Texture **tex;
    for(i=GPLAY_TEX_ROOM_1;  i <= GPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED; ++i){
        tex = &play_tex[i];
        *tex = IMG_LoadTexture(r, play_texPaths[i]);
        if(!*tex){
            printf("Error while open play textures: %s\n", SDL_GetError());
        }
    }
    //
    tex = &play_tex[GPLAY_TEX_PAUSED];
    *tex = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        GPLAY_TEX_W, GPLAY_TEX_H
    );
    if(!*tex){
        printf("Error while open play textures: %s\n", SDL_GetError());
    }
    SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(r, *tex);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 200);
    SDL_RenderClear(r);
    SDL_SetRenderTarget(r, NULL);

    // Buttons
    memcpy(play_buttons, (Button*[GPLAY_BUTTON_COUNT])GPLAY_BUTTONS, sizeof(play_buttons));

    // Change game State
    game_state = GAME_STATE_SCREEN_PLAY;
    game_playState = GAME_PLAY_STATE_VIEW_ROOM_1;
    gamePlay_tex = GPLAY_TEX_ROOM_1;

    GameMechanics_Init();
}

void GamePlay_Quit()
{
    size_t i;

    for(i=0; i < GPLAY_TEXT_COUNT; ++i){
        if(!play_text[i]){
            continue;
        }
        Text_Destroy(&play_text[i]);
    }

    for(i=0; i < GPLAY_TEX_COUNT; ++i){
        if(!play_tex[i]){
            continue;
        }
        SDL_DestroyTexture(play_tex[i]);
    }

    for(i=0; i < GPLAY_BUTTON_COUNT; ++i){
        if(!play_buttons[i]){
            continue;
        }
        Button_Destroy(&play_buttons[i]);
    }

    Room1_Quit();
    Room2_Quit();
    Monitor_Quit();

    GameMechanics_Quit();
}

// Button type alternate functions
void gamePlay_Button_Arrow_Neutral_alterBorder(Text*t, Texture*tex)
{
    SDL_Rect dest;

    SDL_GetWindowSize(w, &w_width, &w_height);
    //
    dest = b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL];
    if(w_width / (double)w_height < dest.w / (double)dest.h){
        dest.w *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)GCORE_WINDOW_W;
        dest.h *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)GCORE_WINDOW_W;
    }
    else {
        dest.w *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)GCORE_WINDOW_W;
        dest.h *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)GCORE_WINDOW_W;
    }
    //
    dest.x = ((*Text_edgesBack(t))[DRAW_TEXT_WIDTH] + *Text_borderLeft(t) + *Text_borderRight(t)) / 2 - dest.w / 2;
    dest.y = (*Text_edgesBack(t))[DRAW_EDGE_TEXT_YMAX];
    //
    SDL_RenderCopy(
        r,
        b_arrowTex,
        &b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL],
        &dest
    );
}

void gamePlay_Button_Arrow_Selected_alterBorder(Text*t, Texture*tex)
{
    SDL_Rect dest;

    SDL_GetWindowSize(w, &w_width, &w_height);
    //
    dest = b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL];
    if(w_width / (double)w_height < dest.w / (double)dest.h){
        dest.w *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)GCORE_WINDOW_W;
        dest.h *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)GCORE_WINDOW_W;
    }
    else {
        dest.w *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)GCORE_WINDOW_W;
        dest.h *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)GCORE_WINDOW_W;
    }
    //
    dest.x = ((*Text_edgesBack(t))[DRAW_TEXT_WIDTH] + *Text_borderLeft(t) + *Text_borderRight(t)) / 2 - dest.w / 2;
    dest.y = (*Text_edgesBack(t))[DRAW_EDGE_TEXT_YMAX];
    //
    SDL_RenderCopy(
        r,
        b_arrowTex,
        &b_arrowState[GBUTTON_ARROW_STATE_SELECTED],
        &dest
    );
}

void gamePlay_Button_Arrow_Hover_alterBorder(Text*t, Texture*tex)
{
    SDL_Rect dest;
    int w_width, w_height;

    SDL_GetWindowSize(w, &w_width, &w_height);
    //
    dest = b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL];
    if(w_width / (double)w_height < dest.w / (double)dest.h){
        dest.w *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)GCORE_WINDOW_W;
        dest.h *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)GCORE_WINDOW_W;
    }
    else {
        dest.w *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)GCORE_WINDOW_W;
        dest.h *= GPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)GCORE_WINDOW_W;
    }
    //
    dest.x = ((*Text_edgesBack(t))[DRAW_TEXT_WIDTH] + *Text_borderLeft(t) + *Text_borderRight(t)) / 2 - dest.w / 2;
    dest.y = (*Text_edgesBack(t))[DRAW_EDGE_TEXT_YMAX];
    //
    SDL_RenderCopy(
        r,
        b_arrowTex,
        &b_arrowState[GBUTTON_ARROW_STATE_HOVER],
        &dest
    );
}
