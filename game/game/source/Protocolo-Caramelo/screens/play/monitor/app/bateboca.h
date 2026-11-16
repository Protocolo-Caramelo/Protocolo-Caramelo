#ifndef APP_BATEBOCA_H
#define APP_BATEBOCA_H

#include"app_communs.h"

/*
 * Bate-boca: o app de conversas do monitor.
 *
 * Mostra o historico das cenas de dialogo que o jogo guarda (DialogScene com
 * `archive_title`, veja mechanics/Dialog/). A esquerda ficam as conversas (a mais
 * recente primeiro, com um ponto laranja quando ha mensagens novas); ao clicar
 * em uma, a direita mostra as mensagens como balaoes: as falas dos outros
 * personagens na esquerda e as do Pedro/opcoes escolhidas na direita. A roda do
 * mouse rola a lista e as mensagens.
 *
 * O app so LE o DialogLog: quem grava e o motor de dialogo.
 * Registrado em monitor_apps.c atraves destas quatro funcoes.
 */
void AppBateBoca_Init(void);
void AppBateBoca_Quit(void);
void AppBateBoca_Draw(const SDL_Rect *body);
void AppBateBoca_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside);

#endif
