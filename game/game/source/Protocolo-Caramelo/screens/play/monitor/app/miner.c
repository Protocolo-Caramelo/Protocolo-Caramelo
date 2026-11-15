#include"miner.h"
#include"screens/play.h"

#define PADDING_R 5
#define PADDING_L 2
#define PADDING_T 2
#define PADDING_B 2

#define MINESESSION_PAUSED "Nenhuma sessão disponível no momento..."

static MonitorText *t_question;
static MonitorText *t_status;
static MonitorText *t_statusAnswer;
#define TI_ANSWER_PLACEHOLD "Digite Sua Respota Aqui!"
static TextInput *ti_answer;
//
static MonitorText *tip;
#define T_MINESESSIONTIME_TITLE_SPRINTF_REMAIN_TIME_TO_END "Tempo restante"
#define T_MINESESSIONTIME_TITLE_SPRINTF_REMAIN_TIME_TO_START "Sessão começa em"
#define T_MINESESSIONTIME_CLOCK_SPRINTF "%02i:%02i:%02is"
static MonitorText *t_mineSessionTime_title;
static MonitorText *t_mineSessionTime_clock;
char t_mineSessionTime_buffer[43];

enum SUBMIT_STATUS {
    SUBMIT_WRONG,
    SUBMIT_CORRECT,
    SUBMIT_NOT_SEND,
};
static int status = SUBMIT_NOT_SEND;


#define STATUS_WRONG        " Errado"
#define STATUS_CORRECT      " Correto"
#define STATUS_NOT_SUBMITED " N/A"
//
#define STATUS_WRONG_COLOR        (SDL_Color){255, 0, 0, 255}
#define STATUS_CORRECT_COLOR      (SDL_Color){0, 255, 0, 255}
#define STATUS_NOT_SUBMITED_COLOR (SDL_Color){0, 0, 0, 125}

void AppMiner_drawVertical(const SDL_Rect *body)
{
}

void AppMiner_drawHorizontal(const SDL_Rect *body)
{
    MonitorText *t;
    TextConfig *tc;
    TextConfig tc_prev[INP_STATES_MAX];
    int t_w, t_h;
    double scale;
    //
    int x, y;
    size_t i;

    scale = MonitorSpace_TextScale();

    // Question
    t = t_question;

    if(mineSession[mineSession_i]){
        Text_SetUTF8(t->text, mineSession[mineSession_i]->question);
    }
    else {
        Text_SetUTF8(t->text, MINESESSION_PAUSED);
    }
    t->config.wrap = body->w / 2 - PADDING_R * scale;

    MonitorText_Measure(t, scale, &t_w, &t_h);
    x = body->x + body->w / 2 - PADDING_R * scale;
    y = body->y + PADDING_T;
    MonitorText_Draw(t, x, y, scale);

    // Status
    t = t_status;
    x = (*Text_edges(t_question->text))[DRAW_EDGE_XMIN];
    y = MAX(body->y + body->h / 2, (*Text_edges(t_question->text))[DRAW_EDGE_YMAX] + 10 * scale);
    MonitorText_Draw(
        t,
        x, y,
        scale
    );

    t = t_statusAnswer;
    x = (*Text_edges(t_status->text))[DRAW_EDGE_XMAX];
    y = (*Text_edges(t_status->text))[DRAW_EDGE_YMIN];
    switch(status){
        case SUBMIT_NOT_SEND:
            Text_SetUTF8(t->text, STATUS_NOT_SUBMITED);
            t->config.color = STATUS_NOT_SUBMITED_COLOR;
            break;

        case SUBMIT_CORRECT:
            Text_SetUTF8(t->text, STATUS_CORRECT);
            t->config.color = STATUS_CORRECT_COLOR;
            break;

        case SUBMIT_WRONG:
            Text_SetUTF8(t->text, STATUS_WRONG);
            t->config.color = STATUS_WRONG_COLOR;
            break;

        default:
            break;
    }
    MonitorText_Draw(
        t,
        x, y,
        scale
    );
 
    // Answer
    for(i=0;  i < INP_STATES_MAX; ++i){
        if(!ti_answer->state_tc[i]){
            continue;
        }
        tc = ti_answer->state_tc[i];
        tc_prev[i] = *tc;

        tc->scale_width  *= scale;
        tc->scale_height *= scale;
        //
        tc->border.bottom  *= scale;
        tc->border.top     *= scale;
        tc->border.left    *= scale;
        tc->border.right   *= scale;
        //
        tc->padding.bottom *= scale;
        tc->padding.top    *= scale;
        tc->padding.left   *= scale;
        tc->padding.right  *= scale;
        //
        tc->wrap =
            body->w / 2
            - (
                PADDING_R
                + tc->padding.left + tc->padding.right
                + tc->border.left  + tc->border.right
            ) * scale
            ;

        tc->bx_width  = ti_answer->state_tc[i]->wrap;
        tc->bx_height = *Text_lineskip(ti_answer->text) * scale;

    }
    x = (*Text_edges(t_status->text))[DRAW_EDGE_XMIN];
    y = (*Text_edges(t_status->text))[DRAW_EDGE_YMAX] + 10 * scale;
    TextInput_Display(ti_answer, x, y, NULL);
    for(i=0;  i < INP_STATES_MAX; ++i){
        if(!ti_answer->state_tc[i]){
            continue;
        }
        *ti_answer->state_tc[i] = tc_prev[i];
    }

    // Tip
    t = tip;
    x = (*Text_edges(ti_answer->text))[DRAW_EDGE_XMIN];
    y = (*Text_edges(ti_answer->text))[DRAW_EDGE_YMAX] + 5 * scale;
    t->config.wrap = body->w / 2;
    MonitorText_Draw(t, x, y, scale);

    // Remain time(time)
    t = t_mineSessionTime_title;
    x = body->x + body->w / 4 - (*Text_edges(t->text))[DRAW_WIDTH] / 2;
    y = body->y;
    sprintf(
        t_mineSessionTime_buffer,
        (mineSession_paused)? T_MINESESSIONTIME_TITLE_SPRINTF_REMAIN_TIME_TO_START
        : T_MINESESSIONTIME_TITLE_SPRINTF_REMAIN_TIME_TO_END
    );
    Text_SetUTF8(t->text, t_mineSessionTime_buffer);
    MonitorText_Draw(t, x, y, scale);


    // Remain time(clock)
    t = t_mineSessionTime_clock;
    x =
        (*Text_edges(t_mineSessionTime_title->text))[DRAW_EDGE_XMAX]
        - (*Text_edges(t_mineSessionTime_title->text))[DRAW_WIDTH] / 2
        - (*Text_edges(t->text))[DRAW_WIDTH] / 2;
    y = (*Text_edges(t_mineSessionTime_title->text))[DRAW_EDGE_YMAX];
    sprintf(
        t_mineSessionTime_buffer,
        T_MINESESSIONTIME_CLOCK_SPRINTF,
        MINESESSION_DURATION_IN_HOURS - mineSession_modTime - 1,
        59 - Clock_min(gclock),
        59 - Clock_sec(gclock)
    );
    Text_SetUTF8(t->text, t_mineSessionTime_buffer);
    MonitorText_Draw(t, x, y, scale);
}


void AppMiner_handleKeyDown(const SDL_Event *e)
{
    SDL_Scancode key;

    key = e->key.keysym.sym;
    switch(key){
        case SDLK_RETURN:
            if(mineSession[mineSession_i] == NULL){
                break;
            }

            status = MathProblem_CheckAnswer(mineSession[mineSession_i], *Text_raw(ti_answer->text));
            break;

        default:
            break;
    }
}

void AppMiner_handlerButtonDown(const SDL_Event *e)
{
    int x, y;
    int (*edges)[DRAW_EDGE_AMT];
    int ti_answer_clicked;

    x = e->button.x;
    y = e->button.y;
    edges = Text_edges(ti_answer->text);
    ti_answer_clicked = 
        x >= (*edges)[DRAW_EDGE_XMIN] && x <= (*edges)[DRAW_EDGE_XMAX]
        && y >= (*edges)[DRAW_EDGE_YMIN] && y <= (*edges)[DRAW_EDGE_YMAX]
        ;

    if(ti_answer_clicked && !strcmp(*Text_raw(ti_answer->text), TI_ANSWER_PLACEHOLD)){
        Text_SetUTF8(ti_answer->text, NULL);
    }
    if(!ti_answer_clicked && !Text_GetSizeUTF8(ti_answer->text)){
        Text_SetUTF8(ti_answer->text, TI_ANSWER_PLACEHOLD);
    }
}

//
void AppMiner_Init(void)
{
    TextConfig *ti_answerConfig[INP_STATES_MAX] = 
    {
        [INP_STATE_NEUTRAL] = TextConfig_Create(
            .scale_width  = 1 / 3.0,
            .scale_height = 1 / 3.0,
            .color = (SDL_Color){0, 0, 0, 255},

            .border = (Border){
                .bottom = 5,
                .top    = 5,
                .right  = 5,
                .left   = 5,
                .color = (SDL_Color){0, 0, 0, 255}
            },
            .padding = (Padding){
                .bottom = 5,
                .top    = 5,
                .right  = 5,
                .left   = 5,
                .color = (SDL_Color){255, 255, 255, 255}
            },
            .color_bg = (SDL_Color){255, 255, 255, 255},
        ),
        [INP_STATE_SELECTED] = TextConfig_Create(
            .scale_width  = 1 / 3.0,
            .scale_height = 1 / 3.0,
            .color = (SDL_Color){0, 0, 0, 255},

            .border = (Border){
                .bottom = 8,
                .top    = 8,
                .right  = 8,
                .left   = 8,
                .color = (SDL_Color){0, 0, 0, 255}
            },
            .padding = (Padding){
                .bottom = 5,
                .top    = 5,
                .right  = 5,
                .left   = 5,
                .color = (SDL_Color){255, 255, 255, 255}
            },
            .color_bg = (SDL_Color){255, 255, 255, 255},
        ),
        [INP_STATE_HOVER] = TextConfig_Create(
            .scale_width  = 1 / 3.0,
            .scale_height = 1 / 3.0,
            .color = (SDL_Color){0, 0, 0, 255},

            .border = (Border){
                .bottom = 8,
                .top    = 8,
                .right  = 8,
                .left   = 8,
                .color = (SDL_Color){0, 0, 0, 255}
            },
            .padding = (Padding){
                .bottom = 5,
                .top    = 5,
                .right  = 5,
                .left   = 5,
                .color = (SDL_Color){255, 255, 255, 255}
            },
            .color_bg = (SDL_Color){255, 255, 255, 255},
        ),
    };

    t_question = MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 2.5,
            .scale_height = 1 / 2.5,
            .color = (SDL_Color){0, 0, 0, 255},
        ),
        MINESESSION_PAUSED
    );
    t_status = MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 2.5,
            .scale_height = 1 / 2.5,
            .color = (SDL_Color){0, 0, 0, 255},
            .chr_fmt = CHAR_FMT_BOLD,
        ),
        "Status:"
    );
    t_statusAnswer = MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 2.5,
            .scale_height = 1 / 2.5,
            .color = (SDL_Color){0, 255, 0, 255},
            .chr_fmt = CHAR_FMT_BOLD,
        ),
        STATUS_NOT_SUBMITED
    );

    ti_answer = TextInput_Create(
        cache[GFONT_REGULAR],
        &ti_answerConfig,
        5, 15,
        TI_ANSWER_PLACEHOLD
    );

    tip = MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 2.6,
            .scale_height = 1 / 2.6,
            .color = (SDL_Color){0, 0, 0, 255},
            .chr_fmt = CHAR_FMT_ITALIC,
        ),
        "Pressione enter para submeter"
    );
    t_mineSessionTime_title = MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 3.0,
            .scale_height = 1 / 3.0,
            .color = (SDL_Color){0, 0, 0, 255},
            .chr_fmt = CHAR_FMT_BOLD
        ),
        NULL
    );
    t_mineSessionTime_clock = MonitorText_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 3.0,
            .scale_height = 1 / 3.0,
            .color = (SDL_Color){0, 0, 0, 255},
        ),
        NULL
    );
}

void AppMiner_Quit(void)
{
    MonitorText_Destroy(&tip);
    MonitorText_Destroy(&t_status);
    //
    MonitorText_Destroy(&t_question);
    MonitorText_Destroy(&t_statusAnswer);
    TextInput_Destroy(&ti_answer);
    //
    MonitorText_Destroy(&t_mineSessionTime_title);
    MonitorText_Destroy(&t_mineSessionTime_clock);
}

void AppMiner_Draw(const SDL_Rect *body)
{
    if(body->w / (double)body->h >= 1){
        AppMiner_drawHorizontal(body);
    }
    else {
        AppMiner_drawVertical(body);
    }
}

void AppMiner_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside)
{
    if(status == SUBMIT_CORRECT){
        status = SUBMIT_NOT_SEND;
    }

    // printf("%i\n", SDL_GetTicks());
    switch(e->type){
        case SDL_KEYDOWN:
            AppMiner_handleKeyDown(e);
            break;

        case SDL_MOUSEBUTTONDOWN:
            AppMiner_handlerButtonDown(e);
            break;

        default:
            break;
    }

    if(
        (e->type == SDL_KEYDOWN && e->key.keysym.sym != SDLK_RETURN)
        || e->type != SDL_KEYDOWN
      ){
        TextInput_handleInput(ti_answer, e);
    }

    if(status == SUBMIT_CORRECT){
        Text_SetUTF8(ti_answer->text, NULL);
        mineSession_i++;
    }
}
