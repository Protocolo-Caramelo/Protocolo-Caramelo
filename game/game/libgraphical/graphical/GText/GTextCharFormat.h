#ifndef GTEXTCHARFORMAT_H
#define GTEXTCHARFORMAT_H

#include<SDL2/SDL_ttf.h>

enum TextCharFormat {
    CHAR_FMT_INVALID        = -1,
    CHAR_FMT_REGULAR        = TTF_STYLE_NORMAL,
    CHAR_FMT_BOLD           = TTF_STYLE_BOLD,
    CHAR_FMT_ITALIC         = TTF_STYLE_ITALIC,
    CHAR_FMT_UNDERLINE      = TTF_STYLE_UNDERLINE,
    CHAR_FMT_STRIKETHROUGH  = TTF_STYLE_STRIKETHROUGH
};

/* Todas as flags de estilo combinadas. */
#define CHAR_FMT_MASK \
    (CHAR_FMT_BOLD | CHAR_FMT_ITALIC | CHAR_FMT_UNDERLINE | CHAR_FMT_STRIKETHROUGH)

/*
 * Um formato e valido quando e >= 0 e usa apenas bits de estilo conhecidos
 * (CHAR_FMT_REGULAR == 0 tambem e valido). Valores como CHAR_FMT_INVALID (-1)
 * ou INT_MAX sao rejeitados.
 */
#define CHAR_FMT_AVAILABLE(fmt) \
    (((int)(fmt)) >= 0 && (((int)(fmt)) & ~CHAR_FMT_MASK) == 0)

/* Nome antigo (com erro de digitacao), mantido por compatibilidade. */
#define CHAR_FMT_AVAIBLE(fmt) CHAR_FMT_AVAILABLE(fmt)

#endif
