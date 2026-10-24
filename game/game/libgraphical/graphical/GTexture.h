#ifndef GTEXTURE_H
#define GTEXTURE_H

#include"GCommons.h"
#include<SDL2/SDL_render.h>

typedef struct Texture Texture;

/*
 * Texture struct
 *
 * Agrupa uma SDL_Texture e o SDL_Renderer ao qual ela pertence.
 */
struct Texture {
    SDL_Texture *raw;
    SDL_Renderer *renderer;
};
WRAP_FIELD(Texture, raw, SDL_Texture*);
WRAP_FIELD(Texture, renderer, SDL_Renderer*);

//
// Methods
//
/*
 * Limpa a textura com branco totalmente transparente (255, 255, 255, 0).
 * O alvo de renderizacao e a cor de desenho anteriores do renderer sao restaurados.
 */
void Texture_Clear(Texture*t);

#endif
