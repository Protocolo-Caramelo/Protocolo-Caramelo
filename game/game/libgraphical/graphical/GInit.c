#include"GInit.h"

int graphical_init(void)
{
    const int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        GRAPHICAL_LOG_ERROR("Error while initializing sdl video: %s", SDL_GetError());
        return -1;
    }

    if((IMG_Init(img_flags) & img_flags) != img_flags){
        GRAPHICAL_LOG_ERROR("Error while initializing images: %s", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    if(TTF_Init() != 0){
        GRAPHICAL_LOG_ERROR("Error while initializing ttf library: %s", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    return 0;
}

void graphical_quit(void)
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
