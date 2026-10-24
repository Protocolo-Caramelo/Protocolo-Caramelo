#ifdef COMPILATION_DEVQUOTE
#include"GTest.h"

void SimpleWindow()
{
    SDL_Window*w;
    SDL_Renderer*r;
    SDL_Event e;

    w = SDL_CreateWindow(
        "Testing memory ram usaage",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        512,
        SDL_WINDOW_RESIZABLE
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    while(1){
        SDL_SetRenderDrawColor(r, 255, 0, 255, 255);
        SDL_RenderClear(r);

        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
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
    }
}

#endif
