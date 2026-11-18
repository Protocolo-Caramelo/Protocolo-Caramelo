#ifndef GAME_INFOS_H
#define GAME_INFOS_H

#define GINFO_MAX_VAL 100.0

//
#define GINFO_HEALTH_LOSS_EACH_FRAME(t) \
    (0.025 * pow(1.00925288608, t * 5) / 100)

#define GINFO_ENERGY_LOSS_EACH_QUESTION_WRONG    0.01
#define GINFO_ENERGY_LOSS_EACH_QUESTION_CORRECT  0.0025
#define GINFO_ENERGY_LOSS_EACH_FRAME 0.0025
// #define GINFO_ENERGY_GAIN_AFTER_SLEEP(time_sec_sleep) MIN(GINFO_MAX_VAL, pow(1.00015991, time_sec_sleep)) // 8hours to complete all energy bar

#define GINFO_TIRED_GAIN_EACH_FRAME(e) \
    (0.050 * pow(1.04712854805, (100 - e)) / 100)

#define GINFO_CARAMELOS_GAIN_MIN   10
#define GINFO_CARAMELOS_GAIN_MAX_E 800.0  // Easy
#define GINFO_CARAMELOS_GAIN_MAX_M 1000.0 // Meddium
#define GINFO_CARAMELOS_GAIN_MAX_H 2000.0 // Hard
#define GINFO_CARAMELOS_GAIN_MAX_I 4000.0 // Impossible
#define GINFO_CARAMELOS_GAIN_EACH_QUESTION_CORRECT(q_diff, t_init, t_end) \
    (   \
     MAX( \
         ( \
         (q_diff == MATHPROBLEM_DIFFICULTY_EASY)?       GINFO_CARAMELOS_GAIN_MAX_E: \
         (q_diff == MATHPROBLEM_DIFFICULTY_MEDIUM)?     GINFO_CARAMELOS_GAIN_MAX_M: \
         (q_diff == MATHPROBLEM_DIFFICULTY_HARD)?       GINFO_CARAMELOS_GAIN_MAX_H: \
         (q_diff == MATHPROBLEM_DIFFICULTY_IMPOSSIBLE)? GINFO_CARAMELOS_GAIN_MAX_I: \
         0 \
         ) * 120000.0 / (MAX(t_end - t_init, 1)), \
         GINFO_CARAMELOS_GAIN_MIN \
        ) * 100.0 \
    )
#define GINFO_CARAMELOS_GAIN_SEACH_QUESTION_WRONG 0

//
extern double i_health;
extern double i_energy;
extern double i_tired;
extern int i_caramelos;

void GameInfos_Init();
void GameInfos_Quit();

void GameInfos_Load(char*);
void GameInfos_LoadNewGame();
void GameInfos_Save();

#endif
