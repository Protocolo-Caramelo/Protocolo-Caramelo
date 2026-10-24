#ifdef COMPILATION_DEVQUOTE

#include"GTest.h"

void TextCG_Test()
{
    TextCG *test;
    TextCG *bin, *tmp;
    SDL_Rect dest;
    Glyph *g_end;
    int c;

    test = NULL;
    for(c = 'A'; c <= 'Z'; ++c){
        dest = (SDL_Rect){
            .x = c,
            .y = c
        };
        TextCG_AddGlyph(&test, c, CHAR_FMT_BOLD, dest);
    }

    HASH_ITER(hh, test, bin, tmp) {
        printf("(%i %i): (%i %i)\n", TextCG_key(bin)->c, TextCG_key(bin)->fmt, TextCG_cGlyph(bin)->x, TextCG_cGlyph(bin)->y);
    }
    g_end = TextCG_GetGlyphEnd(test);

    printf("Glyph last: (%i %i)\n", g_end->x, g_end->y);
    printf("Glyph amount: %li\n", TextCG_GetGlyphAmt(test));

    TextCG_Destroy(&test);
}

#endif
