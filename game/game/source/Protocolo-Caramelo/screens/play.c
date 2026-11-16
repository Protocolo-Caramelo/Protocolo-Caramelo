#include"play.h"
#include"play/room.h"
#include"play/monitor.h"
#include"play/pause.h"

#include"game/game.h"
#include"screens_communs.h"

static Game_State game_statePrev;
static Game_PlayState game_playStatePrev;
//
static int clock_timePrev;
static int clock_timeDelay;
//
int mineSession_paused     = 1;
int mineSession_pausedHour = -1;
int mineSession_modTime    = -1;
//
static const double mineSession_typeProbabilities[MATHPROBLEM_DIFFICULTY_COUNT][MATHPROBLEM_TYPE_COUNT] = {
    {   /* EASY */
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_NORMAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_EQUATION_1,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_EQUATION_2,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_NEWTON,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_LINEAR,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_QUADRACTIC,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_EXPONENTIAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_EASY_FUNCTION_LOGARITHMICA
    },
    {   /* MEDIUM */
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_NORMAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_EQUATION_1,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_EQUATION_2,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_NEWTON,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_LINEAR,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_QUADRACTIC,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_EXPONENTIAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_MEDIUM_FUNCTION_LOGARITHMICA
    },
    {   /* HARD */
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_NORMAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_EQUATION_1,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_EQUATION_2,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_NEWTON,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_LINEAR,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_QUADRACTIC,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_EXPONENTIAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_HARD_FUNCTION_LOGARITHMICA
    },
    {   /* IMPOSSIBLE */
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_NORMAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_EQUATION_1,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_EQUATION_2,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_NEWTON,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_LINEAR,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_QUADRACTIC,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_EXPONENTIAL,
        MINE_SEESSION_PROBLEMTYPE_PROB_IMPOSSIBLE_FUNCTION_LOGARITHMICA
    }
};
static const double mineSession_diffProbalities[MATHPROBLEM_DIFFICULTY_COUNT][MATHPROBLEM_DIFFICULTY_COUNT] = {
    {   /* EASY */
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_EASY,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_MEDIUM,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_HARD,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_EASY_IMPOSSIBLE
    },
    {   /* MEDIUM */
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_EASY,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_MEDIUM,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_HARD,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_MEDIUM_IMPOSSIBLE
    },
    {   /* HARD */
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_EASY,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_MEDIUM,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_HARD,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_HARD_IMPOSSIBLE
    },
    {   /* IMPOSSIBLE */
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_EASY,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_MEDIUM,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_HARD,
        MINE_SEESSION_PROBLEMDIFFICULTY_PROB_IMPOSSIBLE_IMPOSSIBLE
    }
};

// Display functions
void Play_DisplayBackground(GamePlay_Texture tex, SDL_Rect *tex_rect, SDL_Rect *tex_rectRaw, double*scale_width, double*scale_height)
{
    SDL_Texture *t;
    int tex_w, tex_h;
    int tex_nW, tex_nH;
    SDL_Rect rect, rectRaw;

    t = play_tex[tex];
    SDL_GetWindowSize(w, &w_width, &w_height);
    w_ratio = w_width / (double)w_height;
    SDL_QueryTexture(
        t,
        NULL, NULL,
        &tex_w, &tex_h
    );

    if(w_width / (double)w_height < tex_w / (double)tex_h){
        tex_nW = w_width;
        tex_nH = w_width * (GCORE_WINDOW_H / (double)GCORE_WINDOW_W);
    }
    else {
        tex_nH = w_height;
        tex_nW = w_height * (GCORE_WINDOW_W / (double)GCORE_WINDOW_H);
    }

    rect = (SDL_Rect){
        .x = w_width / 2 - tex_nW / 2,
        .y = w_height / 2 - tex_nH / 2,
        .w = tex_nW,
        .h = tex_nH
    };
    rectRaw = (SDL_Rect){
        .x = w_width / 2 - tex_nW / 2,
        .y = w_height / 2 - tex_nH / 2,
        .w = tex_w,
        .h = tex_h
    };

    //
    if(tex_rect){
        *tex_rect = rect;
    }
    if(tex_rectRaw){
        *tex_rectRaw = rectRaw;
    }

    if(scale_width){
        *scale_width = tex_rect->w / (double)rectRaw.w;
    }
    if(scale_height){
        *scale_height = tex_rect->h / (double)rectRaw.h;
    }

    SDL_RenderCopy(
        r,
        t,
        NULL,
        tex_rect
    );
}

void Play_DisplayButtonArrow(Button*b, SDL_Rect*bg_texDest, int x, int y)
{
    double scale;
    //
    int tex_w, tex_h;
    //
    int b_borderL, b_borderR, b_borderB;
    int b_borderLPrev, b_borderRPrev, b_borderBPrev;
    double scale_width, scale_height;
    double scale_widthPrev, scale_heightPrev;
    char *str_prev;
    //
    size_t i;

    //
    b_borderLPrev = b_borderL       = b->state_tc[INP_STATE_NEUTRAL]->border.left;
    b_borderRPrev = b_borderR       = b->state_tc[INP_STATE_NEUTRAL]->border.right;
    b_borderBPrev = b_borderB       = b->state_tc[INP_STATE_NEUTRAL]->border.bottom;
    scale_widthPrev = scale_width   = b->state_tc[INP_STATE_NEUTRAL]->scale_width;
    scale_heightPrev = scale_height = b->state_tc[INP_STATE_NEUTRAL]->scale_height;
    str_prev                        = malloc(sizeof(Text_GetLenUTF8(b->text)));

    if(w_width / (double)w_height < bg_texDest->w / (double)bg_texDest->h){
        scale = w_width / (double)GCORE_WINDOW_W;
    }
    else {
        scale = w_height / (double)GCORE_WINDOW_H;
    }

    b_borderR    *= scale;
    b_borderL    *= scale;
    b_borderB    *= scale;
    scale_width  *= scale;
    scale_height *= scale;
    utf8cpy(str_prev, *Text_raw(b->text));

    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i]->border.left = b_borderL;
        b->state_tc[i]->border.right = b_borderR;
        b->state_tc[i]->border.bottom = b_borderB;
        //
        b->state_tc[i]->scale_width = scale_width;
        b->state_tc[i]->scale_height = scale_height;
        //
    }
    if(!b->state[INP_STATE_SELECTED] && !b->state[INP_STATE_HOVER]){
        free(*Text_raw(b->text));
        *Text_raw(b->text) = NULL;
    }

    //
    //*
    Text_MeasureUTF8(b->text, NULL, NULL);
    Text_MeasureMargin(b->text, NULL, NULL);
    x -= ((*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_XMAX] - (*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_XMIN]) / 2;
    y -= ((*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_YMAX] - (*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_YMIN]) / 2;
    // */
    /*
    x -= (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    y -= (*Text_edges(b->text))[DRAW_HEIGHT] / 2;
    // */
    Button_Display(
        b,
        x,
        y,
        NULL
    );

    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i]->border.left = b_borderLPrev;
        b->state_tc[i]->border.right = b_borderRPrev;
        b->state_tc[i]->border.bottom = b_borderBPrev;
        //
        b->state_tc[i]->scale_width = scale_widthPrev;
        b->state_tc[i]->scale_height = scale_heightPrev;
    }

    //
    tex_w = b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL].w;
    for(i=0; i < INP_STATES_MAX && (*Text_edges(b->text))[DRAW_WIDTH] < tex_w * GPLAY_BUTTON_ARROW_TEXTURE_SCALE * scale; ++i){
        b->state_tc[i]->border.right =
        b->state_tc[i]->border.left =
            ((tex_w + 2) * GPLAY_BUTTON_ARROW_TEXTURE_SCALE
            - (*Text_edges(b->text))[DRAW_WIDTH]) / 2.0
            ;
    }

    //
    Text_SetUTF8(b->text, str_prev);
    free(str_prev);
}

void Play_DisplayStatus(SDL_Rect *tex_dest, double texScale_w, double texScale_h)
{
    SDL_Rect dest, dest_src;
    SDL_Rect destInv, destInv_src;
    SDL_Texture *tex, *texInv;
    int tex_w, tex_h;
    int texInv_w, texInv_h;
    //
    int infos[] = {
        i_health, 
        i_energy,
        i_tired
    };
    int x, y;
    double p;
    size_t i;
    //

    //
    x = tex_dest->x + 20 * texScale_w;
    y = tex_dest->y + 50 * texScale_h;
    for(i=0; i < sizeof(infos) / sizeof(infos[0]); ++i){
        tex    = status_tex[i * 2];
        texInv = status_tex[i * 2 + 1];

        p = infos[i] / GINFO_MAX_VAL;
        SDL_QueryTexture(
            tex,
            NULL, NULL,
            &tex_w, &tex_h
        );
        SDL_QueryTexture(
            tex,
            NULL, NULL,
            &texInv_w, &texInv_h
        );
        //
        dest = (SDL_Rect){
            .x = x,
            .y = y,
            .w = (p * GSTATUS_TEX_SIZE_W) * texScale_w,
            .h = GSTATUS_TEX_SIZE_H * texScale_h
        };
        dest_src = (SDL_Rect){
            .x = tex_w * (1 - p),
            .w = (p * tex_w),
            .h = tex_h,
        };

        destInv = (SDL_Rect) {
            .x = dest.x + dest.w,
            .y = dest.y,
            .w = ((1 - p) * GSTATUS_TEX_SIZE_W) * texScale_w,
            .h = GSTATUS_TEX_SIZE_H * texScale_h
        };
        destInv_src = (SDL_Rect){
            .w = (1 - p) * texInv_w,
            .h = texInv_h
        };
        SDL_RenderCopy(
            r,
            tex,
            &dest_src,
            &dest
        );
        SDL_RenderCopy(
            r,
            texInv,
            &destInv_src,
            &destInv
        );
        
        y = dest.y + dest.h + 10 * texScale_h;
    }
}

// Manage mechanics
void Play_ManageClock()
{
    if(game_statePrev != game_state || game_playStatePrev == GAME_PLAY_STATE_PAUSED){
        clock_timePrev = SDL_GetTicks();
    }

    clock_timeDelay = SDL_GetTicks() - clock_timePrev;
    Clock_SumMs(gclock, clock_timeDelay * CLOCK_TIME_PLAYER_TO_TIME_GAME);
    clock_timePrev = SDL_GetTicks();
}

void Play_GetClockFormat(char*to, char*fmt)
{
    Clock_sprintf(gclock, to, fmt);
}


void Play_QuitMineSession()
{
    size_t i;
    for(i=0; i < MINESESSION_SIZE; ++i){
        if(!mineSession[i]){
            continue;
        }
        free(mineSession[i]);
        mineSession[i] = NULL;
    }
}

MathProblem_Type Play_GenMineSessionProblemType(Difficulty diff)
{
    double target = rand() / ((double)RAND_MAX + 1.0);
    double accumulated = 0.0;
    int chosen = MATHPROBLEM_NORMAL;

    if ((int)diff < 0 || (int)diff >= MATHPROBLEM_DIFFICULTY_COUNT) {
        return MATHPROBLEM_NORMAL;
    }

    for (int i = 0; i < MATHPROBLEM_TYPE_COUNT; i++) {
        if (mineSession_typeProbabilities[diff][i] <= 0.0) {
            continue;
        }
        accumulated += mineSession_typeProbabilities[diff][i];
        chosen = i;
        if (target < accumulated) {
            break;
        }
    }
    return (MathProblem_Type)chosen;
}

MathProblem_Difficulty Play_GenMineSessionProblemDiff(Difficulty diff)
{
    double target = rand() / ((double)RAND_MAX + 1.0);
    double accumulated = 0.0;
    int chosen = MATHPROBLEM_DIFFICULTY_EASY;

    for (int i = 0; i < MATHPROBLEM_DIFFICULTY_COUNT; i++) {
        if (mineSession_diffProbalities[diff][i] <= 0.0) {
            continue;
        }
        accumulated += mineSession_diffProbalities[diff][i];
        chosen = i;
        if (target < accumulated) {
            break;
        }
    }
    return (MathProblem_Difficulty)chosen;
}

void Play_GenMineSession()
{
    size_t i;
    MathProblem_Type m_type;
    MathProblem_Difficulty m_diff;

    Play_QuitMineSession();

    for(i=0; i < MINESESSION_SIZE; ++i){
        m_type = Play_GenMineSessionProblemType(gdiff);
        m_diff = Play_GenMineSessionProblemDiff(gdiff);
        mineSession[i] = malloc(sizeof(*mineSession[i]));
        MathProblem_GenProblem(mineSession[i], m_type, m_diff);
    }
    mineSession_i = 0;
}

void Play_ManageMineSession()
{
    int hour;

    hour = Clock_hour(gclock);
    mineSession_modTime = (MAX(hour - MINESESSION_FIRST_HOUR, MINESESSION_FIRST_HOUR - hour) + MINESESSION_DURATION_IN_HOURS) % MINESESSION_DURATION_IN_HOURS;

    if(!mineSession_modTime && mineSession_pausedHour != Clock_hour(gclock)){
        mineSession_paused = !mineSession_paused;
        mineSession_pausedHour = Clock_hour(gclock);
        if(!mineSession_paused){
            Play_GenMineSession();
        }
        else {
            Play_QuitMineSession();
        }
    }
}


void Play_ManageStatus()
{
    i_health = MAX(MIN(GINFO_MAX_VAL, i_health), 0);
    i_energy = MAX(MIN(GINFO_MAX_VAL, i_energy), 0);
    i_tired  = MAX(MIN(GINFO_MAX_VAL, i_tired), 0);

    i_energy -= GINFO_ENERGY_LOSS_EACH_FRAME;
    i_tired += GINFO_TIRED_GAIN_EACH_FRAME(i_energy);
    i_health -= GINFO_HEALTH_LOSS_EACH_FRAME(i_tired);

    /*
    printf(
        "(%.2lf %.2lf %.2lf) (%lf %lf %lf)\n",
        i_energy, i_tired, i_health,
        GINFO_ENERGY_LOSS_EACH_FRAME, GINFO_TIRED_GAIN_EACH_FRAME(i_energy), GINFO_HEALTH_LOSS_EACH_FRAME(i_tired)
        );
        // */
}

//
void Play_ResetButtons()
{
    size_t i;
    Button *b;

    for(i=0; i < GPLAY_BUTTON_COUNT; ++i){
        b = play_buttons[i];
        memset(b->state, 0, sizeof(b->state));
    }
}

void Play_Render()
{
    if(game_statePrev != game_state || game_playStatePrev != game_playState){
        Play_ResetButtons();
    }
    Play_ManageClock();
    Play_ManageMineSession();
    Play_ManageStatus();
    //
    game_statePrev = game_state;
    game_playStatePrev = game_playState;

    switch(game_playState){
        case GAME_PLAY_STATE_VIEW_ROOM_1:
            Room1_Render();
            break;

        case GAME_PLAY_STATE_VIEW_ROOM_2:
            Room2_Render();
            break;

        case GAME_PLAY_STATE_VIEW_MONITOR:
            Monitor_Render();
            break;

        case GAME_PLAY_STATE_PAUSED:
            Pause_Render();
            break;

        default:
            break;
    }

    Dialog_Render();
}
