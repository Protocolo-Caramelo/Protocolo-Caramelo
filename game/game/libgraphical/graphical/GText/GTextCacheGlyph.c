#include"GTextCacheGlyph.h"

//
// Properties
//
Glyph *TextCG_GetGlyph(TextCG*g, int c, TextCharFormat fmt)
{
    TextCG *out;
    TextCGK key;

    // memset garante bytes de padding iguais, ja que o hash e feito sobre bytes
    memset(&key, 0, sizeof(key));
    key.c = c;
    key.fmt = fmt;

    HASH_FIND(hh, g, &key, sizeof(key), out);
    return (out)? TextCG_cGlyph(out): NULL;
}

Glyph *TextCG_GetGlyphEnd(TextCG*g)
{
    TextCG *bin, *tmp;
    Glyph *glyph;
    Glyph *res;

    res = NULL;
    HASH_ITER(hh, g, bin, tmp){
        glyph = TextCG_cGlyph(bin);
        if(!res){
            res = glyph;
        }

        if(
            (glyph->y >= res->y && glyph->x > res->x)
            || glyph->y > res->y
        ){
            res = glyph;
        }
    }

    return res;
}

size_t TextCG_GetGlyphAmt(TextCG*g)
{
    return HASH_COUNT(g);
}

//
// Methods
//
void TextCG_AddGlyph(TextCG**g, int c, TextCharFormat fmt, Glyph glyph)
{
    TextCG *n_tfag;

    if(TextCG_GetGlyph(*g, c, fmt) != NULL){
        GRAPHICAL_LOG_CRITICAL("Character '%i' already into Text's Atlas's Glyph Hash", c);
        return;
    }

    n_tfag = calloc(1, sizeof(*n_tfag));
    if(!n_tfag){
        GRAPHICAL_LOG_ERROR("Out of memory while adding character '%i' to the glyph hash", c);
        return;
    }
    n_tfag->key.c = c;
    n_tfag->key.fmt = fmt;
    n_tfag->c_glyph = glyph;

    HASH_ADD(hh, *g, key, sizeof(n_tfag->key), n_tfag);
}

void TextCG_Destroy(TextCG**g)
{
    TextCG *bin, *tmp;

    HASH_ITER(hh, *g, bin, tmp) {
        // HASH_DEL tambem libera a tabela interna do uthash quando esvazia
        HASH_DEL(*g, bin);
        free(bin);
    }
}
