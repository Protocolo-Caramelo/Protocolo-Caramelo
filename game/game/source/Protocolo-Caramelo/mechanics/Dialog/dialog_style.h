#ifndef DIALOG_STYLE_H
#define DIALOG_STYLE_H

/*
 * DialogStyle: tudo o que muda a APARENCIA e o COMPORTAMENTO visual de um
 * dialogo. Uma fala (DialogLine) aponta para um estilo; se nao apontar, usa
 * dialog_styleDefault.
 *
 * Todas as medidas em "pixels virtuais": a resolucao de referencia do jogo
 * (GCORE_WINDOW_W x GCORE_WINDOW_H). O sistema multiplica pela escala da
 * janela, entao o dialogo se adapta sozinho quando a janela e redimensionada.
 */

#include <graphical.h>
#include "game/globals/game_cache.h"   /* GameFont */
#include "dialog_fx.h"

/* ------------------------------------------------------------------ */
/* Ganchos de personalizacao                                           */
/* ------------------------------------------------------------------ */

/* Informacoes de uma letra, entregues ao gancho DialogStyle.alter_char */
typedef struct DialogCharInfo {
    size_t index;      /* posicao da letra no texto (nao conta '\n') */
    double time_ms;    /* tempo desde o inicio desta fala */
    double reveal;     /* progresso da aparicao: 0 = escondida, 1 = completa */
    double ui_scale;   /* escala da janela: multiplique seus "px virtuais" por ela */
    int    option;     /* so no menu de opcoes: indice da opcao dona da letra (senao -1) */
    int    selected;   /* so no menu de opcoes: 1 se essa opcao esta selecionada */
} DialogCharInfo;

/*
 * Gancho por letra, chamado DEPOIS da aparicao e da ondulacao.
 * Pode mexer em c->x, c->y, c->color, c->rotation... (e o func_alterText da
 * libgraphical, com informacao extra). Deixe NULL para nao usar.
 */
typedef void (*DialogCharFn)(TextChar *c, const DialogCharInfo *info);

/* ------------------------------------------------------------------ */
/* Partes do estilo                                                    */
/* ------------------------------------------------------------------ */
typedef enum DialogFaceSide {
    DIALOG_FACE_RIGHT,   /* rosto na direita, texto na esquerda (padrao) */
    DIALOG_FACE_LEFT,    /* rosto na esquerda, texto na direita (estilo Undertale) */
    DIALOG_FACE_NONE     /* sem rosto: so o texto */
} DialogFaceSide;

/* A caixa: margem -> borda -> preenchimento -> area do texto */
typedef struct DialogBoxStyle {
    int screen_margin;        /* distancia da caixa ate as laterais da area de jogo */
    int bottom_gap;           /* distancia da caixa ate a parte de baixo */
    int lines;                /* quantas linhas de texto cabem na caixa */

    Margin  margin;           /* lados em px virtuais + .color */
    Border  border;
    Padding padding;
    SDL_Color text_bg;        /* fundo da area do texto ({0,0,0,0} = transparente) */
    SDL_Color accent;         /* cor extra usada pelos decoradores abaixo */
    int text_slack;           /* folga vertical extra (px) para efeitos por letra */

    /* func_alter* da libgraphical: desenham sobre a textura da camada.
     * NULL = camada lisa, so com a cor definida acima. */
    void (*alterMargin)(Text*, Texture*);
    void (*alterBorder)(Text*, Texture*);
    void (*alterPadding)(Text*, Texture*);
} DialogBoxStyle;

typedef struct DialogTextStyle {
    GameFont font;            /* qual fonte do cache (GFONT_REGULAR, GFONT_TITLE...) */
    double scale;             /* escala do glifo (as fontes do cache tem 100pt) */
    SDL_Color color;          /* cor das letras (alpha 255 = opaco) */
    TextCharFormat fmt;       /* CHAR_FMT_REGULAR | CHAR_FMT_BOLD ... */
    int chr_dist;             /* espaco extra entre letras (px virtuais) */
    unsigned lineskip;        /* distancia entre linhas; 0 = a da fonte */
} DialogTextStyle;

typedef struct DialogFaceStyle {
    DialogFaceSide side;
    int inset;                /* folga entre a imagem e a borda interna da caixa */
    int gap;                  /* espaco entre a imagem e o texto */
    DialogJellyStyle jelly;
} DialogFaceStyle;

/* Setinha que pisca quando a fala terminou e espera o jogador */
typedef struct DialogPromptStyle {
    int size;                 /* 0 = sem setinha */
    SDL_Color color;
    double bounce_px;
} DialogPromptStyle;

/*
 * O menu de opcoes que aparece ao fim de uma cena com `choices`.
 * Ele reaproveita a caixa (`box`) e a fonte (`text`) do estilo da ULTIMA fala,
 * entao margem, borda, preenchimento e os func_alter* da caixa valem aqui tambem.
 */
typedef enum DialogChoiceAlign {
    DIALOG_CHOICE_ALIGN_LEFT,    /* canto esquerdo da caixa de fala (padrao) */
    DIALOG_CHOICE_ALIGN_RIGHT    /* canto direito */
} DialogChoiceAlign;

typedef struct DialogChoiceStyle {
    DialogChoiceAlign align;
    int gap;                     /* distancia entre o menu e a caixa de fala */
    int min_width;               /* largura minima do menu (px virtuais) */
    int row_gap;                 /* espaco extra entre uma opcao e a seguinte */

    SDL_Color idle_color;        /* cor das opcoes nao selecionadas */
    SDL_Color selected_color;    /* cor da opcao selecionada */

    int marker_size;             /* tamanho da setinha ao lado da selecionada (0 = sem) */
    int marker_gap;              /* espaco entre a setinha e o texto */
    SDL_Color marker_color;
    double marker_bounce_px;     /* o quanto a setinha vai e volta na horizontal */

    /* So a opcao selecionada ondula (as outras ficam paradas) */
    DialogWaveStyle selected_wave;

    /* Abertura: cada opcao sobe de baixo para cima, uma depois da outra */
    double delay_ms;             /* espera depois do fim da fala, antes de abrir */
    double stagger_ms;           /* atraso entre uma opcao e a proxima */
    double duration_ms;          /* duracao da subida de cada opcao */
    double rise_px;

    /* Gancho por letra do menu (como DialogStyle.alter_char). NULL = nenhum. */
    void (*alter_char)(TextChar *c, const DialogCharInfo *info);
} DialogChoiceStyle;

typedef struct DialogStyle {
    DialogBoxStyle    box;
    DialogTextStyle   text;
    DialogRevealStyle reveal;   /* como cada letra aparece */
    DialogWaveStyle   wave;     /* ondulacao continua das letras */
    DialogFaceStyle   face;     /* posicao e gelatina do rosto */
    DialogPromptStyle prompt;
    DialogChoiceStyle choice;   /* menu de opcoes (fim da cena) */
    DialogCharFn      alter_char;
} DialogStyle;

/* ------------------------------------------------------------------ */
/* Estilos prontos                                                     */
/* ------------------------------------------------------------------ */

/*
 * Estilos globais, preenchidos por DialogStyles_Init() (chamada por Dialog_Init).
 * Use o endereco deles em uma DialogLine:  .style = &dialog_styleShout
 */
extern DialogStyle dialog_styleDefault;   /* fala normal */
extern DialogStyle dialog_styleWhisper;   /* devagar, baixinho, mais apagado */
extern DialogStyle dialog_styleShout;     /* rapido, tremendo, vermelho */

/* Devolve uma COPIA do estilo padrao para voce alterar e guardar. */
DialogStyle DialogStyle_Make(void);

void DialogStyles_Init(void);

/* ------------------------------------------------------------------ */
/* Decoradores prontos (para os campos alter* e alter_char)            */
/* ------------------------------------------------------------------ */

/* Preenchimento: degrade vertical de padding.color (topo) ate accent (base). */
void DialogDecor_PaddingGradient(Text *t, Texture *tex);
/* Borda: pulsa entre border.color e accent. */
void DialogDecor_BorderPulse(Text *t, Texture *tex);

/* Letras: tremida (gritos) e arco-iris. */
void DialogChar_Shake(TextChar *c, const DialogCharInfo *info);
void DialogChar_Rainbow(TextChar *c, const DialogCharInfo *info);

#endif
