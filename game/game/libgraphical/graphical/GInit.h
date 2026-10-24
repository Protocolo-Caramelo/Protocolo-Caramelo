#ifndef GINIT_H
#define GINIT_H

#include"GCommons.h"
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

/*
 * Inicializa SDL (video), SDL_image (PNG e JPG) e SDL_ttf.
 *
 * Retorno: 0 em caso de sucesso, -1 em caso de falha. Quando falha, tudo o que
 * ja havia sido inicializado e desfeito e o motivo e registrado via log.
 */
int graphical_init(void);

/*
 * Encerra SDL_ttf, SDL_image e SDL. Deve ser chamada apos destruir todos os
 * objetos (Text, TextCache, Texture, ...) criados pela biblioteca.
 */
void graphical_quit(void);

#endif
