#include"GTextCache.h"

/* Maior codepoint representavel pelas funcoes TTF_*UNICODE (UCS-2). */
#define TEXTCACHE_MAX_BMP_CODEPOINT 0xFFFF
#define TEXTCACHE_REPLACEMENT_CODEPOINT 0xFFFD

/*
 * Altura de uma linha do atlas: nunca menor que a altura real da fonte, para
 * que linhas vizinhas de glifos nao se sobreponham.
 */
static int TextCache_RowHeight(TextCache*a)
{
    return MAX(TTF_FontLineSkip(*TextCache_font(a)), TTF_FontHeight(*TextCache_font(a)));
}

//
// Properties
//
Glyph* TextCache_GetGlyph(TextCache*a, int c, TextCharFormat fmt)
{
    Glyph *g;

    g = TextCG_GetGlyph(*TextCache_glyphs(a), c, fmt);
    if(g == NULL){
        TextCache_AddGlyph(a, c, fmt);
        g = TextCG_GetGlyph(*TextCache_glyphs(a), c, fmt);
    }
    return g;
}

size_t TextCache_GetGlyphAmt(TextCache*a)
{
    return TextCG_GetGlyphAmt(*TextCache_glyphs(a));
}

void TextCache_GetGlyphSize(TextCache*a, int c, TextCharFormat fmt, int *w, int *h)
{
    Glyph *g;
    g = TextCache_GetGlyph(a, c, fmt);

    if(w){
        *w = (g)? g->w: 0;
    }
    if(h){
        *h = (g)? g->h: 0;
    }
}


void TextCache_GetAtlasSize(TextCache*a, int *w, int *h)
{
    int width, height;
    SDL_QueryTexture(
        *TextCache_texture(a),
        NULL,
        NULL,
        &width,
        &height
    );

    if(w){
        *w = width;
    }
    if(h){
        *h = height;
    }
}

void TextCache_SetAtlasSize(TextCache*a, unsigned w, unsigned h)
{
    SDL_Texture *n_tex, *prev_target;
    SDL_Rect dest;
    TextCG *bin, *tmp;
    int row_height;

    n_tex = SDL_CreateTexture(
        *TextCache_renderer(a),
        TEXTFA_TEXTURE_PIXELFORMAT,
        TEXTFA_TEXTURE_ACCESS,
        w,
        h
    );
    if(!n_tex){
        // O atlas antigo continua valido; quem chamou percebe que o tamanho nao mudou.
        GRAPHICAL_LOG_ERROR("Error while creating resizing TextCache's texture: %s", SDL_GetError());
        return;
    }
    SDL_SetTextureBlendMode(
        n_tex,
        SDL_BLENDMODE_BLEND
    );

    dest = (SDL_Rect){0};
    row_height = TextCache_RowHeight(a);
    prev_target = SDL_GetRenderTarget(*TextCache_renderer(a));

    SDL_SetRenderTarget(*TextCache_renderer(a), n_tex);
    SDL_SetRenderDrawColor(*TextCache_renderer(a), 255, 255, 255, 0);
    SDL_RenderClear(*TextCache_renderer(a));
    HASH_ITER(hh, *TextCache_glyphs(a), bin, tmp) {
        dest.w = TextCG_cGlyph(bin)->w;
        dest.h = TextCG_cGlyph(bin)->h;
        if(dest.x + dest.w > (int)w){
            dest.x = 0;
            dest.y += row_height;
        }

        SDL_RenderCopy(
            *TextCache_renderer(a),
            *TextCache_texture(a),
            TextCG_cGlyph(bin),
            &dest
        );
        *TextCG_cGlyph(bin) = dest;

        dest.x += dest.w;
    }
    SDL_SetRenderTarget(*TextCache_renderer(a), prev_target);

    SDL_DestroyTexture(*TextCache_texture(a));
    *TextCache_texture(a) = n_tex;
}

//
// Methods
//
TextCache* TextCache_Create(SDL_Renderer*r, const char *f_path, unsigned ptsize)
{
    TextCache *n_cache;

    n_cache = calloc(1, sizeof(*n_cache));
    if(!n_cache){
        GRAPHICAL_LOG_ERROR("Out of memory while creating the cache of font \"%s\"", f_path);
        return NULL;
    }

    *TextCache_font(n_cache) = TTF_OpenFont(f_path, ptsize);
    if(!*TextCache_font(n_cache)){
        GRAPHICAL_LOG_ERROR("Error while opening font \"%s\": %s", f_path, TTF_GetError());
        free(n_cache);
        return NULL;
    }

    *TextCache_renderer(n_cache) = r;
    *TextCache_texture(n_cache) = SDL_CreateTexture(
        r,
        TEXTFA_TEXTURE_PIXELFORMAT,
        TEXTFA_TEXTURE_ACCESS,
        TEXTFA_DEFAULT_TEXTURE_WIDTH,
        TEXTFA_DEFAULT_TEXTURE_HEIGHT
    );
    if(!*TextCache_texture(n_cache)){
        GRAPHICAL_LOG_ERROR("Error while creating texture for cache of font \"%s\" with ptsize %u: %s", f_path, ptsize, SDL_GetError());
        TTF_CloseFont(*TextCache_font(n_cache));
        free(n_cache);
        return NULL;
    }
    SDL_SetTextureBlendMode(
        *TextCache_texture(n_cache),
        SDL_BLENDMODE_BLEND
    );
    Texture_Clear(&(n_cache->texture));

    *TextCache_glyphs(n_cache) = NULL;

    return n_cache;
}

void TextCache_AddGlyph(TextCache*a, int c, TextCharFormat fmt)
{
    TTF_Font *font;
    int row_height;
    //
    SDL_Rect dest;
    SDL_Color color;
    //
    SDL_Surface *chr_surf;
    SDL_Texture *chr_tex;
    Uint16 chr_raw[2];
    int chr_width, chr_height;
    //
    SDL_Texture *prev_target;
    Glyph *g_end;
    Glyph g_init;
    int atlas_width, atlas_height;

    if(TextCG_GetGlyph(*TextCache_glyphs(a), c, fmt) != NULL){
        GRAPHICAL_LOG_CRITICAL("Glyph '%i' already added to TextCache", c);
        return;
    }

    font = *TextCache_font(a);
    row_height = TextCache_RowHeight(a);
    color = TEXTFA_DEFAULT_GLYPH_COLOR;
    g_init = (Glyph){0};
    g_end = &g_init;
    if(*TextCache_glyphs(a) != NULL){
        g_end = TextCG_GetGlyphEnd(*TextCache_glyphs(a));
    }
    TextCache_GetAtlasSize(a, &atlas_width, &atlas_height);

    TTF_SetFontStyle(font, fmt);

    // As funcoes TTF_*UNICODE trabalham com UCS-2: codepoints fora do BMP
    // seriam truncados e desenhariam o glifo errado, entao usamos U+FFFD.
    if(c > TEXTCACHE_MAX_BMP_CODEPOINT || c < 0){
        GRAPHICAL_LOG_WARN("Character '%i' is outside the Basic Multilingual Plane; using U+FFFD", c);
        chr_raw[0] = TEXTCACHE_REPLACEMENT_CODEPOINT;
    }
    else {
        chr_raw[0] = (Uint16)c;
    }
    chr_raw[1] = 0;

    // Create texture for char
    chr_surf = TTF_RenderUNICODE_Blended(
        font, chr_raw, color
    );
    if(!chr_surf){
        // Caracteres de controle (ex.: '\n') nao tem desenho; nao e um erro de verdade.
        if(c < ' '){
            GRAPHICAL_LOG_DEBUG("Control character '%i' has no glyph to render", c);
        }
        else {
            GRAPHICAL_LOG_ERROR("Error while rendering char surf for character '%i': %s", c, TTF_GetError());
        }
        return;
    }
    if(TTF_SizeUNICODE(font, chr_raw, &chr_width, &chr_height) != 0){
        GRAPHICAL_LOG_ERROR("Error while measuring character '%i': %s", c, TTF_GetError());
        SDL_FreeSurface(chr_surf);
        return;
    }
    chr_tex = SDL_CreateTextureFromSurface(*TextCache_renderer(a), chr_surf);
    SDL_FreeSurface(chr_surf);
    if(!chr_tex){
        GRAPHICAL_LOG_ERROR("Error while creating texture for character '%i': %s", c, SDL_GetError());
        return;
    }

    // Get Glyph size
    dest.x = g_end->x + g_end->w;
    dest.y = g_end->y;
    dest.w = chr_width;
    dest.h = chr_height;
    if(dest.x + dest.w > atlas_width){
        dest.x = 0;
        dest.y += row_height;
    }

    // Resize if the atlas's texture isn't big enough
    if(dest.y + chr_height > atlas_height){
        int new_width, new_height;

        new_width = (atlas_width * 2 > chr_width)? atlas_width * 2: chr_width;
        new_height = (atlas_height * 2 > chr_height)? atlas_height * 2: chr_height;
        TextCache_SetAtlasSize(a, new_width, new_height);
        TextCache_GetAtlasSize(a, &atlas_width, &atlas_height);

        g_end = TextCG_GetGlyphEnd(*TextCache_glyphs(a));
        dest.x = dest.y = 0;
        if(g_end != NULL){
            dest.x = g_end->x + g_end->w;
            dest.y = g_end->y;
        }

        if(dest.x + chr_width > atlas_width){
            dest.x = 0;
            dest.y += row_height;
        }
        if(dest.y + chr_height > atlas_height){
            GRAPHICAL_LOG_ERROR("Error while resizing TextCache for character '%i': the atlas can't grow enough. Are you really using a good font?", c);
            SDL_DestroyTexture(chr_tex);
            return;
        }
    }

    // Render Glyph
    prev_target = SDL_GetRenderTarget(*TextCache_renderer(a));
    SDL_SetRenderTarget(*TextCache_renderer(a), *TextCache_texture(a));
    SDL_RenderCopy(
        *TextCache_renderer(a),
        chr_tex,
        NULL,
        &dest
    );
    SDL_SetRenderTarget(*TextCache_renderer(a), prev_target);

    SDL_DestroyTexture(chr_tex);

    // Add c to TextCG
    TextCG_AddGlyph(TextCache_glyphs(a), c, fmt, dest);
}

void TextCache_Destroy(TextCache**a)
{
    if(!a || !*a){
        return;
    }

    TextCG_Destroy(TextCache_glyphs(*a));
    SDL_DestroyTexture(*TextCache_texture(*a));
    TTF_CloseFont(*TextCache_font(*a));
    free(*a);
    *a = NULL;
}
