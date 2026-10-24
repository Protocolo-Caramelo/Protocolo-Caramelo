#ifdef COMPILATION_DEVQUOTE

#include"GTest.h"

void TextCache_Test(char *font_path, unsigned ptsize)
{
    TextCache *test;
    //
    SDL_Renderer *r;
    SDL_Window *w;
    SDL_Event e;
    SDL_Rect dest;
    //
    int c;
    TextCharFormat fmt;

    if(graphical_init() != 0){

        exit(1);

    }
    w = SDL_CreateWindow(
        "Testing TextFontAtlas struct...",
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

    test = TextCache_Create(r, font_path, ptsize);

    if(!test){

        exit(1);

    }
    c = L' ';
    fmt = CHAR_FMT_REGULAR;
    
    while(1){
        SDL_SetRenderDrawColor(r, 255, 0, 255, 255);
        SDL_RenderClear(r);

        if(c <= L'ÿ'){
            TextCache_GetGlyphSize(test, c, fmt, NULL, NULL);
            ++c;
            ++fmt;
            fmt %= (CHAR_FMT_BOLD | CHAR_FMT_ITALIC | CHAR_FMT_UNDERLINE | CHAR_FMT_STRIKETHROUGH);
        }
        dest.x = dest.y = 0;
        SDL_QueryTexture(
            *TextCache_texture(test),
            NULL,
            NULL,
            &dest.w,
            &dest.h
        );
        // printf("%i %i\n", dest.w, dest.h);
        SDL_RenderCopy(
            r,
            *TextCache_texture(test),
            NULL,
            &dest
        );


        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    TextCache_Destroy(&test);
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
        SDL_Delay(16);
        // SDL_Delay(500);
    }
}

#endif
