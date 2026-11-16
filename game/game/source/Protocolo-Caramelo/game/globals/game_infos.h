#ifndef GAME_INFOS_H
#define GAME_INFOS_H

#define GINFO_MAX_VAL 100.0
//
#define GINFO_HEALTH_LOSS_BECAUSE_TIRED 50
#define GINFO_HEALTH_LOSS_EACH_FRAME(t) \
    (0.025 * pow(1.01859138805, t * 2.5) / 100)

#define GINFO_ENERGY_LOSS_EACH_FRAME 0.0025
#define GINFO_ENERGY_GAIN_AFTER_SLEEP(time_sec_sleep) MIN(GINFO_MAX_VAL, pow(1.00015991, time_sec_sleep)) // 8hours to complete all energy bar

#define GINFO_TIRED_GAIN_BECAUSE_ENERGY 75
#define GINFO_TIRED_GAIN_EACH_FRAME(e) \
    (0.050 * pow(1.04712854805, (100 - e)) / 100)
//
extern double i_health;
extern double i_energy;
extern double i_tired;
extern double i_caramelos;

void GameInfos_Init();
void GameInfos_Quit();

void GameInfos_Load(char*);
void GameInfos_LoadNewGame();
void GameInfos_Save();

#endif
