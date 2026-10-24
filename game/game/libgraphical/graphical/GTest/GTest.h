#if !defined(GTEST_H) \
    && (defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE))
#define GTEST_H

#include"GText/GTextCharFormat.h"
#include"GText/GTextCacheGlyph.h"
#include"GText/GTextCache.h"
#include"GText/GText.h"

#include"GInit.h"

#define GTEST_DELAY 16
#define FPS_DELAY 100

#define CHR_ROTATION_DELAY 10000
#define CHR_ROTATION_MAX 360.0
#define TXT_ROTATION_DELAY 50000
#define TXT_ROTATION_MAX 360.0

//
// Hardware test
//
void SimpleWindow();
void SimpleTexture();

//
// GText
//
void TextCG_Test();
void TextCache_Test(char*fpath, unsigned ptsize);
void Text_Test1(char*fpath, unsigned ptsize, char*);
void Text_Test2(char*fpath, unsigned ptsize, char*);
void Text_Test3(char*fpath, unsigned ptsize, char*);
void Text_Test4(char*fpath, unsigned ptsize, char*);

#endif
