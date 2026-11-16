#include"monitor_apps.h"
#include"app/taskmgr.h"
#include"app/miner.h"
#include"app/bateboca.h"

static const MonitorApp apps[MONITOR_APP_COUNT] = {
    [MONITOR_APP_BATEBOCA] = {
        .icon_label     = "Bate-boca",
        .window_title   = "Bate-boca",
        .icon_path      = "./assets/images/monitor/icon_bateboca.png",
        .icon_cell      = {7, 0},
        .window_w       = 1000, .window_h     = 700,
        .window_min_w   = 700,  .window_min_h = 420,
        .init           = AppBateBoca_Init,
        .quit           = AppBateBoca_Quit,
        .draw_content   = AppBateBoca_Draw,
        .handle_event   = AppBateBoca_HandleEvent,
    },
    [MONITOR_APP_MINE] = {
        .icon_label     = "Minerar",
        .window_title   = "Minerar",
        .icon_path      = "./assets/images/monitor/icon_mine.png",
        .icon_cell      = {7, 1},
        .window_w       = 900,  .window_h     = 680,
        .window_min_w   = 480,  .window_min_h = 340,
        .init           = AppMiner_Init,
        .quit           = AppMiner_Quit,
        .draw_content   = AppMiner_Draw,
        .handle_event   = AppMiner_HandleEvent,
    },
    [MONITOR_APP_DELIVERY] = {
        .icon_label     = "EntregasRápidas",
        .window_title   = "Entregas Rápidas",
        .icon_path      = "./assets/images/monitor/icon_delivery.png",
        .icon_cell      = {7, 2},
        .window_w       = 900,  .window_h     = 680,
        .window_min_w   = 480,  .window_min_h = 340,
    },
    [MONITOR_APP_TASKMGR] = {
        .icon_label     = "Gerenciador",
        .window_title   = "Gerenciador de Tarefas",
        .icon_path      = "./assets/images/monitor/icon_taskmgr.png",
        .icon_cell      = {7, 3},
        .window_w       = 760,  .window_h     = 520,
        .window_min_w   = 580,  .window_min_h = 420,
        .init           = AppTaskManager_Init,
        .quit           = AppTaskManager_Quit,
        .draw_content   = AppTaskManager_Draw,
        .handle_event   = AppTaskManager_HandleEvent,
    },
};

const MonitorApp* MonitorApps_Get(MonitorAppId id)
{
    if(id < 0 || id >= MONITOR_APP_COUNT){
        return NULL;
    }
    return &apps[id];
}

void MonitorApps_InitAll(void)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(apps[i].init){
            apps[i].init();
        }
    }
}

void MonitorApps_QuitAll(void)
{
    size_t i;

    for(i = 0; i < MONITOR_APP_COUNT; ++i){
        if(apps[i].quit){
            apps[i].quit();
        }
    }
}
