#include"game_menu.h"
#include"game_cache.h"

TextConfig *menu_textConfig;
TextConfig *menu_buttonConfig[INP_STATES_MAX];
Text        *menu_tTitle[MENU_TITLE_COUNT];
Button      *menu_buttons[MENU_BUTTONS_MAX];

void GameMenu_Init()
{
    menu_textConfig = 
        TextConfig_Create(
            .scale_width = 1 / 2.0,
            .scale_height = 1 / 2.0,
            .color = (SDL_Color){255, 180, 27, 255}
        )
        ;
    menu_buttonConfig[INP_STATE_NEUTRAL]  = MENU_BUTTON_CONFIG_NEUTRAL;
    menu_buttonConfig[INP_STATE_SELECTED] = MENU_BUTTON_CONFIG_SELECTED;
    menu_buttonConfig[INP_STATE_HOVER]    = MENU_BUTTON_CONFIG_HOVER;

    menu_tTitle[MENU_TITLE_PROTOCOLO] = Text_Create(
        cache[GFONT_3D],
        menu_textConfig,
        "Protocolo"
    );
    menu_tTitle[MENU_TITLE_CARAMELO] = Text_Create(
        cache[GFONT_3D],
        menu_textConfig,
        "C.A.R.A.M.E.L.O"
    );
    *Text_scaleWidth(menu_tTitle[MENU_TITLE_CARAMELO]) *= 1.5;
    *Text_scaleHeight(menu_tTitle[MENU_TITLE_CARAMELO]) *= 1.5;

    menu_buttons[MENU_BUTTON_INIT]      = Button_Create(cache[GFONT_REGULAR], &menu_buttonConfig, "Novo jogo"); 
    menu_buttons[MENU_BUTTON_CONTINUE]  = Button_Create(cache[GFONT_REGULAR], &menu_buttonConfig, "Continuar");
    menu_buttons[MENU_BUTTON_CONFIG]    = Button_Create(cache[GFONT_REGULAR], &menu_buttonConfig, "Configurar");
    menu_buttons[MENU_BUTTON_EXIT]      = Button_Create(cache[GFONT_REGULAR], &menu_buttonConfig, "Sair");
    menu_buttons[MENU_BUTTON_ABOUT]     = Button_Create(cache[GFONT_REGULAR], &menu_buttonConfig, "Sobre");

}

void GameMenu_Quit()
{
    size_t i;

    for(i=0; i < MENU_BUTTONS_MAX; ++i){
        Button_Destroy(&menu_buttons[i]);
    }
    for(i=0; i < MENU_TITLE_COUNT; ++i){
        Text_Destroy(&menu_tTitle[i]);
    }
}
