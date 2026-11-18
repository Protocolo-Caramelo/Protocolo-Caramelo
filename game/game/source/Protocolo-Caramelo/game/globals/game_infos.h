#ifndef GAME_INFOS_H
#define GAME_INFOS_H

#define GINFO_MAX_VAL 100.0

//
/* Saude: perde exponencialmente conforme o sono (t). Em t=0 ~0.2/h de jogo;
 * em t=100 ~22/h (morre em ~4.6h de jogo totalmente exausto). */
#define GINFO_HEALTH_LOSS_EACH_FRAME(t) \
    (0.006 * pow(1.00925288608, t * 5) / 100)

/* Energia: queda linear. 1 s real = 1 min de jogo, 60 FPS:
 * 0.0012/quadro = ~4.3 de energia por hora de jogo (16h acordado ~ -69). */
#define GINFO_ENERGY_LOSS_EACH_QUESTION_WRONG    0.60
#define GINFO_ENERGY_LOSS_EACH_QUESTION_CORRECT  0.25
#define GINFO_ENERGY_LOSS_EACH_FRAME 0.0012
// #define GINFO_ENERGY_GAIN_AFTER_SLEEP(time_sec_sleep) MIN(GINFO_MAX_VAL, pow(1.00015991, time_sec_sleep)) // 8hours to complete all energy bar

/* Sono: sobe exponencialmente conforme a falta de energia (e).
 * e=100: ~0.7/h; e=60: ~4.5/h; e=30: ~18/h. */
#define GINFO_TIRED_GAIN_EACH_FRAME(e) \
    (0.020 * pow(1.04712854805, (100 - e)) / 100)

/* ---- Descanso na cama ----
 * Fracao recuperada f(x) = (B^x - 1) / (B - 1), x = horas / 8h (limitado a 1).
 * Quanto maior B, mais "tardia" a recuperacao (curva mais curva).
 *   energia: B=100 -> 4h dao so ~9% da recuperacao, 8h completam tudo
 *   sono:    B=10  -> 4h dao ~24%, 8h zeram o sono
 *   saude:   B=3   -> quase linear; 8h dao +GINFO_SLEEP_HEALTH_GAIN_MAX */
#define GINFO_SLEEP_FULL_HOURS         8.0
#define GINFO_SLEEP_CURVE_ENERGY       100.0
#define GINFO_SLEEP_CURVE_TIRED        10.0
#define GINFO_SLEEP_CURVE_HEALTH       3.0
#define GINFO_SLEEP_HEALTH_GAIN_MAX    35.0
/* Animação do descanso: segundos REAIS por hora de jogo (8h = 6s) */
#define GINFO_SLEEP_ANIM_SEC_PER_HOUR  0.75

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

/* Fracao (0..1) recuperada apos `hours` de sono com a curva de base `curve` */
double GameInfos_SleepFraction(double curve, double hours);
/* Define energia/sono/saude como se o jogador tivesse dormido `hours` a partir
 * dos valores iniciais dados (chamar todo quadro com o total acumulado). */
void GameInfos_SleepApply(double hours, double e0, double t0, double h0);

void GameInfos_Init();
void GameInfos_Quit();

void GameInfos_Load(char*);
void GameInfos_LoadNewGame();
void GameInfos_Save();

#endif
