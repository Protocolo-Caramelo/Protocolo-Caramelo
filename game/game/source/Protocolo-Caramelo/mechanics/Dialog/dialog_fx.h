#ifndef DIALOG_FX_H
#define DIALOG_FX_H

/*
 * dialog_fx: matematica pura dos efeitos de dialogo.
 *
 * Este modulo NAO depende de SDL nem da libgraphical: so recebe numeros e
 * devolve numeros. Por isso ele pode ser testado isoladamente
 * (veja tests/test_dialog_fx.c) e e facil de ler e de ajustar.
 *
 * Unidades: tempos em milissegundos; distancias em "pixels virtuais" (a
 * resolucao de referencia do jogo, GCORE_WINDOW_W x GCORE_WINDOW_H). Quem
 * desenha multiplica pela escala da janela.
 */

#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Easing: recebem t em [0, 1] e devolvem o progresso suavizado        */
/* ------------------------------------------------------------------ */
typedef double (*DialogEaseFn)(double t);

double DialogEase_Linear(double t);
double DialogEase_OutQuad(double t);
double DialogEase_OutCubic(double t);   /* padrao: sobe rapido e assenta devagar */
double DialogEase_OutBack(double t);    /* passa um pouco do alvo e volta (quicada) */
double DialogEase_InOutSine(double t);

/* ------------------------------------------------------------------ */
/* Estilos dos efeitos (somente dados)                                 */
/* ------------------------------------------------------------------ */

/* Como cada letra aparece: sobe de baixo para cima enquanto ganha opacidade */
typedef struct DialogRevealStyle {
    double char_interval_ms;  /* tempo entre o inicio de duas letras seguidas */
    double char_duration_ms;  /* quanto dura a animacao de UMA letra */
    double rise_px;           /* de quantos px abaixo da posicao final a letra parte */
    double pause_comma_ms;    /* pausa extra apos  , ; :  (so quando seguidos de espaco/fim) */
    double pause_stop_ms;     /* pausa extra apos  . ! ? ...  (idem) */
    DialogEaseFn ease;        /* curva da subida; NULL = DialogEase_OutCubic */
} DialogRevealStyle;

/* Ondulacao continua de TODAS as letras (inclusive as que ainda nao apareceram) */
typedef struct DialogWaveStyle {
    double amplitude_px;      /* altura da onda (0 desliga) */
    double frequency_hz;      /* ciclos por segundo */
    double phase_per_char;    /* defasagem entre letras vizinhas, em radianos */
} DialogWaveStyle;

/* Efeito "gelatina" no rosto enquanto o personagem fala */
typedef struct DialogJellyStyle {
    double squash;            /* amplitude de achatar/esticar (0.05 = 5%) (0 desliga) */
    double sway_px;           /* balanco lateral do topo da imagem, em px */
    double frequency_hz;      /* velocidade da tremida */
    double sway_phase;        /* defasagem do balanco ao longo da altura (radianos) */
    double ramp_in_ms;        /* tempo para a gelatina "ligar" quando comeca a falar */
    double ramp_out_ms;       /* tempo para ela assentar quando para de falar */
    int    strips;            /* em quantas faixas horizontais a imagem e fatiada */
} DialogJellyStyle;

/* ------------------------------------------------------------------ */
/* Cronograma de revelacao                                             */
/* ------------------------------------------------------------------ */

/*
 * Conta os caracteres (codepoints UTF-8) de uma string.
 */
size_t DialogFx_CountChars(const char *utf8);

/*
 * Calcula quando cada caractere comeca a aparecer.
 *
 *  utf8      texto original (UTF-8)
 *  style     estilo de revelacao
 *  start_ms  vetor de saida, um valor por caractere (inclusive espacos e '\n')
 *  cap       capacidade de start_ms
 *  total_ms  saida: quando a ultima letra termina de aparecer (pode ser NULL)
 *
 * Retorna quantos caracteres foram escritos em start_ms.
 */
size_t DialogFx_BuildSchedule(
    const char *utf8,
    const DialogRevealStyle *style,
    double *start_ms, size_t cap,
    double *total_ms
);

/* Progresso (0..1, ja suavizado pela curva) de uma letra no instante elapsed_ms. */
double DialogFx_Reveal(double elapsed_ms, double start_ms, double duration_ms, DialogEaseFn ease);

/* Opacidade (0..1) da letra: aparece mais rapido que a subida, para nao "piscar". */
double DialogFx_Alpha(double elapsed_ms, double start_ms, double duration_ms);

/* ------------------------------------------------------------------ */
/* Ondulacao                                                           */
/* ------------------------------------------------------------------ */

/* Deslocamento vertical (px, entre -amplitude e +amplitude) da letra de indice char_idx. */
double DialogFx_Wave(const DialogWaveStyle *wave, double time_ms, size_t char_idx);

/* ------------------------------------------------------------------ */
/* Gelatina                                                            */
/* ------------------------------------------------------------------ */

/*
 * Devolve a nova "energia" (0..1) da gelatina. Sobe rapido enquanto o
 * personagem fala e decai suavemente quando ele para.
 */
double DialogFx_JellyEnergy(const DialogJellyStyle *j, double energy, int speaking, double dt_ms);

/* Fatores de escala horizontal/vertical da imagem inteira (preservam o "volume"). */
void DialogFx_JellyScale(const DialogJellyStyle *j, double energy, double time_ms, double *sx, double *sy);

/*
 * Deslocamento horizontal (px) de uma faixa da imagem.
 * y_frac: 0 = topo da imagem, 1 = base. A base fica parada; o topo balanca mais.
 */
double DialogFx_JellySway(const DialogJellyStyle *j, double energy, double time_ms, double y_frac);

/* ------------------------------------------------------------------ */
/* Quebra de linha por palavras                                        */
/* ------------------------------------------------------------------ */

/* Largura (px) de um caractere; ctx e repassado sem alteracao. */
typedef int (*DialogGlyphWidthFn)(void *ctx, int codepoint);

/*
 * Devolve uma COPIA (malloc; libere com free) do texto em que alguns espacos
 * foram trocados por '\n' para que nenhuma linha passe de max_width.
 * O tamanho em bytes e o numero de caracteres nao mudam.
 * chr_dist: espaco extra entre caracteres (igual ao chr_dist do Text).
 */
char *DialogFx_WordWrap(
    const char *utf8, int max_width, int chr_dist,
    DialogGlyphWidthFn width_of, void *ctx
);

/* Decodifica um codepoint e avanca *p (entrada invalida vira U+FFFD, 1 byte). */
int DialogFx_NextCodepoint(const char **p);

/* ------------------------------------------------------------------ */
/* Menu de opcoes                                                      */
/* ------------------------------------------------------------------ */

/*
 * Move a selecao de um menu com `count` itens. step = +1 (para baixo) ou -1
 * (para cima); passar do fim volta ao comeco e vice-versa.
 * count <= 0 devolve 0.
 */
int DialogFx_MenuStep(int current, int count, int step);

/*
 * Qual linha esta na altura y? As linhas comecam em y = top e tem row_h de
 * altura cada. Devolve -1 se y estiver fora de todas.
 */
int DialogFx_MenuRowAt(int y, int top, int row_h, int count);

/*
 * Instante (ms, desde a abertura do menu) em que a opcao `index` comeca a
 * aparecer: index * stagger_ms.
 */
double DialogFx_MenuStart(int index, double stagger_ms);


#endif
