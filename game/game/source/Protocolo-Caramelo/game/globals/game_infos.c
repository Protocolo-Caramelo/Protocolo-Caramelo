#include"game_infos.h"
#include"game_core.h"
#include<math.h>

double i_health;
double i_energy;
double i_tired;
int i_caramelos;


void GameInfos_Init()
{
}

void GameInfos_Quit()
{
}

void GameInfos_Load(char*fpath)
{
}

void GameInfos_LoadNewGame(){
    //*
    i_caramelos = 0;
    i_health    = 100;
    i_energy    = 100;
    i_tired     = 0;
    // */

    /*
    i_caramelos = 0;
    i_health    = 100;
    i_energy    = 100;
    i_tired     = 0;
    // */
}

void GameInfos_Save()
{
}

double GameInfos_SleepFraction(double curve, double hours)
{
    double x;

    x = hours / GINFO_SLEEP_FULL_HOURS;
    x = (x < 0)? 0: (x > 1)? 1: x;
    return (pow(curve, x) - 1.0) / (curve - 1.0);
}

void GameInfos_SleepApply(double hours, double e0, double t0, double h0)
{
    i_energy = e0 + (GINFO_MAX_VAL - e0)
        * GameInfos_SleepFraction(GINFO_SLEEP_CURVE_ENERGY, hours);
    i_tired  = t0 * (1.0 - GameInfos_SleepFraction(GINFO_SLEEP_CURVE_TIRED, hours));
    i_health = h0 + GINFO_SLEEP_HEALTH_GAIN_MAX
        * GameInfos_SleepFraction(GINFO_SLEEP_CURVE_HEALTH, hours);
    if(i_health > GINFO_MAX_VAL){
        i_health = GINFO_MAX_VAL;
    }
}
