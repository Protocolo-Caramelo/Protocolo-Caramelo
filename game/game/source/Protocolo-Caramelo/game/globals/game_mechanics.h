#ifndef GAME_GLOBALS_MECHANICS_H
#define GAME_GLOBALS_MECHANICS_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"mechanics/mechanics.h"

#define CLOCK_TIME_PLAYER_TO_TIME_GAME (60)

extern Clock *clock;
extern MathProblem m_problem;

typedef enum Character Character;
typedef enum Character_State Character_State;
enum Character {
    C_PEDRO,
    C_FELIPE,
    C_ANA,
    C_JOSE,
    C_RICARDO,
    C_DONODOPREDIO,
    C_INSPETOR,

    C_COUNT
};
enum Character_State {
    CS_NORMAL,
    CS_HAPPY,
    CS_ANIMATED,
    CS_SAD,
    CS_ANGRY,
    CS_REVOLTED,
    CS_DISTRUSTING,
    CS_DARK,
    CS_SUPERANIMATED,

    CS_COUNT
};

extern SDL_Texture *chars_tex[C_COUNT][CS_COUNT];
extern const char* const chars_texPaths[C_COUNT][CS_COUNT];
#define CHARACTER_TEX_PATHS_PREFIX "./assets/images/characters"
#define CHARACTER_TEX_PATHS \
    {                           \
        [C_PEDRO][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Normal.png",                               \
        [C_PEDRO][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Happy.png",                                \
        [C_PEDRO][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Animated.png",                             \
        [C_PEDRO][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Sad.png",                                  \
        [C_PEDRO][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Angry.png",                                \
        [C_PEDRO][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Revolted.png",                             \
        [C_PEDRO][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Distrusting.png",                          \
        [C_PEDRO][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/Pedro/Dark.png",                                 \
        [C_PEDRO][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/Pedro/SuperAnimated.png",                        \
        \
        [C_FELIPE][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Normal.png",                             \
        [C_FELIPE][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Happy.png",                              \
        [C_FELIPE][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Animated.png",                           \
        [C_FELIPE][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Sad.png",                                \
        [C_FELIPE][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Angry.png",                              \
        [C_FELIPE][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Revolted.png",                           \
        [C_FELIPE][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Distrusting.png",                        \
        [C_FELIPE][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/Felipe/Dark.png",                               \
        [C_FELIPE][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/Felipe/SuperAnimated.png",                      \
        \
        [C_ANA][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/Ana/Normal.png",                                   \
        [C_ANA][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/Ana/Happy.png",                                    \
        [C_ANA][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/Ana/Animated.png",                                 \
        [C_ANA][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/Ana/Sad.png",                                      \
        [C_ANA][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/Ana/Angry.png",                                    \
        [C_ANA][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/Ana/Revolted.png",                                 \
        [C_ANA][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/Ana/Distrusting.png",                              \
        [C_ANA][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/Ana/Dark.png",                                     \
        [C_ANA][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/Ana/SuperAnimated.png",                            \
        \
        [C_JOSE][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/Jose/Normal.png",                                 \
        [C_JOSE][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/Jose/Happy.png",                                  \
        [C_JOSE][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/Jose/Animated.png",                               \
        [C_JOSE][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/Jose/Sad.png",                                    \
        [C_JOSE][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/Jose/Angry.png",                                  \
        [C_JOSE][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/Jose/Revolted.png",                               \
        [C_JOSE][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/Jose/Distrusting.png",                            \
        [C_JOSE][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/Jose/Dark.png",                                   \
        [C_JOSE][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/Jose/SuperAnimated.png",                          \
        \
        [C_RICARDO][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Normal.png",                           \
        [C_RICARDO][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Happy.png",                            \
        [C_RICARDO][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Animated.png",                         \
        [C_RICARDO][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Sad.png",                              \
        [C_RICARDO][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Angry.png",                            \
        [C_RICARDO][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Revolted.png",                         \
        [C_RICARDO][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Distrusting.png",                      \
        [C_RICARDO][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Dark.png",                             \
        [C_RICARDO][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/Ricardo/SuperAnimated.png",                    \
        \
        [C_DONODOPREDIO][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Normal.png",                 \
        [C_DONODOPREDIO][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Happy.png",                  \
        [C_DONODOPREDIO][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Animated.png",               \
        [C_DONODOPREDIO][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Sad.png",                    \
        [C_DONODOPREDIO][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Angry.png",                  \
        [C_DONODOPREDIO][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Revolted.png",               \
        [C_DONODOPREDIO][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Distrusting.png",            \
        [C_DONODOPREDIO][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Dark.png",                   \
        [C_DONODOPREDIO][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/SuperAnimated.png",          \
        \
        [C_INSPETOR][CS_NORMAL]        = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Normal.png",                         \
        [C_INSPETOR][CS_HAPPY]         = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Happy.png",                          \
        [C_INSPETOR][CS_ANIMATED]      = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Animated.png",                       \
        [C_INSPETOR][CS_SAD]           = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Sad.png",                            \
        [C_INSPETOR][CS_ANGRY]         = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Angry.png",                          \
        [C_INSPETOR][CS_REVOLTED]      = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Revolted.png",                       \
        [C_INSPETOR][CS_DISTRUSTING]   = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Distrusting.png",                    \
        [C_INSPETOR][CS_DARK]          = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Dark.png",                           \
        [C_INSPETOR][CS_SUPERANIMATED] = CHARACTER_TEX_PATHS_PREFIX"/Inspetor/SuperAnimated.png"                   \
        \
    }

void GameMechanics_Init();
void GameMechanics_Quit();

#endif
