#include"game_play.h"
#include"game_core.h"
#include"game_buttons.h"
#include"game_cache.h"
#include"game_others.h"


GamePlay_Texture gamePlay_tex;
SDL_Texture *play_tex[GPLAY_TEX_COUNT];
Button      *play_buttons[GPLAY_BUTTON_COUNT];
TextConfig  *play_buttonsArrowConfig[INP_STATES_MAX];
TextConfig  *play_buttonMonitorPowerConfig[INP_STATES_MAX];

static const char* play_texPaths[] = GPLAY_TEX_PATHS;

void GamePlay_Init()
{
    size_t i;

    memcpy(play_buttonsArrowConfig, (TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG, sizeof(play_buttonsArrowConfig));
    memcpy(play_buttonMonitorPowerConfig, (TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_MONITOR_POWER_CONFIG, sizeof(play_buttonMonitorPowerConfig));

    for(i=0;  i < GPLAY_TEX_COUNT; ++i){
        play_tex[i] = IMG_LoadTexture(r, play_texPaths[i]);
        if(!play_tex[i]){
            printf("Error while open play textures: %s\n", SDL_GetError());
        }
    }

    // Play Buttons
    play_buttons[GPLAY_BUTTON_ARROW_ROOM1_BACK]    = Button_Create(cache[GFONT_REGULAR], &play_buttonsArrowConfig, "Virar");
    play_buttons[GPLAY_BUTTON_ARROW_ROOM1_MONITOR] = Button_Create(cache[GFONT_REGULAR], &play_buttonsArrowConfig, "PC");
    play_buttons[GPLAY_BUTTON_ARROW_ROOM1_BED]     = Button_Create(cache[GFONT_REGULAR], &play_buttonsArrowConfig, "Cama");

    play_buttons[GPLAY_BUTTON_ARROW_ROOM2_BACK]    = Button_Create(cache[GFONT_REGULAR], &play_buttonsArrowConfig, "Virar");
    play_buttons[GPLAY_BUTTON_ARROW_ROOM2_DOOR]    = Button_Create(cache[GFONT_REGULAR], &play_buttonsArrowConfig, "Porta");

    play_buttons[GPLAY_BUTTON_MONITOR_BACK]         = Button_Create(cache[GFONT_REGULAR], &play_buttonMonitorPowerConfig, "");
}

void GamePlay_Quit()
{
    size_t i;

    for(i=0; i < GPLAY_TEX_COUNT; ++i){
        SDL_DestroyTexture(play_tex[i]);
    }
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
