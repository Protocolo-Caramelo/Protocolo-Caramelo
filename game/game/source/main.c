#include"main.h"

void Test_libgraphical();
void Test_inputs();
void Test_problems();

int main(int argc, char*argv[])
{
    //*
    extern SDL_Event e;
    extern GameState gameState;

    Game_Init();
    while(1){
        switch(gameState){
            case GAME_STATE_SCREEN_MENU:
                Menu_Render();
                break;

            case GAME_STATE_SCREEN_PLAY:
                Play_Render();
                break;

            default:
                break;
        }

        Game_Debug();

        SDL_RenderPresent(r);
        SDL_Delay(32);
    }
    // */
    
    // Test_problems();

    return 0;
}

//
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

void Test_problems()
{
    Button *b_view_answer;
    Button *b_generate;
    Button *b_difficulty[MATHPROBLEM_DIFFICULTY_COUNT];
    Button *b_type[MATHPROBLEM_TYPE_COUNT];
    TextConfig *b_config[INP_STATES_MAX] = {
        [INP_STATE_NEUTRAL] = 
            TextConfig_Create(
                .scale_width = 1 / 4.0,
                .scale_height = 1 / 4.0,
                .color = (SDL_Color){
                    .a = 255
                },
                .color_bg = (SDL_Color){
                    .r = 255,
                    .g = 255,
                    .b = 255,
                    .a = 255
                },
                .margin = (Margin){
                    .left   = 2,
                    .right  = 2,
                    .bottom = 2,
                    .top    = 2,
                    .color = (SDL_Color){
                        .a = 255
                    }
                },
                .padding = (Padding){
                    .left   = 5,
                    .right  = 5,
                    .bottom = 5,
                    .top    = 5,
                    .color = (SDL_Color){
                        .r = 255,
                        .g = 255,
                        .b = 255,
                        .a = 255
                    }
                },
            ),
        [INP_STATE_SELECTED] = 
            TextConfig_Create(
                .scale_width = 1 / 4.0,
                .scale_height = 1 / 4.0,
                .color = (SDL_Color){
                    .a = 255
                },
                .color_bg = (SDL_Color){
                    .r = 255,
                    .g = 255,
                    .b = 255,
                    .a = 255
                },
                .margin = (Margin){
                    .left   = 5,
                    .right  = 5,
                    .bottom = 5,
                    .top    = 5,
                    .color = (SDL_Color){
                        .a = 255
                    }
                },
                .padding = (Padding){
                    .left   = 5,
                    .right  = 5,
                    .bottom = 5,
                    .top    = 5,
                    .color = (SDL_Color){
                        .r = 255,
                        .g = 255,
                        .b = 255,
                        .a = 255
                    }
                },
            ),
    };
    Text *difficulty_t;
    Text *type_t;
    //
    MathProblem_Difficulty p_difficulty;
    MathProblem_Type p_type;
    MathProblem problem;
    Text *p_t;
    //
    Text *answer_t;
    TextInput *answer;
    Button *b_submit;
    TextConfig *b_submit_config[INP_STATES_MAX] = {
        [INP_STATE_NEUTRAL] = 
            TextConfig_Create(
                .scale_width = 1 / 4.0,
                .scale_height = 1 / 4.0,
                .color = (SDL_Color){255, 255, 255, 255},
                .margin = (Margin){
                    .left   = 2,
                    .right  = 2,
                    .bottom = 2,
                    .top    = 2,
                    .color = (SDL_Color){
                        .a = 255
                    }
                },
                .padding = (Padding){
                    .left   = 5,
                    .right  = 5,
                    .bottom = 5,
                    .top    = 5,
                    .color = (SDL_Color){
                    .r = 139,
                    .g = 201,
                    .b = 20,
                    .a = 255
                    },
                },
            ),
        [INP_STATE_SELECTED] = 
            TextConfig_Create(
                .scale_width = 1 / 4.0,
                .scale_height = 1 / 4.0,
                .color = (SDL_Color){255, 255, 255, 255},
                .margin = (Margin){
                    .left   = 5,
                    .right  = 5,
                    .bottom = 5,
                    .top    = 5,
                    .color = (SDL_Color){
                        .a = 255
                    }
                },
                .padding = (Padding){
                    .left   = 5,
                    .right  = 5,
                    .bottom = 5,
                    .top    = 5,
                    .color = (SDL_Color){
                        .r = 139,
                        .g = 201,
                        .b = 20,
                        .a = 255
                    }
                },
            ),
    };
    //
    size_t i;
    int x, y;
    char *b_title_difficulty[MATHPROBLEM_DIFFICULTY_COUNT] = {
        "Facil", "Mediano", "Dificil", "Impossivel"
    };
    char *b_title_type[MATHPROBLEM_TYPE_COUNT] = {
        "Normal", "Equacao 1o grau", "Equacao 2o grau", "Binomio de Newton",
        "Funcao linear", "Funcao quadratica", "Funcao exponencial", "Funcao logaritmica"
    };

    Game_Init();
    //
    b_generate = Button_Create(cache_fRegular, &b_config, "Gerar problema");
    b_view_answer = Button_Create(cache_fRegular, &b_config, "Ver resposta");
    //
    for(i=MATHPROBLEM_DIFFICULTY_EASY; i < MATHPROBLEM_DIFFICULTY_COUNT; ++i){
        b_difficulty[i] = Button_Create(cache_fRegular, &b_config, b_title_difficulty[i]);
    }
    for(i=MATHPROBLEM_NORMAL; i < MATHPROBLEM_TYPE_COUNT; ++i){
        b_type[i] = Button_Create(cache_fRegular, &b_config, b_title_type[i]);
    }
    //
    type_t = Text_Create(cache_fTitle, b_config[0], "Escolha um tipo: ");
    difficulty_t = Text_Create(cache_fTitle, b_config[0], "Escolha uma dificuldade: ");
    //
    p_difficulty = MATHPROBLEM_DIFFICULTY_EASY;
    p_type = MATHPROBLEM_NORMAL;
    p_t = Text_Create(cache_fRegular, b_config[0], "(Sem problemas)");
    //
    answer_t = Text_Create(
        cache_fTitle,
        TextConfig_Create(
            .scale_width = 1 / 4.0,
            .scale_height = 1 / 4.0
        )
    );
    answer = TextInput_Create(
        cache_fRegular,
        &b_config,
        5, 0,
        "(Insira sua resposta aqui)"
    );
    b_submit = Button_Create(cache_fRegular, &b_submit_config, "Submissão");

    while(1){
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);

        while(SDL_PollEvent(&e)){
            for(i=MATHPROBLEM_DIFFICULTY_EASY; i < MATHPROBLEM_DIFFICULTY_COUNT; ++i){
                Button_handleInput(b_difficulty[i], &e);
            }
            for(i=MATHPROBLEM_NORMAL; i < MATHPROBLEM_TYPE_COUNT; ++i){
                Button_handleInput(b_type[i], &e);
            }
            Button_handleInput(b_view_answer, &e);
            Button_handleInput(b_generate, &e);
            Button_handleInput(b_submit, &e);
            TextInput_handleInput(answer, &e);

            switch(e.type){
                case SDL_QUIT:
                    Game_Quit();
                    break;

                default:
                    break;
            }
        }

        SDL_GetWindowSize(w, &w_width, &w_height);

        //
        for(i=MATHPROBLEM_DIFFICULTY_EASY; i < MATHPROBLEM_DIFFICULTY_COUNT; ++i){
            if(b_difficulty[i]->state[INP_STATE_SELECTED] || *Text_raw(answer_t) != NULL){
                free(*Text_raw(answer_t));
                *Text_raw(answer_t) = NULL;
            }
        }
        for(i=MATHPROBLEM_NORMAL; i < MATHPROBLEM_TYPE_COUNT; ++i){
            if(b_difficulty[i]->state[INP_STATE_SELECTED] || *Text_raw(answer_t) != NULL){
                free(*Text_raw(answer_t));
                *Text_raw(answer_t) = NULL;
            }
        }

        // Display difficulty of problems
        Text_DrawUTF8(
            difficulty_t,
            NULL,
            TextConfig_Create(
                .x = 0,
                .y = 0,
                .chr_fmt = CHAR_FMT_BOLD
            ),
            NULL
        );
        x = 0;
        y = (*Text_edges(difficulty_t))[DRAW_EDGE_YMAX] + 10;
        for(i=MATHPROBLEM_DIFFICULTY_EASY; i < MATHPROBLEM_DIFFICULTY_COUNT; ++i){
            if(b_difficulty[i]->state[INP_STATE_SELECTED]){
                p_difficulty = i;
            }
            //
            b_difficulty[i]->state[INP_STATE_SELECTED] = i == p_difficulty;
            Button_Display(b_difficulty[i], x, y, NULL);
            b_difficulty[i]->state[INP_STATE_SELECTED] = 0;
            x += (*Text_edges(b_difficulty[i]->text))[DRAW_WIDTH] + 10;
        }
        // Display type of problems
        Text_DrawUTF8(
            type_t,
            NULL,
            TextConfig_Create(
                .x = 0,
                .y = (*Text_edges(b_difficulty[i-1]->text))[DRAW_EDGE_YMAX] + 10,
                .chr_fmt = CHAR_FMT_BOLD
            ),
            NULL
        );
        x = 0;
        y = (*Text_edges(type_t))[DRAW_EDGE_YMAX] + 10;
        for(i=MATHPROBLEM_NORMAL; i < MATHPROBLEM_TYPE_COUNT; ++i){
            if(b_type[i]->state[INP_STATE_SELECTED]){
                p_type = i;
            }
            //
            if(x + (*Text_edges(b_type[i]->text))[DRAW_WIDTH] > w_width){
                x = 0;
                y += (*Text_edges(b_type[i]->text))[DRAW_HEIGHT] + 5;
            }

            b_type[i]->state[INP_STATE_SELECTED] = i == p_type;
            Button_Display(b_type[i], x, y, NULL);
            b_type[i]->state[INP_STATE_SELECTED] = 0;
            x += (*Text_edges(b_type[i]->text))[DRAW_WIDTH] + 10;
        }

        // Display generate and view answer button
        Button_Display(b_generate, 0, (*Text_edges(b_type[i - 1]->text))[DRAW_EDGE_YMAX] + 50, NULL);
        Button_Display(b_view_answer, (*Text_edges(b_generate->text))[DRAW_EDGE_XMAX] + 10, (*Text_edges(b_generate->text))[DRAW_EDGE_YMIN], NULL);


        // Generate a problem, if necessary
        if(b_generate->state[INP_STATE_SELECTED]){
            MathProblem_GenProblem(&problem, p_type, p_difficulty);
            Text_SetUTF8(p_t, problem.question);
        }
        Text_DrawUTF8(
            p_t,
            NULL,
            TextConfig_Create(
                .x = w_width / 2 - (*Text_edges(p_t))[DRAW_WIDTH] / 2,
                .y = (*Text_edges(b_generate->text))[DRAW_EDGE_YMAX] + 50,
                .wrap = w_width - 20,
            ),
            NULL
        );

        // Check if user wants see the answer
        if(b_view_answer->state[INP_STATE_SELECTED]){
            MathProblem_GetProblem(&problem);
            Text_SetUTF8(answer->text, problem.answer);
            MathProblem_HideAnswer(&problem);
        }

        // Answer
        *Text_wrap(answer->text) = w_width - 20;
        TextInput_Display(
            answer,
            w_width / 2 - (*Text_edges(answer->text))[DRAW_WIDTH] / 2,
            (*Text_edges(p_t))[DRAW_EDGE_YMAX] + 5,
            NULL
        );

        Button_Display(
            b_submit,
            (*Text_edges(answer->text))[DRAW_EDGE_XMAX] + 2,
            (*Text_edges(answer->text))[DRAW_EDGE_YMIN],
            NULL
        );

        // Check if answer is correct
        if(b_submit->state[INP_STATE_SELECTED]){
            MathProblem_GetProblem(&problem);
            Text_SetUTF8(answer_t, strcmp(problem.answer, *Text_raw(answer->text))? "Incorreto": "Correto");
            MathProblem_HideAnswer(&problem);
        }
        if(*Text_raw(answer_t)){
            Text_DrawUTF8(
                answer_t,
                NULL,
                TextConfig_Create(
                    .x = (*Text_edges(answer->text))[DRAW_EDGE_XMIN] + (*Text_edges(answer->text))[DRAW_WIDTH] / 2 - (*Text_edges(answer_t))[DRAW_WIDTH] / 2,
                    .y = (*Text_edges(answer->text))[DRAW_EDGE_YMIN],
                    .rotation = -45,
                    .color = (!strcmp(*Text_raw(answer_t), "Incorreto"))?(SDL_Color){255, 0, 0, 255}: (SDL_Color){0, 255, 0, 255},
                ),
                NULL
            );
        }
        
        Game_Debug();
        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
}
