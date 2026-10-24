#ifndef GTEXTCACHE_H
#define GTEXTCACHE_H

#include"GCommons.h"
#include"GTexture.h"
#include"GTextTypedefs.h"
#include"GTextCharFormat.h"
#include"GTextCacheGlyph.h"

#define TEXTFA_TEXTURE_PIXELFORMAT          SDL_PIXELFORMAT_RGBA8888
#define TEXTFA_TEXTURE_ACCESS               SDL_TEXTUREACCESS_TARGET
#define TEXTFA_DEFAULT_GLYPH_COLOR          (SDL_Color){255, 255, 255, 255}
#define TEXTFA_DEFAULT_TEXTURE_WIDTH        256
#define TEXTFA_DEFAULT_TEXTURE_HEIGHT       256

struct TextCache {
    TTF_Font *font;
    Texture texture;
    TextCG *glyphs;
};
WRAP_FIELD(TextCache, font, TTF_Font*);
WRAP_FIELD(TextCache, texture.renderer, renderer, SDL_Renderer*);
WRAP_FIELD(TextCache, texture.raw, texture, SDL_Texture*);
WRAP_FIELD(TextCache, glyphs, TextCG*);

//
// Properties
//
/*
 * Retorna o glifo (criando-o no atlas se ainda nao existir).
 * Retorna NULL se o glifo nao puder ser renderizado.
 */
Glyph* TextCache_GetGlyph(TextCache*, int c, TextCharFormat);
size_t TextCache_GetGlyphAmt(TextCache*);
/* Se o glifo nao puder ser obtido, *w e *h recebem 0. */
void TextCache_GetGlyphSize(TextCache*, int c, TextCharFormat, int*, int*);

void TextCache_GetAtlasSize(TextCache*, int*, int*);
void TextCache_SetAtlasSize(TextCache*, unsigned, unsigned);

//
// Methods
//
/* Retorna NULL (e registra o erro) se a fonte ou a textura nao puderem ser criadas. */
TextCache* TextCache_Create(SDL_Renderer*, const char*, unsigned);

void TextCache_AddGlyph(TextCache*, int c, TextCharFormat fmt);
/* Libera o cache e deixa *a == NULL. Aceita NULL. */
void TextCache_Destroy(TextCache**);

#endif
