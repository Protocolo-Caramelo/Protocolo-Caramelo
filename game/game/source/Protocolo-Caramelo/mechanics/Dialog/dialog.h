#ifndef DIALOG_H
#define DIALOG_H

/*
 * Sistema de dialogos (estilo Undertale, com letras em onda e rosto de gelatina).
 *
 * Uso minimo:
 *
 *     static const DialogLine cena[] = {
 *         DIALOG_LINE(C_FELIPE, CS_HAPPY, "Fala Pedroca, como voce ta?"),
 *         DIALOG_LINE(C_PEDRO,  CS_SAD,   "Eu estou bem... eu acho."),
 *     };
 *     Dialog_Play(cena, NULL);        // comeca o dialogo
 *
 * e, uma vez por quadro / evento (ja ligado em play.c, room.c e monitor.c):
 *
 *     if(Dialog_HandleInput(&e)) ...  // 1 = o dialogo consumiu o evento
 *     Dialog_Render();                // desenha por cima do jogo
 *
 * Com opcoes no fim e historico no app Bate-boca (veja DialogScene abaixo):
 *
 *     static const DialogChoice opcoes[] = {
 *         { .label = "Aceitar" },
 *         { .label = "Recusar", .on_select = Recusou },
 *     };
 *     static const DialogScene cena = {
 *         .archive_title = "Felipe",              // guarda no Bate-boca
 *         DIALOG_SCENE_LINES(falas),
 *         DIALOG_SCENE_CHOICES(opcoes),
 *     };
 *     Dialog_PlayScene(&cena);
 *
 * Documentacao completa: mechanics/Dialog/README.md
 */

#include<SDL2/SDL.h>
#include"dialog_chars.h"
#include"dialog_style.h"

/* Uma fala. Os campos nao citados ficam 0/NULL (= padrao). */
typedef struct DialogLine {
    Character          speaker;   /* quem fala: C_PEDRO, C_FELIPE... */
    Character_State    state;     /* expressao do rosto: CS_HAPPY, CS_ANGRY... */
    const char        *text;      /* UTF-8; '\n' forca quebra de linha */
    const DialogStyle *style;     /* NULL = dialog_styleDefault */
    void (*on_show)(void);        /* opcional: chamada quando a fala aparece */
} DialogLine;

/* Quem e o jogador: as opcoes escolhidas aparecem no Bate-boca como falas dele */
#define DIALOG_PLAYER C_PEDRO

/* Atalhos para escrever falas em vetores estaticos */
#define DIALOG_LINE(who, mood, txt) \
    { .speaker = (who), .state = (mood), .text = (txt) }
#define DIALOG_LINE_STYLED(who, mood, txt, st) \
    { .speaker = (who), .state = (mood), .text = (txt), .style = (st) }
#define DIALOG_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/* ------------------------------------------------------------------ */
/* Cenas: falas + opcoes no fim + historico                            */
/* ------------------------------------------------------------------ */
typedef struct DialogScene DialogScene;

/* Uma opcao do menu mostrado no fim da cena. */
typedef struct DialogChoice {
    const char *label;               /* texto da opcao (curto: cabe em uma linha) */
    void (*on_select)(void);         /* opcional: roda quando o jogador escolhe */
    const DialogScene *next;         /* opcional: cena que continua depois da escolha */
} DialogChoice;

/*
 * Uma cena. So `lines`/`count` sao obrigatorios; o resto fica 0/NULL.
 * Tudo (a cena, os vetores e os textos) precisa continuar vivo: use `static const`.
 */
struct DialogScene {
    const char *archive_title;       /* != NULL: guarda as falas (e a opcao escolhida)
                                        na conversa com esse titulo do Bate-boca */
    const DialogLine *lines;
    size_t count;
    const DialogChoice *choices;     /* menu no fim da cena (NULL/0 = sem menu) */
    size_t choice_count;
    void (*on_finish)(void);         /* ao terminar a cena (depois da escolha, se houver) */
};

#define DIALOG_SCENE_LINES(array) \
    .lines = (array), .count = DIALOG_COUNT(array)
#define DIALOG_SCENE_CHOICES(array) \
    .choices = (array), .choice_count = DIALOG_COUNT(array)

/* ------------------------------------------------------------------ */
/* Ciclo de vida                                                       */
/* ------------------------------------------------------------------ */
void Dialog_Init(void);   /* ja chamada por GameMechanics_Init */
void Dialog_Quit(void);   /* ja chamada por GameMechanics_Quit */

/* ------------------------------------------------------------------ */
/* Controle                                                            */
/* ------------------------------------------------------------------ */

/*
 * Comeca um dialogo. O vetor `lines` NAO e copiado: precisa continuar vivo
 * (use `static const`). Se ja houver um dialogo ativo, ele e substituido
 * (sem chamar o on_finish dele). on_finish e opcional e e chamado ao fim da
 * ultima fala, ja com o sistema livre para iniciar outro dialogo.
 */
void Dialog_Start(const DialogLine *lines, size_t count, void (*on_finish)(void));
#define Dialog_Play(lines, on_finish) \
    Dialog_Start((lines), DIALOG_COUNT(lines), (on_finish))

/*
 * Comeca uma cena (veja DialogScene). Se havia um dialogo ativo, ele e
 * substituido sem chamar o on_finish dele. Ordem ao fim de uma cena com
 * opcoes: on_select da opcao -> on_finish da cena -> cena `next` da opcao.
 */
void Dialog_PlayScene(const DialogScene *scene);

int  Dialog_IsActive(void);

/* 1 enquanto o menu de opcoes esta na tela esperando o jogador. */
int  Dialog_IsChoosing(void);

/*
 * Se a fala ainda esta aparecendo, mostra tudo de uma vez; senao, passa para a
 * proxima (ou, com o menu aberto, confirma a opcao selecionada).
 */
void Dialog_Next(void);

/* Encerra o dialogo na hora, sem chamar o on_finish. */
void Dialog_Abort(void);

/*
 * Chamada a cada letra (nao-espaco) que comeca a aparecer; util para sons de
 * voz ("blips"). `index` e a posicao da letra no texto original. NULL desliga.
 */
void Dialog_SetCharCallback(void (*fn)(const DialogLine *line, size_t index));

/* ------------------------------------------------------------------ */
/* Laco do jogo                                                        */
/* ------------------------------------------------------------------ */

/*
 * Repasse cada SDL_Event. Retorna 1 se o dialogo consumiu o evento (o resto
 * do jogo deve ignora-lo) e 0 se nao. Avancar: Z, Enter, Espaco ou clique
 * esquerdo. Com o menu aberto: cima/baixo (ou W/S) mudam a selecao, o mouse
 * seleciona ao passar por cima, e Z/Enter/Espaco/clique confirmam.
 * SDL_QUIT e ESC nunca sao consumidos.
 */
int  Dialog_HandleInput(SDL_Event *e);

/* Atualiza e desenha o dialogo (nao faz nada se nao houver dialogo ativo). */
void Dialog_Render(void);

/*
 * Estilo da fala atual, ou NULL. Feita para os decoradores (alterBorder,
 * alterPadding...) que recebem so o Text* e precisam ler cores do estilo.
 */
const DialogStyle *Dialog_CurrentStyle(void);

#endif
