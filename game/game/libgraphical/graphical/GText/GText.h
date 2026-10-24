#ifndef GTEXT_H
#define GTEXT_H

#include"GCommons.h"
#include"GTexture.h"
#include"GTextTypedefs.h"
#include"GTextCache.h"
#include"GTextConfig.h"
#include"GTextChar.h"
#include"GTextCharRect.h"
#include"GTextCharFormat.h"

#include<utf8.h>

typedef enum DRAW_EDGES DRAW_EDGES;
#define DRAW_EDGE_AMT 30
enum DRAW_EDGES {
    DRAW_EDGE_XMAX,
    DRAW_EDGE_YMAX,
    DRAW_EDGE_XMIN,
    DRAW_EDGE_YMIN,

    DRAW_EDGE_MARGIN_XMAX,
    DRAW_EDGE_MARGIN_YMAX,
    DRAW_EDGE_MARGIN_XMIN,
    DRAW_EDGE_MARGIN_YMIN,

    DRAW_EDGE_BORDER_XMAX,
    DRAW_EDGE_BORDER_YMAX,
    DRAW_EDGE_BORDER_XMIN,
    DRAW_EDGE_BORDER_YMIN,

    DRAW_EDGE_PADDING_XMAX,
    DRAW_EDGE_PADDING_YMAX,
    DRAW_EDGE_PADDING_XMIN,
    DRAW_EDGE_PADDING_YMIN,

    DRAW_EDGE_TEXT_XMAX,
    DRAW_EDGE_TEXT_YMAX,
    DRAW_EDGE_TEXT_XMIN,
    DRAW_EDGE_TEXT_YMIN,

    DRAW_WIDTH,
    DRAW_MARGIN_WIDTH,
    DRAW_BORDER_WIDTH,
    DRAW_PADDING_WIDTH,
    DRAW_TEXT_WIDTH,

    DRAW_HEIGHT,
    DRAW_MARGIN_HEIGHT,
    DRAW_BORDER_HEIGHT,
    DRAW_PADDING_HEIGHT,
    DRAW_TEXT_HEIGHT,
};

/*
 * Text struct
 */
struct Text {
    TextCache *cache;
    SDL_Texture *cache_margin;
    SDL_Texture *cache_border;
    SDL_Texture *cache_padding;
    SDL_Texture *cache_text;
    SDL_Texture *cache_texture;

    char *raw;
    TextConfig config;
    TextCharRect *chrs_dest;
    int edges[DRAW_EDGE_AMT];
    int edges_client[DRAW_EDGE_AMT];

    int raw_changed;
    int config_changed;

    // Deslocamento aplicado ao desenhar o texto dentro da propria textura
    // (compensa coordenadas negativas). E medido por Text_MeasureUTF8 e
    // precisa viver no proprio Text: o cache de texturas reaproveita o valor
    // entre quadros, e varios Text podem existir ao mesmo tempo.
    int draw_x_init;
    int draw_y_init;
};
WRAP_FIELD(Text, cache->texture.renderer, renderer, SDL_Renderer*);
WRAP_FIELD(Text, cache, TextCache*);

WRAP_FIELD(Text, raw, char*);

/* TextConfig struct */
WRAP_FIELD(Text, config, TextConfig);

WRAP_FIELD(Text, config.padding, padding, Padding);
WRAP_FIELD(Text, config.padding.top, paddingTop, unsigned);
WRAP_FIELD(Text, config.padding.right, paddingRight, unsigned);
WRAP_FIELD(Text, config.padding.bottom, paddingBottom, unsigned);
WRAP_FIELD(Text, config.padding.left, paddingLeft, unsigned);
WRAP_FIELD(Text, config.padding.color, paddingColor, SDL_Color);
WRAP_FIELD(Text, cache_padding, paddingCache, SDL_Texture*);

WRAP_FIELD(Text, config.border, border, TextSpacing);
WRAP_FIELD(Text, config.border.top, borderTop, unsigned);
WRAP_FIELD(Text, config.border.right, borderRight, unsigned);
WRAP_FIELD(Text, config.border.bottom, borderBottom, unsigned);
WRAP_FIELD(Text, config.border.left, borderLeft, unsigned);
WRAP_FIELD(Text, config.border.color, borderColor, SDL_Color);
WRAP_FIELD(Text, cache_border, borderCache, SDL_Texture*);

WRAP_FIELD(Text, config.margin, margin, TextSpacing);
WRAP_FIELD(Text, config.margin.top, marginTop, unsigned);
WRAP_FIELD(Text, config.margin.right, marginRight, unsigned);
WRAP_FIELD(Text, config.margin.bottom, marginBottom, unsigned);
WRAP_FIELD(Text, config.margin.left, marginLeft, unsigned);
WRAP_FIELD(Text, config.margin.color, marginColor, SDL_Color);
WRAP_FIELD(Text, cache_margin, marginCache, SDL_Texture*);

WRAP_FIELD(Text, config.color, color, SDL_Color);
WRAP_FIELD(Text, config.color_bg, colorBackground, SDL_Color);
WRAP_FIELD(Text, config.lineskip, lineskip, unsigned);
WRAP_FIELD(Text, config.wrap, wrap, unsigned);
WRAP_FIELD(Text, config.rotation, rotation, double);
WRAP_FIELD(Text, config.flip, flip, SDL_RendererFlip);
//
WRAP_FIELD(Text, config.bx_width, boxWidthText, int);
WRAP_FIELD(Text, config.bx_height, boxHeightText, int);
//
WRAP_FIELD(Text, config.scale_width, scaleWidth, double);
WRAP_FIELD(Text, config.scale_height, scaleHeight, double);

WRAP_FIELD(Text, config.chr_dist, chrDist, int);
WRAP_FIELD(Text, config.chr_rotation, chrRotation, double);
WRAP_FIELD(Text, config.chr_fmt, chrFmt, TextCharFormat);
WRAP_FIELD(Text, config.chr_flip, chrFlip, SDL_RendererFlip);
//
WRAP_FIELD(Text, config.x, x, double);
WRAP_FIELD(Text, config.y, y, double);
//
WRAP_FIELD(Text, chrs_dest, chrsDest, TextCharRect*);
//
WRAP_FIELD(Text, cache_text, textCache, SDL_Texture*);
WRAP_FIELD(Text, cache_texture, cacheTexture, SDL_Texture*);

void Text_CalculateCenterEdgesClient(
    double cx, double cy,
    int *xmin, int *xmax,
    int *ymin, int *ymax,
    double t_sin, double t_cos
    );
int (*Text_edges(Text*t))[DRAW_EDGE_AMT];

static inline int (*Text_edgesBack(Text*t))[DRAW_EDGE_AMT]
{
    return &t->edges;
}

static inline void (**Text_fAlterText(Text*t))(Text*, TextChar*)
{
    return &t->config.func_alterText;
}

static inline void (**Text_fAlterPadding(Text*t))(Text*, Texture*)
{
    return &t->config.func_alterPadding;
}

static inline void (**Text_fAlterBorder(Text*t))(Text*, Texture*)
{
    return &t->config.func_alterBorder;
}

static inline void (**Text_fAlterMargin(Text*t))(Text*, Texture*)
{
    return &t->config.func_alterMargin;
}


//
// Properties
// 
int Text_getUTF8(Text*, size_t );

/* Quantidade de caracteres (codepoints) do texto. */
size_t Text_GetSizeUTF8(Text*t);
/* Tamanho do texto em bytes, incluindo o '\\0' final. */
size_t Text_GetLenUTF8(Text*t);
char* Text_GetUTF8(Text*t);

/* Copia str para o Text. NULL limpa o texto. */
void Text_SetUTF8(Text*t, char*);

void Text_UpdateConfig(Text*t, TextConfig*);

// Measures
void Text_MeasureMargin(Text*, int*, int*);
void Text_MeasureBorder(Text*, int*, int*);
void Text_MeasurePadding(Text*, int*, int*);
void Text_MeasureUTF8(Text*, int*x, int*y);

// Positions
void Text_PositionMargin(Text*, int*, int*);
void Text_PositionBorder(Text*, int*, int*);
void Text_PositionPadding(Text*, int*, int*);
void Text_PositionUTF8(Text*, int*, int*);

// Textures
#define TEXT_TEXTURE_PIXELFORMAT SDL_PIXELFORMAT_RGBA8888
#define TEXT_TEXTURE_ACCESS SDL_TEXTUREACCESS_TARGET
//
SDL_Texture* Text_GenTextureSpacing(Text*t, int w, int h, SDL_Color*c, void(*f_alterTexture)(Text*, Texture*));
SDL_Texture* Text_GenTextureMargin(Text*);
SDL_Texture* Text_GenTextureBorder(Text*);
SDL_Texture* Text_GenTexturePadding(Text*);
//
SDL_Texture* Text_GenTextureUTF8(Text*);

//
// Methods
//
/* Retorna NULL se o cache for NULL ou faltar memoria. */
Text* Text_Create_Core(TextCache*, TextConfig*, char*str);
#define TEXT_CREATE_VA_NUM_ARGS_IMPL(_0, _1, _2, _3, N, ...) N
#define TEXT_CREATE_VA_NUM_ARGS(...) TEXT_CREATE_VA_NUM_ARGS_IMPL(__VA_ARGS__,, 3, 2, 1, 0)
//
/* Identificador inexistente de proposito: o erro de compilacao mostra a mensagem. */
#define TEXT_CREATE_0 \
    Text_Create_requires_at_least_one_argument
#define TEXT_CREATE_1(arg1) \
    Text_Create_Core(arg1, NULL, NULL)
#define TEXT_CREATE_2(arg1, arg2) \
    Text_Create_Core(arg1, arg2, NULL)
#define TEXT_CREATE_3(arg1, arg2, arg3) \
    Text_Create_Core(arg1, arg2, arg3)
//
#define TEXT_CREATE_CAT(a, b) a##b
#define TEXT_CREATE_SELECT(N) TEXT_CREATE_CAT(TEXT_CREATE_, N)
#define Text_Create(...) \
    TEXT_CREATE_SELECT(TEXT_CREATE_VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

void Text_DrawUTF8(Text*, char*str, TextConfig*tc, int (*edges)[DRAW_EDGE_AMT]);

/* Libera o Text e deixa *t == NULL. Aceita NULL. */
void Text_Destroy(Text**);

#endif
