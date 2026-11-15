#ifndef GAME_GLOBALS_MECHANICS_H
#define GAME_GLOBALS_MECHANICS_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"mechanics/mechanics.h"

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

typedef enum Difficulty Difficulty;
enum Difficulty {
    GDIFF_EASY,
    GDIFF_NORMAL,
    GDIFF_HARD,
    GDIFF_IMPOSSIBLE,

    GDIFF_COUNT
};

#define CLOCK_INIT_SEC   0
#define CLOCK_INIT_MIN   0
#define CLOCK_INIT_HOUR  21
#define CLOCK_INIT_MDAY  31
#define CLOCK_INIT_MONTH MAI_
#define CLOCK_INIT_YEAR  2046
#define CLOCK_TIME_PLAYER_TO_TIME_GAME (60)
extern Clock *gclock;

#define MINESESSION_DURATION_IN_HOURS 3
#define MINESESSION_FIRST_HOUR 0
#define MINESESSION_SIZE 50
extern MathProblem *mineSession[MINESESSION_SIZE];
extern size_t mineSession_i;
/*
 * Wrote with help of Claude
 */
/* ---- Nível EASY ---- */
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_NORMAL                0.40
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_EQUATION_1            0.30
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_EQUATION_2            0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_NEWTON                0.03
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_LINEAR       0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_QUADRACTIC   0.03
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_EXPONENTIAL  0.02
#define MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_LOGARITHMICA 0.02

/* ---- Nível MEDIUM ---- */
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_NORMAL                0.20
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_EQUATION_1            0.20
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_EQUATION_2            0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_NEWTON                0.05
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_LINEAR       0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_QUADRACTIC   0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_EXPONENTIAL  0.08
#define MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_LOGARITHMICA 0.07

/* ---- Nível HARD ---- */
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_NORMAL                0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_EQUATION_1            0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_EQUATION_2            0.20
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_NEWTON                0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_LINEAR       0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_QUADRACTIC   0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_EXPONENTIAL  0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_LOGARITHMICA 0.10

/* ---- Nível IMPOSSIBLE ---- */
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_NORMAL                0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_EQUATION_1            0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_EQUATION_2            0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_NEWTON                0.20
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_LINEAR       0.05
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_QUADRACTIC   0.10
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_EXPONENTIAL  0.15
#define MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_LOGARITHMICA 0.10

/* ======================================================================
 * Dificuldades de problema (MathProblem_Difficulty)
 * ====================================================================== */

/* ---- Nível EASY ---- */
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_EASY        0.70
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_MEDIUM      0.30
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_HARD        0.00
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_IMPOSSIBLE  0.00

/* ---- Nível MEDIUM ---- */
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_EASY        0.25
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_MEDIUM      0.50
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_HARD        0.25
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_IMPOSSIBLE  0.00

/* ---- Nível HARD ---- */
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_EASY        0.00
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_MEDIUM      0.20
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_HARD        0.60
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_IMPOSSIBLE  0.20

/* ---- Nível IMPOSSIBLE ---- */
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_EASY        0.00
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_MEDIUM      0.00
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_HARD        0.20
#define MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_IMPOSSIBLE  0.80

extern SDL_Texture *chars_tex[C_COUNT][CS_COUNT];
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

#define GDIFF_INIT GDIFF_EASY
extern Difficulty gdiff;


void GameMechanics_Init();
void GameMechanics_Quit();

#endif
