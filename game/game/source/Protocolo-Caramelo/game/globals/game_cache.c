#include"game_cache.h"
#include"game_core.h"

TextCache *cache[GFONT_COUNT];
static const char* const gfont_config[GFONT_COUNT][GFONT_CONFIG_COUNT] = GFONT_CONFIG;

void GameCache_Init()
{
    size_t i;
    for(i=0; i < GFONT_COUNT; ++i){
        cache[i] = TextCache_Create(
            r,
            gfont_config[i][GFONT_CONFIG_PATH],
            (int)gfont_config[i][GFONT_CONFIG_PTSIZE]
        );
    }
}

void GameCache_Quit()
{
    size_t i;
    for(i=0; i < GFONT_COUNT; ++i){
        TextCache_Destroy(&(cache[i]));
    }
}
