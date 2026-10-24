#if defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
#include"GTest.h"

void Text_Test1(char*path, unsigned ptsize, char*str)
{
    Text *t;
    TextCache *cache;
    //
    double fps;
    unsigned time;
    int fps_delay;
    //
    double chrRotation;
    int chrRotation_delay;
    int chrFmt;
    //
    double rotation;
    int rotation_delay;
    //
    SDL_Window *w;
    SDL_Renderer *r;
    SDL_Event e;

    if(graphical_init() != 0){

        exit(1);

    }


    w = SDL_CreateWindow(
        "Testing Text struct...",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1028,
        512,
        SDL_WINDOW_RESIZABLE
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    cache = TextCache_Create(r, path, ptsize);

    if(!cache){

        exit(1);

    }
    t = Text_Create(cache);
    *Text_color(t) = (SDL_Color){0, 0, 255, 255};
    Text_SetUTF8(t, str);

    *Text_wrap(t) = 1024;
    *Text_scaleWidth(t) = *Text_scaleHeight(t) = 1 / 2.0;
    // *Text_x(t) = *Text_y(t) = 100;
    //
    fps = 0;
    fps_delay = SDL_GetTicks();
    time = SDL_GetTicks();
    //
    chrRotation_delay = 0;
    chrRotation = rotation_delay = 0;
    chrFmt = CHAR_FMT_REGULAR;
    while(1){
        if((SDL_GetTicks() - fps_delay) >= FPS_DELAY){
            printf("FPS: %.2lf\n", fps);
            fps_delay = SDL_GetTicks();
        }
        // printf("%lf\n", chrRotation);

        //
        // Char rotation
        //
        if((SDL_GetTicks() - chrRotation_delay) >= CHR_ROTATION_DELAY){
            chrRotation_delay = SDL_GetTicks();
            chrRotation = 0;
            ++chrFmt;
            chrFmt %= (CHAR_FMT_REGULAR | CHAR_FMT_BOLD | CHAR_FMT_ITALIC | CHAR_FMT_UNDERLINE | CHAR_FMT_STRIKETHROUGH);
        };

        //
        // Text rotation
        //
        if(SDL_GetTicks() - rotation_delay >= TXT_ROTATION_DELAY){
            rotation_delay = SDL_GetTicks();
            rotation = 0;
        }

        //
        //
        //
        SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
        SDL_RenderClear(r);

        // 
        // Draw text
        //
        //*
        Text_DrawUTF8(
            t,
            str,
            TextConfig_Create(
                .rotation = rotation,
                .chr_rotation = chrRotation,
                .chr_fmt = chrFmt,
            ),
            NULL
        );
        // */
        /*
        int atlas_w, atlas_h;
        SDL_Rect atlas_d;
        TextCache_GetAtlasSize(*Text_cache(t), &atlas_w, &atlas_h);
        printf("%i %i\n", atlas_w, atlas_h);
        atlas_d = (SDL_Rect){
            .x = 0,
            .y = 0,
            .w = atlas_w,
            .h = atlas_h
        };
        SDL_RenderCopy(
            r,
            *TextCache_texture(*Text_cache(t)),
            NULL,
            &atlas_d
        );
        // */

        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    Text_Destroy(&t);
                    TextCache_Destroy(&cache);
                    SDL_DestroyWindow(w);
                    SDL_DestroyRenderer(r);
                    SDL_Quit();
                    exit(0);
                    break;

                default:
                    break;
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(GTEST_DELAY);

        fps = 1000.0 / (SDL_GetTicks() - time);
        chrRotation += (CHR_ROTATION_MAX / CHR_ROTATION_DELAY) * (SDL_GetTicks() - time);
        rotation += (TXT_ROTATION_MAX / TXT_ROTATION_DELAY) * (SDL_GetTicks() - time);
        time = SDL_GetTicks();
    }
}
#endif
