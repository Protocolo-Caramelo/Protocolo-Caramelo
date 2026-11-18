#include"gameover.h"
#include"game/game.h"
#include"play.h"
#include"play/bed.h"

static Text *t_title;
static Text *t_reason;
static Text *t_hint;
static char  reason_buffer[128];

static Text *GameOver_CreateText(int font, double scale, SDL_Color color)
{
    return Text_Create(
        cache[font],
        TextConfig_Create(
            .scale_width  = scale,
            .scale_height = scale,
            .color = color,
        )
    );
}

static void GameOver_Destroy()
{
    if(t_title){  Text_Destroy(&t_title);  }
    if(t_reason){ Text_Destroy(&t_reason); }
    if(t_hint){   Text_Destroy(&t_hint);   }
}

void GameOver_Trigger(GameOver_Reason reason)
{
    if(game_state == GAME_STATE_SCREEN_GAMEOVER){
        return;
    }

    // Close everything that may be open over the play screen
    Dialog_Abort();
    if(game_playState == GAME_PLAY_STATE_BED){
        Bed_Leave();
    }

    switch(reason){
        case GAMEOVER_BILL:
            snprintf(
                reason_buffer, sizeof(reason_buffer),
                "Você não pagou a conta (%s) a tempo e foi despejado.",
                Bills_FailedName()? Bills_FailedName(): "aluguel"
            );
            break;

        case GAMEOVER_HEALTH:
        default:
            snprintf(
                reason_buffer, sizeof(reason_buffer),
                "Sua saúde chegou a zero. Pedro não aguentou."
            );
            break;
    }

    GameOver_Destroy();
    t_title  = GameOver_CreateText(GFONT_3D,      1.0 / 2.0, (SDL_Color){220, 40, 40, 255});
    t_reason = GameOver_CreateText(GFONT_REGULAR, 1.0 / 4.0, (SDL_Color){255, 255, 255, 255});
    t_hint   = GameOver_CreateText(GFONT_REGULAR, 1.0 / 5.0, (SDL_Color){255, 180, 27, 255});

    game_state = GAME_STATE_SCREEN_GAMEOVER;
}

static void GameOver_ToMenu()
{
    GameOver_Destroy();
    game_state = GAME_STATE_SCREEN_MENU;
    GamePlay_Quit();
}

static void GameOver_drawCentered(Text *t, char *str, int win_w, int y)
{
    int w_text;

    Text_SetUTF8(t, str);
    w_text = (*Text_edges(t))[DRAW_WIDTH];   // size from the previous frame
    Text_DrawUTF8(t, str, TextConfig_Create(.x = (win_w - w_text) / 2, .y = y), NULL);
}

void GameOver_Render()
{
    int win_w, win_h;

    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT:
                Game_Quit();
                return;

            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_RETURN
                    || e.key.keysym.sym == SDLK_SPACE
                    || e.key.keysym.sym == SDLK_ESCAPE){
                    GameOver_ToMenu();
                    return;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                GameOver_ToMenu();
                return;

            default:
                break;
        }
    }

    SDL_GetWindowSize(w, &win_w, &win_h);
    SDL_SetRenderDrawColor(r, 15, 0, 0, 255);
    SDL_RenderClear(r);

    GameOver_drawCentered(t_title,  "FIM DE JOGO", win_w, win_h / 4);
    GameOver_drawCentered(t_reason, reason_buffer,  win_w, win_h / 2);
    GameOver_drawCentered(t_hint,   "Clique ou aperte Enter para voltar ao menu", win_w, win_h * 3 / 4);
}
