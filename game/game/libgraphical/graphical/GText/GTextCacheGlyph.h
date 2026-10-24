#ifndef GTEXTCACHEGLYPH_H
#define GTEXTCACHEGLYPH_H

#include"GCommons.h"
#include"GTextTypedefs.h"
#include"GTextCharFormat.h"

#include<SDL2/SDL_rect.h>
#include<uthash.h>

typedef SDL_Rect Glyph;

struct TextCacheGlyphKey{
    int c;
    TextCharFormat fmt;
};

struct TextCacheGlyph {
    TextCGK key;
    Glyph c_glyph;
    UT_hash_handle hh;
};
WRAP_FIELD(TextCG, key, TextCGK);
WRAP_FIELD(TextCG, c_glyph, cGlyph, Glyph);

//
// Properties
//
Glyph *TextCG_GetGlyph(TextCG*, int c, TextCharFormat fmt);
Glyph *TextCG_GetGlyphEnd(TextCG*);
size_t TextCG_GetGlyphAmt(TextCG*);

//
// Methods
//
void TextCG_AddGlyph(TextCG**, int c, TextCharFormat fmt, Glyph);
/* Libera todos os glifos e deixa *g == NULL. */
void TextCG_Destroy(TextCG**);

#endif
