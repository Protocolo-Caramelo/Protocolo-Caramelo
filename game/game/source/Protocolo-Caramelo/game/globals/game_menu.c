#include"game_menu.h"
#include"game_cache.h"

Text        *menu_tTitle[MENU_TITLE_COUNT];
Button      *menu_buttons[MENU_BUTTONS_MAX];

void GameMenu_Init()
{
    menu_tTitle[MENU_TITLE_PROTOCOLO] = Text_Create(
        cache[GFONT_3D],
        MENU_TITLE_CONFIG,
        "Protocolo"
    );
    menu_tTitle[MENU_TITLE_CARAMELO] = Text_Create(
        cache[GFONT_3D],
        MENU_TITLE_CONFIG,
        "C.A.R.A.M.E.L.O"
    );
    *Text_scaleWidth(menu_tTitle[MENU_TITLE_CARAMELO]) *= 1.5;
    *Text_scaleHeight(menu_tTitle[MENU_TITLE_CARAMELO]) *= 1.5;

    menu_buttons[MENU_BUTTON_INIT]      = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Novo jogo"); 
    menu_buttons[MENU_BUTTON_CONTINUE]  = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Continuar");
    menu_buttons[MENU_BUTTON_GCONFIG]   = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Configurar");
    menu_buttons[MENU_BUTTON_EXIT]      = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Sair");
    menu_buttons[MENU_BUTTON_ABOUT]     = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Sobre");
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
