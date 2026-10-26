#include"main.h"

void Test_libgraphical();
void Test_inputs();

int main(int argc, char*argv[])
{
    extern SDL_Event e;
    extern GameState gameState;

    Game_Init();
    while(1){
        switch(gameState){
            case GAME_STATE_SCREEN_MENU:
                Menu_Render();
                break;

            default:
                break;
        }

        Game_Debug();

        SDL_RenderPresent(r);
        SDL_Delay(32);
    }

    return 0;
}

void Test_libgraphical()
{
    /*
    const char font_path[] = 
        // "./assets/fonts/Ubuntu/Ubuntu-Regular.ttf"
        "./assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf"
        // "./assets/fonts/pixeloid-font/PixeloidMono-nAOpP.ttf"
        ;
        // */
    /*
     * Please, run one test per program...
     */
    // 
    // Test SDL
    // // SimpleWindow();
    //
    // Text TextFA struct
    //
    // TextCache_Test(font_path, 50);

    //
    // Test Text struct
    //
    /*
    Text_Test1(
        font_path,
        50,
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In mollis faucibus porttitor. Mauris tempor ut lorem ac laoreet. Quisque facilisis aliquet mauris a blandit. Sed magna ante, faucibus sit amet purus ac, eleifend posuere nunc.\nDuis id elit vulputate, sollicitudin odio nec, volutpat massa. Praesent auctor tincidunt massa, vitae interdum felis euismod sed. Vivamus ultrices, odio ac ullamcorper tincidunt, est urna iaculis lorem, a venenatis nunc magna ac quam. Donec nunc turpis, bibendum a lobortis at, auctor vitae augue."
        "#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
        );
    // */
    /*
    Text_Test2(
        font_path,
        50,
        // "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In mollis faucibus porttitor. Mauris tempor ut lorem ac laoreet."
        "#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
        // "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        // "Protocolo-Caramelo"
        // "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In mollis faucibus porttitor. Mauris tempor ut lorem ac laoreet. Quisque facilisis aliquet mauris a blandit. Sed magna ante, faucibus sit amet purus ac, eleifend posuere nunc.\nDuis id elit vulputate, sollicitudin odio nec, volutpat massa. Praesent auctor tincidunt massa, vitae interdum felis euismod sed. Vivamus ultrices, odio ac ullamcorper tincidunt, est urna iaculis lorem, a venenatis nunc magna ac quam. Donec nunc turpis, bibendum a lobortis at, auctor vitae augue."
    );
    // */
    /*
    Text_Test3(
        font_path,
        50,
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In mollis faucibus porttitor. Mauris tempor ut lorem ac laoreet. Quisque facilisis aliquet mauris a blandit. Sed magna ante, faucibus sit amet purus ac, eleifend posuere nunc.\nDuis id elit vulputate, sollicitudin odio nec, volutpat massa. Praesent auctor tincidunt massa, vitae interdum felis euismod sed. Vivamus ultrices, odio ac ullamcorper tincidunt, est urna iaculis lorem, a venenatis nunc magna ac quam. Donec nunc turpis, bibendum a lobortis at, auctor vitae augue."
    );
    // */
    /*
    Text_Test4(
        font_path,
        90,
        "Click Here!"
    );
    // */
}

void Test_inputs()
{
    extern SDL_Window *w;
    extern SDL_Renderer *r;
    extern SDL_Event e;
    extern TextCache *cache_fRegular;
    extern TextCache *cache_fTitle;
    //
    TextConfig *bi_tc[INP_STATES_MAX] = {
        [INP_STATE_NEUTRAL] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 2,
                .right = 2,
                .top = 2,
                .bottom = 2,
                .color = (SDL_Color){.r=55, .g=55, .b=55, .a = 255}
            },
            .padding.left = 2,
            .padding.color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            // .bx_width = 100,
            .color_bg = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        ),
        [INP_STATE_SELECTED] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 5,
                .right = 5,
                .top = 5,
                .bottom = 5,
                .color = (SDL_Color){.a = 255}
            },
            .padding.left = 2,
            .padding.color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            // .bx_width = 100,
        ),
        [INP_STATE_HOVER] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 5,
                .right = 5,
                .top = 5,
                .bottom = 5,
                .color = (SDL_Color){.a = 255}
            },
            .padding.left = 2,
            .padding.color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            // .bx_width = 100,
        )
    };
    BarInput *bi;
    //
    Button *b;
    TextConfig *b_tc[INP_STATES_MAX] = {
        [INP_STATE_NEUTRAL] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 5,
                .right = 5,
                .top = 5,
                .bottom = 5,
                .color = (SDL_Color){.a = 255}
            },
            .padding = (Padding){
                .left = 5,
                .right = 5,
                .top = 5,
                .bottom = 5,
                .color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            },
            .color_bg = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            .chr_fmt = CHAR_FMT_REGULAR,
        ),
        [INP_STATE_SELECTED] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 10,
                .right = 10,
                .top = 10,
                .bottom = 10,
                .color = (SDL_Color){.a = 255}
            },
            .padding = (Padding){
                .left = 2,
                .right = 2,
                .top = 2,
                .bottom = 2,
                .color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            },
            .color_bg = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            .chr_fmt = CHAR_FMT_UNDERLINE,
        )
    };
    //
    TextInput *ti;
    TextConfig *ti_tc[INP_STATES_MAX] = {
        [INP_STATE_NEUTRAL] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 2,
                .right = 2,
                .top = 2,
                .bottom = 2,
                .color = (SDL_Color){.a = 255}
            },
            .padding = (Padding){
                .left = 2,
                .right = 2,
                .top = 2,
                .bottom = 2,
                .color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            },
            .color_bg = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            .wrap = 200,
        ),
        [INP_STATE_SELECTED] = TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .margin = (Margin){
                .left = 5,
                .right = 5,
                .top = 5,
                .bottom = 5,
                .color = (SDL_Color){.a = 255}
            },
            .padding = (Padding){
                .left = 2,
                .right = 2,
                .top = 2,
                .bottom = 2,
                .color = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            },
            .color_bg = (SDL_Color){.r=255, .g=255, .b=255, .a=255},
            .wrap = 200,
        ),
    };


    
    graphical_init();
    w = SDL_CreateWindow(
        ELEMENT_WINDOW_NAME,
        ELEMENT_WINDOW_X_POSITION,
        ELEMENT_WINDOW_Y_POSITION,
        ELEMENT_WINDOW_W,
        ELEMENT_WINDOW_H,
        ELEMENT_WINDOW_FLAGS
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        ELEMENT_RENDERER_FLAGS
    );
    //
    cache_fRegular = TextCache_Create(
        r,
        ELEMENT_CACHE_FREGULAR_PATH,
        ELEMENT_CACHE_FREGULAR_PTSIZE
    );
    cache_fTitle = TextCache_Create(
        r,
        ELEMENT_CACHE_FTITLE_PATH,
        ELEMENT_CACHE_FTITLE_PTSIZE
    );
    //
    bi = BarInput_Create(
        cache_fRegular, &bi_tc,
        "|%.2lf",
        0, 100,
        200, 0,
        (SDL_Color){}, (SDL_Color){}
    );
    //
    b = Button_Create(
        cache_fTitle,
        &b_tc,
        "Click Here!"
    );
    //
    ti = TextInput_Create(
        cache_fRegular,
        &ti_tc,
        2, 0,
        "AAAA"
    );

    while(1){
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        while(SDL_PollEvent(&e)){
            BarInput_handleInput(bi, &e);
            Button_handleInput(b, &e);
            TextInput_handleInput(ti, &e);

            switch(e.type){
                case SDL_QUIT:
                    BarInput_Destroy(&bi);
                    Button_Destroy(&b);
                    TextInput_Destroy(&ti);
                    //
                    TextCache_Destroy(&cache_fRegular);
                    TextCache_Destroy(&cache_fTitle);
                    //
                    SDL_DestroyRenderer(r);
                    SDL_DestroyWindow(w);
                    //
                    graphical_quit();
                    SDL_Quit();

                    exit(0);
                    break;

                default:
                    break;
            }
        }
        BarInput_Display(bi, 100, 100, NULL);
        Button_Display(b, 0, 0, NULL);
        TextInput_Display(ti, 500, 100, NULL);

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
}
