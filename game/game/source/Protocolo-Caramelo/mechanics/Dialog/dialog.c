#include "dialog.h"
#include "dialog_log.h"
#include "game/game_globals.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================== */
/* Estado                                                              */
/* ================================================================== */

typedef struct DialogRunner {
    /* roteiro */
    int active;
    const DialogLine *lines;
    size_t count;
    size_t index;
    void (*on_finish)(void);
    void (*on_char)(const DialogLine*, size_t);
    const char *archive_title;     /* != NULL: grava no DialogLog (Bate-boca) */

    /* opcoes do fim da cena */
    const DialogChoice *choices;
    size_t choice_count;
    int choosing;                  /* 1 = menu aberto */
    int sel;                       /* opcao selecionada */
    double menu_ms;                /* tempo desde que o menu abriu */
    Text *menu_text;
    char *menu_str;                /* rotulos separados por '\n' */
    int *menu_opt;                 /* opcao dona de cada letra visivel (sem '\n') */
    size_t menu_chars;
    double menu_slack_px;          /* lidos pelo func_alterText do menu */
    double menu_slot_px;

    /* fala atual */
    const DialogLine  *line;
    const DialogStyle *style;
    Text *text;
    unsigned base_lineskip;        /* lineskip da fonte, lido ao criar o Text */

    /* tempo */
    double reveal_ms;              /* relogio da aparicao das letras (pode "pular") */
    double age_ms;                 /* relogio livre: ondulacao e gelatina nunca param */
    double total_ms;               /* quando a ultima letra termina de aparecer */
    Uint32 last_ticks;
    int have_last;
    double jelly_energy;

    /* cronograma (por caractere do texto ORIGINAL) */
    double *orig_start;
    unsigned char *orig_blip;      /* 1 = letra que dispara o callback de som */
    size_t orig_count;
    size_t next_fire;

    /* texto quebrado em linhas (refeito se a janela mudar de tamanho) */
    char *wrapped;
    int wrap_w;
    double wrap_gs;
    double *vis_start;             /* inicio de cada letra VISIVEL (sem '\n') */
    size_t vis_count;

    /* lidos pelo func_alterText durante Text_DrawUTF8 */
    double ui_scale;
    double slack_px;

    /* clique que fechou o dialogo: o "soltar" do mouse tambem nao deve vazar */
    int swallow_mouse_up;
} DialogRunner;

static DialogRunner dlg;

/* Tempo depois de abrir o menu em que confirmacoes sao ignoradas: evita escolher
 * sem querer quando o jogador estava apertando "avancar" para a fala terminar. */
#define DIALOG_MENU_INPUT_LOCK_MS 250.0

/* Geometria do menu de opcoes, em pixels de tela (recalculada a cada quadro) */
typedef struct DialogMenuLayout {
    int valid;
    int x, y;                      /* canto da caixa (margem externa) */
    int outer_w, outer_h;
    int text_x, text_y;            /* canto da area de texto */
    int text_w, text_h;
    int slack, rise;
    int slot;                      /* espaco da setinha, a esquerda do texto */
    unsigned lineskip_unscaled;    /* lineskip passado ao Text (ja com row_gap) */
    int row_h;                     /* altura de cada opcao */
    int rows_top;                  /* y do topo da primeira opcao */
    int hit_x0, hit_x1;            /* faixa horizontal que responde ao mouse */
} DialogMenuLayout;

static DialogMenuLayout menu_L;

/* Medidas calculadas a cada quadro */
typedef struct DialogLayout {
    double s;                      /* escala janela / resolucao virtual */
    double glyph_scale;
    int x, y;                      /* canto da caixa (margem externa) */
    int m_t, m_r, m_b, m_l;
    int b_t, b_r, b_b, b_l;
    int pad_t, pad_r, pad_b, pad_l;    /* ja incluem o espaco do rosto */
    int text_w, text_h;
    int chr_dist;
    unsigned lineskip_unscaled;
    int inner_x, inner_y;          /* canto do preenchimento */
    int text_x, text_y;            /* canto da area de texto */
    SDL_FRect face;
} DialogLayout;

static int px(double virtual_px, double scale)
{
    return (int)lround(virtual_px * scale);
}

/* ================================================================== */
/* func_alterText: aparicao + ondulacao + gancho do estilo             */
/* ================================================================== */
static void Menu_alterChar(TextChar *c);

static void Dialog_alterChar(Text *t, TextChar *c)
{
    const DialogStyle *st = dlg.style;
    DialogCharInfo info;
    double start, progress, alpha;
    size_t idx;

    if(!dlg.active || !st || c->chr == '\n'){
        return;
    }
    if(t && t == dlg.menu_text){
        Menu_alterChar(c);
        return;
    }

    idx = c->chr_textIdx;
    start = (idx < dlg.vis_count)? dlg.vis_start[idx]: 0.0;
    progress = DialogFx_Reveal(dlg.reveal_ms, start, st->reveal.char_duration_ms, st->reveal.ease);
    alpha    = DialogFx_Alpha(dlg.reveal_ms, start, st->reveal.char_duration_ms);

    /* folga no topo: a onda e a subida nunca empurram a letra para fora da textura */
    c->y += dlg.slack_px;
    /* ondulacao continua (todas as letras, mesmo as ainda invisiveis) */
    c->y += DialogFx_Wave(&st->wave, dlg.age_ms, idx) * dlg.ui_scale;
    /* aparicao: sobe de baixo para cima enquanto ganha opacidade */
    c->y += (1.0 - progress) * st->reveal.rise_px * dlg.ui_scale;
    c->color.a = (Uint8)lround(c->color.a * alpha);

    if(st->alter_char){
        info.index    = idx;
        info.time_ms  = dlg.age_ms;
        info.reveal   = progress;
        info.ui_scale = dlg.ui_scale;
        info.option   = -1;
        info.selected = 0;
        st->alter_char(c, &info);
    }
}

/* ================================================================== */
/* Roteiro                                                             */
/* ================================================================== */
static void Runner_FreeLine(void)
{
    Text_Destroy(&dlg.text);
    free(dlg.orig_start);
    free(dlg.orig_blip);
    free(dlg.wrapped);
    free(dlg.vis_start);
    dlg.orig_start = NULL;
    dlg.orig_blip  = NULL;
    dlg.wrapped    = NULL;
    dlg.vis_start  = NULL;
    dlg.orig_count = dlg.vis_count = dlg.next_fire = 0;
    dlg.wrap_w = -1;
    dlg.line  = NULL;
    dlg.style = NULL;
}

static void Runner_FreeMenu(void)
{
    Text_Destroy(&dlg.menu_text);
    free(dlg.menu_str);
    free(dlg.menu_opt);
    dlg.menu_str   = NULL;
    dlg.menu_opt   = NULL;
    dlg.menu_chars = 0;
    dlg.choosing   = 0;
    dlg.sel        = 0;
    dlg.menu_ms    = 0.0;
    menu_L.valid   = 0;
}

/* Desliga tudo (menos o callback de letras, que e configuracao do jogo) */
static void Runner_Clear(void)
{
    Runner_FreeLine();
    Runner_FreeMenu();
    dlg.active        = 0;
    dlg.lines         = NULL;
    dlg.count         = 0;
    dlg.on_finish     = NULL;
    dlg.archive_title = NULL;
    dlg.choices       = NULL;
    dlg.choice_count  = 0;
}

static void Runner_BeginLine(size_t index)
{
    const char *txt;
    const char *cursor;
    size_t i;
    int cp;

    Runner_FreeLine();

    dlg.index = index;
    dlg.line  = &dlg.lines[index];
    dlg.style = dlg.line->style? dlg.line->style: &dialog_styleDefault;
    txt = dlg.line->text? dlg.line->text: "";

    dlg.orig_count = DialogFx_CountChars(txt);
    dlg.orig_start = calloc(MAX(dlg.orig_count, 1), sizeof(double));
    dlg.orig_blip  = calloc(MAX(dlg.orig_count, 1), sizeof(unsigned char));
    if(!dlg.orig_start || !dlg.orig_blip){
        free(dlg.orig_start);
        free(dlg.orig_blip);
        dlg.orig_start = NULL;
        dlg.orig_blip  = NULL;
        dlg.orig_count = 0;
    }

    dlg.total_ms = 0.0;
    DialogFx_BuildSchedule(txt, &dlg.style->reveal, dlg.orig_start, dlg.orig_count, &dlg.total_ms);

    cursor = txt;
    for(i = 0; i < dlg.orig_count; ++i){
        cp = DialogFx_NextCodepoint(&cursor);
        dlg.orig_blip[i] = (cp != ' ' && cp != '\n' && cp != '\t');
    }

    dlg.reveal_ms = 0.0;
    dlg.age_ms    = 0.0;
    dlg.next_fire = 0;

    /* historico do Bate-boca: a fala e gravada quando comeca a aparecer */
    if(dlg.archive_title){
        DialogLog_AddSpeech(dlg.archive_title, (int)dlg.line->speaker, (int)dlg.line->state, txt);
    }

    /* por ultimo: o callback pode chamar Dialog_Start */
    if(dlg.line->on_show){
        dlg.line->on_show();
    }
}

static void Runner_Finish(void)
{
    void (*finish)(void) = dlg.on_finish;

    Runner_Clear();
    if(finish){
        finish();
    }
}

void Dialog_Init(void)
{
    memset(&dlg, 0, sizeof(dlg));
    dlg.wrap_w = -1;
    DialogStyles_Init();
}

void Dialog_Quit(void)
{
    Runner_Clear();
    memset(&dlg, 0, sizeof(dlg));
    DialogLog_Reset();
}

void Dialog_PlayScene(const DialogScene *scene)
{
    void (*on_char)(const DialogLine*, size_t) = dlg.on_char;

    if(!scene || !scene->lines || scene->count == 0){
        return;
    }
    Runner_Clear();

    dlg.on_char       = on_char;
    dlg.active        = 1;
    dlg.lines         = scene->lines;
    dlg.count         = scene->count;
    dlg.on_finish     = scene->on_finish;
    dlg.archive_title = scene->archive_title;
    if(scene->choices && scene->choice_count > 0){
        dlg.choices      = scene->choices;
        dlg.choice_count = scene->choice_count;
    }
    dlg.have_last = 0;
    Runner_BeginLine(0);
}

void Dialog_Start(const DialogLine *lines, size_t count, void (*on_finish)(void))
{
    DialogScene scene;

    memset(&scene, 0, sizeof(scene));
    scene.lines     = lines;
    scene.count     = count;
    scene.on_finish = on_finish;
    Dialog_PlayScene(&scene);       /* o motor copia os campos: `scene` pode ser local */
}

int Dialog_IsActive(void)
{
    return dlg.active;
}

int Dialog_IsChoosing(void)
{
    return dlg.active && dlg.choosing;
}

/* A cena tem opcoes e esta na ultima fala? */
static int Runner_onChoiceLine(void)
{
    return dlg.choice_count > 0 && dlg.index + 1 == dlg.count;
}

static void Runner_OpenMenu(void)
{
    size_t i, total, n;
    const char *label;
    const char *cursor;
    char *out;
    size_t chars;
    int *opt;

    /* texto do menu: rotulos separados por '\n' */
    total = 1;
    for(i = 0; i < dlg.choice_count; ++i){
        label = dlg.choices[i].label? dlg.choices[i].label: "";
        total += strlen(label) + 1;
    }
    out = malloc(total);
    opt = calloc(total, sizeof(int));          /* folga: no maximo `total` letras */
    if(!out || !opt){
        free(out);
        free(opt);
        return;
    }

    out[0] = '\0';
    chars = 0;
    for(i = 0; i < dlg.choice_count; ++i){
        label = dlg.choices[i].label? dlg.choices[i].label: "";
        if(i > 0){
            strcat(out, "\n");
        }
        strcat(out, label);
        cursor = label;
        n = DialogFx_CountChars(label);
        while(n-- > 0){
            (void)DialogFx_NextCodepoint(&cursor);
            opt[chars++] = (int)i;
        }
    }

    Runner_FreeMenu();
    dlg.menu_str   = out;
    dlg.menu_opt   = opt;
    dlg.menu_chars = chars;
    dlg.choosing   = 1;
    dlg.sel        = 0;
    dlg.menu_ms    = 0.0;
}

/* Confirma a opcao `i`: grava no historico, encerra a cena e segue o fluxo. */
static void Runner_Choose(int i)
{
    const DialogChoice *choice;
    void (*selected)(void);
    void (*finish)(void);
    const DialogScene *next;

    if(i < 0 || (size_t)i >= dlg.choice_count){
        return;
    }
    choice   = &dlg.choices[i];
    selected = choice->on_select;
    next     = choice->next;
    finish   = dlg.on_finish;

    if(dlg.archive_title){
        DialogLog_AddChoice(dlg.archive_title, (int)DIALOG_PLAYER, choice->label);
    }

    Runner_Clear();
    if(selected){
        selected();
    }
    if(finish){
        finish();
    }
    if(next){
        Dialog_PlayScene(next);
    }
}

void Dialog_Next(void)
{
    if(!dlg.active){
        return;
    }
    if(dlg.choosing){
        if(dlg.menu_ms >= DIALOG_MENU_INPUT_LOCK_MS){
            Runner_Choose(dlg.sel);
        }
        return;
    }
    if(dlg.reveal_ms < dlg.total_ms){
        /* ainda falando: mostra tudo de uma vez */
        dlg.reveal_ms = dlg.total_ms;
        dlg.next_fire = dlg.orig_count;
        return;
    }
    if(dlg.index + 1 < dlg.count){
        Runner_BeginLine(dlg.index + 1);
    }
    else if(Runner_onChoiceLine()){
        Runner_OpenMenu();                 /* sem esperar o atraso do estilo */
    }
    else {
        Runner_Finish();
    }
}

void Dialog_Abort(void)
{
    Runner_Clear();
}

void Dialog_SetCharCallback(void (*fn)(const DialogLine*, size_t))
{
    dlg.on_char = fn;
}

const DialogStyle *Dialog_CurrentStyle(void)
{
    return dlg.active? dlg.style: NULL;
}

/* ================================================================== */
/* Entrada                                                             */
/* ================================================================== */
static int Runner_isConfirmKey(SDL_Keycode key)
{
    return key == SDLK_z || key == SDLK_RETURN || key == SDLK_KP_ENTER || key == SDLK_SPACE;
}

/* Opcao do menu sob o ponto (x, y) da tela, ou -1 */
static int Runner_menuRowAt(int x, int y)
{
    if(!menu_L.valid || x < menu_L.hit_x0 || x >= menu_L.hit_x1){
        return -1;
    }
    return DialogFx_MenuRowAt(y, menu_L.rows_top, menu_L.row_h, (int)dlg.choice_count);
}

static int Runner_handleMenuInput(SDL_Event *e)
{
    int row;

    switch(e->type){
        case SDL_KEYDOWN:
            if(Runner_isConfirmKey(e->key.keysym.sym)){
                if(!e->key.repeat){
                    Dialog_Next();
                }
                return 1;
            }
            switch(e->key.keysym.sym){
                case SDLK_UP:
                case SDLK_w:
                    dlg.sel = DialogFx_MenuStep(dlg.sel, (int)dlg.choice_count, -1);
                    return 1;
                case SDLK_DOWN:
                case SDLK_s:
                    dlg.sel = DialogFx_MenuStep(dlg.sel, (int)dlg.choice_count, +1);
                    return 1;
                default:
                    return 0;
            }

        case SDL_MOUSEMOTION:
            row = Runner_menuRowAt(e->motion.x, e->motion.y);
            if(row >= 0){
                dlg.sel = row;
            }
            return 0;                       /* o jogo continua vendo o mouse mexer */

        case SDL_MOUSEBUTTONDOWN:
            if(e->button.button == SDL_BUTTON_LEFT){
                row = Runner_menuRowAt(e->button.x, e->button.y);
                if(row >= 0){
                    dlg.sel = row;
                    Dialog_Next();          /* confirma (respeita a trava de abertura) */
                }
                dlg.swallow_mouse_up = 1;
            }
            return 1;

        case SDL_MOUSEBUTTONUP:
            return 1;

        default:
            return 0;
    }
}

int Dialog_HandleInput(SDL_Event *e)
{
    if(e->type == SDL_MOUSEBUTTONUP && dlg.swallow_mouse_up){
        dlg.swallow_mouse_up = 0;
        return 1;
    }
    if(!dlg.active){
        return 0;
    }
    if(dlg.choosing){
        return Runner_handleMenuInput(e);
    }

    switch(e->type){
        case SDL_KEYDOWN:
            if(Runner_isConfirmKey(e->key.keysym.sym)){
                if(!e->key.repeat){
                    Dialog_Next();
                }
                return 1;
            }
            return 0;

        case SDL_MOUSEBUTTONDOWN:
            if(e->button.button == SDL_BUTTON_LEFT){
                Dialog_Next();
                dlg.swallow_mouse_up = 1;
            }
            return 1;

        case SDL_MOUSEBUTTONUP:
            return 1;

        default:
            return 0;
    }
}

/* ================================================================== */
/* Atualizacao                                                         */
/* ================================================================== */
static void Runner_Update(double dt_ms)
{
    int speaking;

    dlg.age_ms    += dt_ms;
    dlg.reveal_ms += dt_ms;
    speaking = dlg.reveal_ms < dlg.total_ms;

    dlg.jelly_energy = DialogFx_JellyEnergy(&dlg.style->face.jelly, dlg.jelly_energy, speaking, dt_ms);

    /* menu de opcoes: abre sozinho um instante depois da ultima fala terminar */
    if(Runner_onChoiceLine() && !dlg.choosing
        && dlg.reveal_ms >= dlg.total_ms + dlg.style->choice.delay_ms){
        Runner_OpenMenu();
    }
    if(dlg.choosing){
        dlg.menu_ms += dt_ms;
    }

    /* som/efeito a cada letra que comeca a aparecer */
    while(dlg.next_fire < dlg.orig_count && dlg.orig_start[dlg.next_fire] <= dlg.reveal_ms){
        if(dlg.on_char && dlg.orig_blip[dlg.next_fire]){
            dlg.on_char(dlg.line, dlg.next_fire);
        }
        ++dlg.next_fire;
    }
}

/* ================================================================== */
/* Layout                                                              */
/* ================================================================== */
static SDL_Texture *Runner_faceTexture(void)
{
    if((int)dlg.line->speaker < 0 || dlg.line->speaker >= C_COUNT
        || (int)dlg.line->state < 0 || dlg.line->state >= CS_COUNT){
        return NULL;
    }
    return chars_tex[dlg.line->speaker][dlg.line->state];
}

static void Runner_computeLayout(DialogLayout *L, int win_w, int win_h)
{
    const DialogStyle *st = dlg.style;
    SDL_Texture *face_tex = Runner_faceTexture();
    int tw = 3, th = 4;                 /* proporcao padrao dos rostos: 3:4 */
    double ox, oy;
    int outer_w, slot, inner_h, face_h, face_w;
    int slack, rise;
    int has_face = (st->face.side != DIALOG_FACE_NONE);
    double lineskip;

    L->s = MIN(win_w / (double)GCORE_WINDOW_W, win_h / (double)GCORE_WINDOW_H);
    ox = (win_w - GCORE_WINDOW_W * L->s) / 2.0;     /* mesma "letterbox" do fundo do jogo */
    oy = (win_h - GCORE_WINDOW_H * L->s) / 2.0;

    L->glyph_scale = st->text.scale * L->s;
    L->chr_dist    = px(st->text.chr_dist, L->s);

    L->m_t = px(st->box.margin.top, L->s);    L->m_r = px(st->box.margin.right, L->s);
    L->m_b = px(st->box.margin.bottom, L->s); L->m_l = px(st->box.margin.left, L->s);
    L->b_t = px(st->box.border.top, L->s);    L->b_r = px(st->box.border.right, L->s);
    L->b_b = px(st->box.border.bottom, L->s); L->b_l = px(st->box.border.left, L->s);
    L->pad_t = px(st->box.padding.top, L->s);    L->pad_r = px(st->box.padding.right, L->s);
    L->pad_b = px(st->box.padding.bottom, L->s); L->pad_l = px(st->box.padding.left, L->s);

    /* altura da area de texto: linhas + folga para onda/aparicao */
    L->lineskip_unscaled = st->text.lineskip? st->text.lineskip: dlg.base_lineskip;
    lineskip = (int)(L->lineskip_unscaled * L->glyph_scale);      /* igual ao calculo da libgraphical */
    slack = (int)ceil(
        (st->wave.amplitude_px + st->box.text_slack + 0.15 * st->reveal.rise_px) * L->s
    );
    rise = (int)ceil(st->reveal.rise_px * L->s);
    L->text_h = (int)lineskip * MAX(st->box.lines, 1) + 2 * slack + rise;
    dlg.slack_px = slack;
    dlg.ui_scale = L->s;

    /* rosto: ocupa a altura interna da caixa, com a proporcao da imagem */
    inner_h = L->pad_t + L->text_h + L->pad_b;
    face_h = MAX(inner_h - 2 * px(st->face.inset, L->s), 1);
    if(face_tex){
        SDL_QueryTexture(face_tex, NULL, NULL, &tw, &th);
    }
    face_w = (int)lround(face_h * (double)tw / (double)th);
    slot = has_face? face_w + px(st->face.gap, L->s): 0;

    if(st->face.side == DIALOG_FACE_LEFT){
        L->pad_l += slot;
    }
    else if(st->face.side == DIALOG_FACE_RIGHT){
        L->pad_r += slot;
    }

    outer_w = px(GCORE_WINDOW_W - 2 * st->box.screen_margin, L->s);
    L->text_w = MAX(
        outer_w - (L->m_l + L->m_r + L->b_l + L->b_r + L->pad_l + L->pad_r),
        20
    );

    L->x = (int)lround(ox) + px(st->box.screen_margin, L->s);
    L->y = (int)lround(oy) + px(GCORE_WINDOW_H, L->s) - px(st->box.bottom_gap, L->s)
         - (L->m_t + L->m_b + L->b_t + L->b_b + inner_h);

    L->inner_x = L->x + L->m_l + L->b_l;
    L->inner_y = L->y + L->m_t + L->b_t;
    L->text_x  = L->inner_x + L->pad_l;
    L->text_y  = L->inner_y + L->pad_t;

    L->face.y = (float)(L->inner_y + px(st->face.inset, L->s));
    L->face.w = (float)face_w;
    L->face.h = (float)face_h;
    if(st->face.side == DIALOG_FACE_LEFT){
        L->face.x = (float)(L->inner_x + px(st->box.padding.left, L->s));
    }
    else {
        L->face.x = (float)(L->text_x + L->text_w + px(st->face.gap, L->s));
    }
}

/* ================================================================== */
/* Quebra de linha                                                     */
/* ================================================================== */
typedef struct DialogWidthCtx {
    TextCache *font_cache;
    TextCharFormat fmt;
    double glyph_scale;
} DialogWidthCtx;

static int Dialog_glyphWidth(void *ctx, int codepoint)
{
    DialogWidthCtx *c = ctx;
    int gw = 0, gh = 0;

    TextCache_GetGlyphSize(c->font_cache, codepoint, c->fmt, &gw, &gh);
    return (int)(gw * c->glyph_scale);      /* trunca como a libgraphical */
}

static void Runner_ensureWrap(const DialogLayout *L)
{
    DialogWidthCtx ctx;
    const char *txt = dlg.line->text? dlg.line->text: "";
    const char *cursor;
    size_t i, v;
    int cp;

    if(dlg.wrapped && dlg.wrap_w == L->text_w && fabs(dlg.wrap_gs - L->glyph_scale) < 1e-9){
        return;
    }

    free(dlg.wrapped);
    free(dlg.vis_start);

    ctx.font_cache  = cache[dlg.style->text.font];
    ctx.fmt         = dlg.style->text.fmt;
    ctx.glyph_scale = L->glyph_scale;
    dlg.wrapped = DialogFx_WordWrap(txt, L->text_w, L->chr_dist, Dialog_glyphWidth, &ctx);
    dlg.vis_start = calloc(MAX(dlg.orig_count, 1), sizeof(double));

    /* a libgraphical nao conta '\n' no indice das letras: remonta o mapa de tempos */
    v = 0;
    i = 0;
    cursor = dlg.wrapped;
    while(cursor && *cursor && dlg.vis_start){
        cp = DialogFx_NextCodepoint(&cursor);
        if(cp != '\n' && i < dlg.orig_count){
            dlg.vis_start[v++] = dlg.orig_start[i];
        }
        ++i;
    }
    dlg.vis_count = v;
    dlg.wrap_w  = L->text_w;
    dlg.wrap_gs = L->glyph_scale;
}

/* ================================================================== */
/* Menu de opcoes                                                      */
/* ================================================================== */

/* func_alterText do menu: cada opcao sobe em sequencia ao abrir; so a
 * selecionada ondula e ganha a cor de destaque. */
static void Menu_alterChar(TextChar *c)
{
    const DialogChoiceStyle *cs = &dlg.style->choice;
    DialogCharInfo info;
    double start, progress, alpha;
    size_t idx = c->chr_textIdx;
    int option, selected;

    option = (idx < dlg.menu_chars)? dlg.menu_opt[idx]: -1;
    if(option < 0){
        return;
    }
    selected = (option == dlg.sel);

    start    = DialogFx_MenuStart(option, cs->stagger_ms);
    progress = DialogFx_Reveal(dlg.menu_ms, start, cs->duration_ms, dlg.style->reveal.ease);
    alpha    = DialogFx_Alpha(dlg.menu_ms, start, cs->duration_ms);

    c->x += dlg.menu_slot_px;                         /* espaco da setinha */
    c->y += dlg.menu_slack_px;                        /* folga para onda e subida */
    c->y += (1.0 - progress) * cs->rise_px * dlg.ui_scale;
    if(selected){
        c->y += DialogFx_Wave(&cs->selected_wave, dlg.menu_ms, idx) * dlg.ui_scale;
    }
    c->color = selected? cs->selected_color: cs->idle_color;
    c->color.a = (Uint8)lround(c->color.a * alpha);

    if(cs->alter_char){
        info.index    = idx;
        info.time_ms  = dlg.menu_ms;
        info.reveal   = progress;
        info.ui_scale = dlg.ui_scale;
        info.option   = option;
        info.selected = selected;
        cs->alter_char(c, &info);
    }
}

/* Largura (px) de uma linha de texto com a fonte e escala do estilo */
static int Menu_labelWidth(const char *label, const DialogLayout *L)
{
    DialogWidthCtx ctx;
    const char *cursor = label? label: "";
    int cp, width = 0, n = 0;

    ctx.font_cache  = cache[dlg.style->text.font];
    ctx.fmt         = dlg.style->text.fmt;
    ctx.glyph_scale = L->glyph_scale;
    while(*cursor){
        cp = DialogFx_NextCodepoint(&cursor);
        width += Dialog_glyphWidth(&ctx, cp);
        if(n++ > 0){
            width += L->chr_dist;
        }
    }
    return width;
}

static void Menu_computeLayout(const DialogLayout *L, DialogMenuLayout *M)
{
    const DialogStyle *st = dlg.style;
    const DialogChoiceStyle *cs = &st->choice;
    int pad_t, pad_r, pad_b, pad_l;
    int label_w, widest, max_text_w, outer_w_dialog, dec_w, dec_h, bottom;
    size_t i;
    double row_extra;

    pad_t = px(st->box.padding.top, L->s);    pad_r = px(st->box.padding.right, L->s);
    pad_b = px(st->box.padding.bottom, L->s); pad_l = px(st->box.padding.left, L->s);

    /* linhas: lineskip da fonte + espaco extra entre opcoes (em unidades do glifo) */
    row_extra = px(cs->row_gap, L->s) / L->glyph_scale;
    M->lineskip_unscaled = L->lineskip_unscaled + (unsigned)MAX(lround(row_extra), 0L);
    M->row_h = (int)(M->lineskip_unscaled * L->glyph_scale);      /* igual a libgraphical */

    M->slack = (int)ceil((cs->selected_wave.amplitude_px + 0.15 * cs->rise_px) * L->s);
    M->rise  = (int)ceil(cs->rise_px * L->s);
    M->slot  = (cs->marker_size > 0)? px(cs->marker_size * 1.4 + cs->marker_gap, L->s): 0;

    widest = 0;
    for(i = 0; i < dlg.choice_count; ++i){
        label_w = Menu_labelWidth(dlg.choices[i].label, L);
        widest = MAX(widest, label_w);
    }
    dec_w = L->m_l + L->m_r + L->b_l + L->b_r + pad_l + pad_r;
    dec_h = L->m_t + L->m_b + L->b_t + L->b_b + pad_t + pad_b;
    outer_w_dialog = px(GCORE_WINDOW_W - 2 * st->box.screen_margin, L->s);
    max_text_w = MAX(outer_w_dialog - dec_w, 20);

    M->text_w = MIN(MAX(widest + M->slot + px(8, L->s), px(cs->min_width, L->s) - dec_w), max_text_w);
    M->text_h = M->row_h * (int)dlg.choice_count + 2 * M->slack + M->rise;

    M->outer_w = dec_w + M->text_w;
    M->outer_h = dec_h + M->text_h;

    /* a caixa de fala cresce para baixo da posicao L->y: o menu fica logo acima */
    bottom = L->y - px(cs->gap, L->s);
    M->y = bottom - M->outer_h;
    if(cs->align == DIALOG_CHOICE_ALIGN_RIGHT){
        M->x = L->x + outer_w_dialog - M->outer_w;
    }
    else {
        M->x = L->x;
    }

    M->text_x   = M->x + L->m_l + L->b_l + pad_l;
    M->text_y   = M->y + L->m_t + L->b_t + pad_t;
    M->rows_top = M->text_y + M->slack;
    M->hit_x0   = M->x + L->m_l + L->b_l;
    M->hit_x1   = M->x + M->outer_w - L->m_r - L->b_r;
    M->valid    = 1;

    dlg.menu_slack_px = M->slack;
    dlg.menu_slot_px  = M->slot;
}

static int Runner_ensureMenuText(void)
{
    if(dlg.menu_text){
        return 1;
    }
    dlg.menu_text = Text_Create(cache[dlg.style->text.font]);
    return dlg.menu_text != NULL;
}

/* Setinha ao lado da opcao selecionada (triangulo apontando para a direita) */
static void Menu_drawMarker(const DialogLayout *L, const DialogMenuLayout *M)
{
    const DialogChoiceStyle *cs = &dlg.style->choice;
    SDL_BlendMode old_blend;
    Uint8 old[4];
    double start, alpha, bounce;
    int size, width, i, half, cx, cy;

    size = px(cs->marker_size, L->s);
    if(size <= 0){
        return;
    }
    start = DialogFx_MenuStart(dlg.sel, cs->stagger_ms);
    alpha = DialogFx_Alpha(dlg.menu_ms, start, cs->duration_ms);
    if(alpha <= 0.0){
        return;
    }

    bounce = fabs(sin(dlg.menu_ms * 0.007)) * cs->marker_bounce_px * L->s;
    width  = (int)lround(size * 1.4);
    cx = M->text_x + (int)bounce;
    cy = M->rows_top + dlg.sel * M->row_h + M->row_h / 2;

    SDL_GetRenderDrawColor(r, &old[0], &old[1], &old[2], &old[3]);
    SDL_GetRenderDrawBlendMode(r, &old_blend);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, cs->marker_color.r, cs->marker_color.g, cs->marker_color.b,
        (Uint8)lround(cs->marker_color.a * alpha));
    for(i = 0; i < width; ++i){
        half = size - (int)lround((double)size * i / width);     /* afina em direcao a ponta */
        SDL_RenderDrawLine(r, cx + i, cy - half, cx + i, cy + half);
    }
    SDL_SetRenderDrawBlendMode(r, old_blend);
    SDL_SetRenderDrawColor(r, old[0], old[1], old[2], old[3]);
}

static void Menu_draw(const DialogLayout *L)
{
    const DialogStyle *st = dlg.style;
    DialogMenuLayout *M = &menu_L;

    if(!dlg.choosing || !dlg.menu_str || !Runner_ensureMenuText()){
        return;
    }
    Menu_computeLayout(L, M);

    Text_DrawUTF8(
        dlg.menu_text,
        dlg.menu_str,
        TextConfig_Create(
            .x = M->x,
            .y = M->y,
            .color = st->choice.idle_color,
            .color_bg = st->box.text_bg,
            .lineskip = M->lineskip_unscaled,
            .wrap = 0,
            .bx_width = M->text_w,
            .bx_height = M->text_h,
            .scale_width = L->glyph_scale,
            .scale_height = L->glyph_scale,
            .chr_dist = L->chr_dist,
            .chr_fmt = st->text.fmt,
            .margin = (Margin){
                .top = L->m_t, .right = L->m_r, .bottom = L->m_b, .left = L->m_l,
                .color = st->box.margin.color
            },
            .border = (Border){
                .top = L->b_t, .right = L->b_r, .bottom = L->b_b, .left = L->b_l,
                .color = st->box.border.color
            },
            .padding = (Padding){
                .top = px(st->box.padding.top, L->s),    .right = px(st->box.padding.right, L->s),
                .bottom = px(st->box.padding.bottom, L->s), .left = px(st->box.padding.left, L->s),
                .color = st->box.padding.color
            },
            .func_alterMargin  = st->box.alterMargin,
            .func_alterBorder  = st->box.alterBorder,
            .func_alterPadding = st->box.alterPadding,
            .func_alterText    = Dialog_alterChar
        ),
        NULL
    );
    Menu_drawMarker(L, M);
}

/* ================================================================== */
/* Desenho                                                             */
/* ================================================================== */
static int Runner_ensureText(void)
{
    if(dlg.text){
        return 1;
    }
    dlg.text = Text_Create(cache[dlg.style->text.font]);
    if(!dlg.text){
        return 0;
    }
    dlg.base_lineskip = *Text_lineskip(dlg.text);
    return 1;
}

static void Runner_drawBox(const DialogLayout *L)
{
    const DialogStyle *st = dlg.style;

    Text_DrawUTF8(
        dlg.text,
        dlg.wrapped,
        TextConfig_Create(
            .x = L->x,
            .y = L->y,
            .color = st->text.color,
            .color_bg = st->box.text_bg,
            .lineskip = L->lineskip_unscaled,
            .wrap = L->text_w,
            .bx_width = L->text_w,
            .bx_height = L->text_h,
            .scale_width = L->glyph_scale,
            .scale_height = L->glyph_scale,
            .chr_dist = L->chr_dist,
            .chr_fmt = st->text.fmt,
            .margin = (Margin){
                .top = L->m_t, .right = L->m_r, .bottom = L->m_b, .left = L->m_l,
                .color = st->box.margin.color
            },
            .border = (Border){
                .top = L->b_t, .right = L->b_r, .bottom = L->b_b, .left = L->b_l,
                .color = st->box.border.color
            },
            .padding = (Padding){
                .top = L->pad_t, .right = L->pad_r, .bottom = L->pad_b, .left = L->pad_l,
                .color = st->box.padding.color
            },
            .func_alterMargin  = st->box.alterMargin,
            .func_alterBorder  = st->box.alterBorder,
            .func_alterPadding = st->box.alterPadding,
            .func_alterText    = Dialog_alterChar
        ),
        NULL
    );
}

/* Gelatina: a imagem e fatiada em faixas horizontais; cada faixa balanca um
 * pouco diferente e o conjunto estica/achata ancorado na base. */
static void Runner_drawJelly(SDL_Texture *tex, SDL_FRect rect, const DialogJellyStyle *j, double s)
{
    SDL_Rect src;
    SDL_FRect dst;
    int tw, th, n, k;
    double sx, sy, dest_w, dest_h, x0, y_top, y_bottom, dx;

    SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
    n = j->strips;
    n = (n < 1)? 1: (n > th)? th: (n > 128)? 128: n;

    DialogFx_JellyScale(j, dlg.jelly_energy, dlg.age_ms, &sx, &sy);
    dest_w   = rect.w * sx;
    dest_h   = rect.h * sy;
    x0       = rect.x + rect.w / 2.0 - dest_w / 2.0;
    y_bottom = rect.y + rect.h;
    y_top    = y_bottom - dest_h;

    for(k = 0; k < n; ++k){
        dx = DialogFx_JellySway(j, dlg.jelly_energy, dlg.age_ms, (k + 0.5) / n) * s;

        src.x = 0;
        src.w = tw;
        src.y = th * k / n;
        src.h = th * (k + 1) / n - src.y;

        dst.x = (float)(x0 + dx);
        dst.y = (float)(y_top + dest_h * k / n);
        dst.w = (float)dest_w;
        dst.h = (float)(dest_h / n + 1.0);   /* +1px: sem frestas entre faixas */

        SDL_RenderCopyF(r, tex, &src, &dst);
    }
}

static void Runner_drawFace(const DialogLayout *L)
{
    SDL_Texture *tex;

    if(dlg.style->face.side == DIALOG_FACE_NONE){
        return;
    }
    tex = Runner_faceTexture();
    if(!tex){
        return;
    }
    Runner_drawJelly(tex, L->face, &dlg.style->face.jelly, L->s);
}

/* Setinha que quica quando a fala terminou */
static void Runner_drawPrompt(const DialogLayout *L)
{
    const DialogPromptStyle *p = &dlg.style->prompt;
    Uint8 old[4];
    int size, i, half, cx, cy;
    double bounce;

    size = px(p->size, L->s);
    if(size <= 0 || dlg.reveal_ms < dlg.total_ms || Runner_onChoiceLine()){
        return;                 /* na ultima fala com opcoes quem convida e o menu */
    }

    bounce = fabs(sin(dlg.age_ms * 0.006)) * p->bounce_px * L->s;
    cx = L->text_x + L->text_w - size;
    cy = L->text_y + L->text_h - 2 * size - (int)bounce;

    SDL_GetRenderDrawColor(r, &old[0], &old[1], &old[2], &old[3]);
    SDL_SetRenderDrawColor(r, p->color.r, p->color.g, p->color.b, p->color.a);
    for(i = 0; i < size; ++i){
        half = size - i;                         /* triangulo apontando para baixo */
        SDL_RenderDrawLine(r, cx - half, cy + i, cx + half, cy + i);
    }
    SDL_SetRenderDrawColor(r, old[0], old[1], old[2], old[3]);
}

void Dialog_Render(void)
{
    DialogLayout L;
    Uint32 now;
    double dt;
    int win_w, win_h;

    if(!dlg.active || !dlg.line){
        return;
    }
    if(game_playState == GAME_PLAY_STATE_PAUSED){
        dlg.have_last = 0;                  /* o tempo nao anda com o jogo pausado */
        return;
    }
    if(!Runner_ensureText()){
        return;
    }

    now = SDL_GetTicks();
    dt = dlg.have_last? (double)(now - dlg.last_ticks): 0.0;
    dt = (dt > 50.0)? 50.0: dt;             /* um engasgo nao pode "pular" a animacao */
    dlg.last_ticks = now;
    dlg.have_last = 1;

    SDL_GetWindowSize(w, &win_w, &win_h);

    Runner_Update(dt);
    Runner_computeLayout(&L, win_w, win_h);
    Runner_ensureWrap(&L);

    Runner_drawBox(&L);
    Runner_drawFace(&L);
    Runner_drawPrompt(&L);
    Menu_draw(&L);
}
