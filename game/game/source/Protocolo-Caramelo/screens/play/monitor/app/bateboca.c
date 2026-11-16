#include"bateboca.h"
#include"mechanics/mechanics.h"

/*
 * Layout, em unidades do monitor (veja monitor_space.h).
 * Para mudar o visual do app mexa so nesta parte e nas cores logo abaixo.
 */
#define LIST_W          340     // coluna das conversas
#define LIST_PAD        12
#define LIST_ROW_H      92
#define LIST_ROW_GAP    8
#define LIST_AVATAR     64
#define UNREAD_DOT      20
#define HEADER_H        76      // titulo da conversa aberta
#define PANE_PAD        22
#define MSG_GAP         18      // entre uma mensagem e a seguinte
#define AVATAR          72
#define AVATAR_GAP      14
#define BUBBLE_PAD_X    18
#define BUBBLE_PAD_Y    12
#define BUBBLE_MAX_PCT  78      // largura maxima do texto, em % do espaco disponivel
#define WHEEL_STEP      90      // quanto rola cada "clique" da roda
#define SCROLLBAR_W     10
#define TEXT_SCALE      (1 / 4.0)

// Parte da imagem do personagem usada como foto: um quadrado da largura inteira
// que comeca a esta fracao da altura (0 = topo). Ajuste quando a arte final chegar.
#define AVATAR_CROP_Y_FRAC 0.0

static const SDL_Color COLOR_LINE       = {  0,   0,   0, 255};
static const SDL_Color COLOR_LIST_BG    = {232, 232, 238, 255};
static const SDL_Color COLOR_PANE_BG    = {250, 250, 252, 255};
static const SDL_Color COLOR_HEADER_BG  = {214, 220, 238, 255};
static const SDL_Color COLOR_ROW        = {255, 255, 255, 255};
static const SDL_Color COLOR_ROW_SELECT = {255, 222, 130, 255};
static const SDL_Color COLOR_HOVER      = {255, 180,  27,  70};
static const SDL_Color COLOR_UNREAD     = {255, 120,  30, 255};
static const SDL_Color COLOR_OTHER      = {255, 255, 255, 255};
static const SDL_Color COLOR_PLAYER     = {196, 222, 255, 255};
static const SDL_Color COLOR_CHOICE     = {255, 232, 172, 255};
static const SDL_Color COLOR_AVATAR_BG  = {200, 200, 208, 255};
static const SDL_Color COLOR_SCROLL     = {  0,   0,   0, 110};
static const SDL_Color TEXT_BLACK       = {  0,   0,   0, 255};
static const SDL_Color TEXT_NOTE        = { 90,  90,  90, 255};
static const SDL_Color TEXT_NAME        = { 40,  60, 140, 255};

static const char *const character_names[C_COUNT] = {
    [C_PEDRO]        = "Pedro",
    [C_FELIPE]       = "Felipe",
    [C_ANA]          = "Ana",
    [C_JOSE]         = "José",
    [C_RICARDO]      = "Ricardo",
    [C_DONODOPREDIO] = "Dono do prédio",
    [C_INSPETOR]     = "Inspetor",
};
#define PLAYER_NAME_IN_CHOICE "Você"

//
// Estado
//
// Cache por mensagem: texto ja quebrado em linhas + medidas + o Text que o desenha.
// Cada Text tem cor fixa: trocar a cor de um Text a cada quadro recriaria a textura dele.
typedef struct MsgCache MsgCache;
struct MsgCache {
    const char *src;     // texto original (para detectar mudanca)
    size_t src_len;
    char *wrapped;
    int wrap_w;
    double gs;
    int lines;
    int width;           // largura da linha mais larga
    Text *text;
};

static Text *probe;                                     // so para ler o lineskip da fonte
static Text *name_text[C_COUNT];
static Text *you_text;
static Text *title_text[DIALOG_LOG_MAX_CONVERSATIONS];
static Text *empty_list_text, *empty_pane_text;

static MsgCache *msgs;
static size_t msgs_cap;

static int selected = -1;       // indice no DialogLog
static int list_scroll;         // px rolados a partir do topo
static int msg_scroll_up;       // px rolados para cima a partir do fim (0 = mensagens mais novas)
static int hover_row = -1;      // linha da lista sob o mouse (ordem de exibicao)
static int pressed_row = -1;

//
// Textos
//
static Text* newText(void)
{
    return Text_Create(cache[GFONT_REGULAR]);
}

static void drawText(Text *t, const char *str, int x, int y, SDL_Color color, double gs)
{
    if(!t || !str){
        return;
    }
    Text_DrawUTF8(
        t,
        (char*)str,
        TextConfig_Create(
            .x = x,
            .y = y,
            .color = color,
            .scale_width = gs,
            .scale_height = gs,
        ),
        NULL
    );
}

static double glyphScale(void)
{
    return TEXT_SCALE * MonitorSpace_TextScale();
}

static int glyphWidth(void *ctx, int codepoint)
{
    double gs = *(double*)ctx;
    int w = 0, h = 0;

    TextCache_GetGlyphSize(cache[GFONT_REGULAR], codepoint, CHAR_FMT_REGULAR, &w, &h);
    return (int)(w * gs);
}

static int textWidth(const char *str, double gs)
{
    const char *cursor = str;
    int width = 0;

    while(cursor && *cursor){
        width += glyphWidth(&gs, DialogFx_NextCodepoint(&cursor));
    }
    return width;
}

static int lenX(int units)
{
    return MonitorSpace_ToScreenLenX(units);
}

static int lenY(int units)
{
    return MonitorSpace_ToScreenLenY(units);
}

static int lineThickness(void)
{
    return MAX(lenY(2), 1);
}

//
// Ciclo de vida
//
void AppBateBoca_Init(void)
{
    size_t i;

    probe = newText();
    for(i = 0; i < C_COUNT; ++i){
        name_text[i] = newText();
    }
    you_text = newText();
    for(i = 0; i < DIALOG_LOG_MAX_CONVERSATIONS; ++i){
        title_text[i] = newText();
    }
    empty_list_text = newText();
    empty_pane_text = newText();

    selected = -1;
    list_scroll = 0;
    msg_scroll_up = 0;
    hover_row = pressed_row = -1;
}

void AppBateBoca_Quit(void)
{
    size_t i;

    for(i = 0; i < msgs_cap; ++i){
        free(msgs[i].wrapped);
        Text_Destroy(&msgs[i].text);
    }
    free(msgs);
    msgs = NULL;
    msgs_cap = 0;

    Text_Destroy(&probe);
    for(i = 0; i < C_COUNT; ++i){
        Text_Destroy(&name_text[i]);
    }
    Text_Destroy(&you_text);
    for(i = 0; i < DIALOG_LOG_MAX_CONVERSATIONS; ++i){
        Text_Destroy(&title_text[i]);
    }
    Text_Destroy(&empty_list_text);
    Text_Destroy(&empty_pane_text);
}

//
// Lista de conversas
//
// Ordem de exibicao: a conversa com atividade mais recente primeiro.
static int buildOrder(int *order)
{
    const DialogLogConversation *a, *b;
    size_t n, i;
    int j, tmp;

    n = DialogLog_Count();
    for(i = 0; i < n; ++i){
        order[i] = (int)i;
    }
    for(i = 1; i < n; ++i){                         // insercao: n <= 32
        j = (int)i;
        while(j > 0){
            a = DialogLog_Get((size_t)order[j - 1]);
            b = DialogLog_Get((size_t)order[j]);
            if(a->last_update >= b->last_update){
                break;
            }
            tmp = order[j - 1];
            order[j - 1] = order[j];
            order[j] = tmp;
            --j;
        }
    }
    return (int)n;
}

static SDL_Rect listRect(const SDL_Rect *body)
{
    return (SDL_Rect){body->x, body->y, MIN(lenX(LIST_W), body->w), body->h};
}

static SDL_Rect paneRect(const SDL_Rect *body)
{
    SDL_Rect list = listRect(body);

    return (SDL_Rect){body->x + list.w, body->y, body->w - list.w, body->h};
}

static int rowStride(void)
{
    return lenY(LIST_ROW_H + LIST_ROW_GAP);
}

static int listContentH(int count)
{
    return lenY(LIST_PAD) * 2 + count * rowStride();
}

static void clampListScroll(const SDL_Rect *list, int count)
{
    int max_scroll = MAX(listContentH(count) - list->h, 0);

    list_scroll = (list_scroll < 0)? 0: (list_scroll > max_scroll)? max_scroll: list_scroll;
}

static SDL_Rect rowRect(const SDL_Rect *list, int row)
{
    return (SDL_Rect){
        list->x + lenX(LIST_PAD),
        list->y + lenY(LIST_PAD) + row * rowStride() - list_scroll,
        list->w - 2 * lenX(LIST_PAD),
        lenY(LIST_ROW_H)
    };
}

// Linha da lista sob (x, y), ou -1
static int hitRow(const SDL_Rect *body, int x, int y)
{
    SDL_Rect list = listRect(body);
    SDL_Point p = {x, y};
    SDL_Rect row;
    int count, i;
    int order[DIALOG_LOG_MAX_CONVERSATIONS];

    count = buildOrder(order);
    for(i = 0; i < count; ++i){
        row = rowRect(&list, i);
        if(SDL_PointInRect(&p, &row) && SDL_PointInRect(&p, &list)){
            return i;
        }
    }
    return -1;
}

static void selectConversation(int conv)
{
    size_t i;

    selected = conv;
    msg_scroll_up = 0;
    for(i = 0; i < msgs_cap; ++i){          // o cache e de outra conversa
        msgs[i].src = NULL;
    }
    if(conv >= 0){
        DialogLog_MarkRead((size_t)conv);
    }
}

//
// Eventos
//
void AppBateBoca_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside)
{
    SDL_Rect pane;
    int order[DIALOG_LOG_MAX_CONVERSATIONS];
    int under, count, mx, my;

    switch(e->type){
        case SDL_MOUSEMOTION:
            hover_row = pointer_inside? hitRow(body, e->motion.x, e->motion.y): -1;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(e->button.button == SDL_BUTTON_LEFT){
                pressed_row = hitRow(body, e->button.x, e->button.y);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(e->button.button != SDL_BUTTON_LEFT){
                break;
            }
            // So conta se soltou sobre a mesma linha em que apertou
            under = pointer_inside? hitRow(body, e->button.x, e->button.y): -1;
            if(pressed_row >= 0 && pressed_row == under){
                count = buildOrder(order);
                if(pressed_row < count){
                    selectConversation(order[pressed_row]);
                }
            }
            pressed_row = -1;
            break;

        case SDL_MOUSEWHEEL:
            if(!pointer_inside){
                break;
            }
            SDL_GetMouseState(&mx, &my);
            pane = paneRect(body);
            if(mx < pane.x){
                list_scroll -= e->wheel.y * lenY(WHEEL_STEP);
            }
            else {
                msg_scroll_up += e->wheel.y * lenY(WHEEL_STEP);
            }
            break;

        default:
            break;
    }
}

//
// Mensagens
//
typedef struct Metrics Metrics;
struct Metrics {
    double gs;
    int ls;            // altura de uma linha de texto
    int pad_x, pad_y;
    int avatar, avatar_gap;
    int margin, gap;
    int wrap_w;        // largura maxima do texto de um balao
    int line;          // espessura dos contornos
};

static Metrics makeMetrics(const SDL_Rect *pane)
{
    Metrics k;
    int space;

    k.gs = glyphScale();
    k.ls = MAX((int)(*Text_lineskip(probe) * k.gs), 1);
    k.pad_x = lenX(BUBBLE_PAD_X);
    k.pad_y = lenY(BUBBLE_PAD_Y);
    k.avatar = lenY(AVATAR);
    k.avatar_gap = lenX(AVATAR_GAP);
    k.margin = lenX(PANE_PAD);
    k.gap = lenY(MSG_GAP);
    k.line = lineThickness();

    space = pane->w - 2 * k.margin - k.avatar - k.avatar_gap - 2 * k.pad_x - lenX(SCROLLBAR_W) * 2;
    k.wrap_w = MAX(space * BUBBLE_MAX_PCT / 100, lenX(120));
    return k;
}

static int ensureMsgCapacity(size_t n)
{
    MsgCache *grown;
    size_t new_cap;

    if(n <= msgs_cap){
        return 1;
    }
    new_cap = MAX(n, msgs_cap * 2);
    grown = realloc(msgs, new_cap * sizeof(*msgs));
    if(!grown){
        return 0;
    }
    memset(&grown[msgs_cap], 0, (new_cap - msgs_cap) * sizeof(*msgs));
    msgs = grown;
    msgs_cap = new_cap;
    return 1;
}

// Quebra o texto em linhas e mede (refeito so quando algo muda)
static MsgCache* ensureMsg(size_t i, const DialogLogEntry *entry, const Metrics *k)
{
    MsgCache *m = &msgs[i];
    const char *cursor;
    int cp, line_w;

    if(m->wrapped && m->src == entry->text && m->src_len == strlen(entry->text)
        && m->wrap_w == k->wrap_w && m->gs == k->gs){
        return m;
    }

    free(m->wrapped);
    m->wrapped = DialogFx_WordWrap(entry->text, k->wrap_w, 0, glyphWidth, (void*)&k->gs);
    m->src = entry->text;
    m->src_len = strlen(entry->text);
    m->wrap_w = k->wrap_w;
    m->gs = k->gs;
    m->lines = 1;
    m->width = 0;

    line_w = 0;
    cursor = m->wrapped? m->wrapped: "";
    while(*cursor){
        cp = DialogFx_NextCodepoint(&cursor);
        if(cp == '\n'){
            m->lines++;
            line_w = 0;
            continue;
        }
        line_w += glyphWidth((void*)&k->gs, cp);
        m->width = MAX(m->width, line_w);
    }
    return m;
}

static const char* nameOf(const DialogLogEntry *e)
{
    if(e->kind == DIALOG_LOG_CHOICE){
        return PLAYER_NAME_IN_CHOICE;
    }
    if(e->speaker >= 0 && e->speaker < C_COUNT && character_names[e->speaker]){
        return character_names[e->speaker];
    }
    return "?";
}

static Text* nameTextOf(const DialogLogEntry *e)
{
    if(e->kind == DIALOG_LOG_CHOICE){
        return you_text;
    }
    return (e->speaker >= 0 && e->speaker < C_COUNT)? name_text[e->speaker]: NULL;
}

static int itemHeight(const DialogLogEntry *e, const MsgCache *m, const Metrics *k)
{
    int text_h = m->lines * k->ls;

    if(e->kind == DIALOG_LOG_NOTE){
        return text_h + k->pad_y;
    }
    return MAX(k->avatar, 2 * k->pad_y + k->ls + text_h);
}

static void drawAvatar(int speaker, int state, const SDL_Rect *dst, const Metrics *k)
{
    SDL_Texture *tex = NULL;
    SDL_Rect src;
    int tw, th;

    MonitorDraw_Fill(dst, COLOR_AVATAR_BG);
    if(speaker >= 0 && speaker < C_COUNT && state >= 0 && state < CS_COUNT){
        tex = chars_tex[speaker][state];
    }
    if(tex){
        SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
        src = (SDL_Rect){0, (int)(th * AVATAR_CROP_Y_FRAC), tw, tw};
        if(src.y + src.h > th){
            src.y = MAX(th - src.h, 0);
            src.h = MIN(src.h, th);
        }
        SDL_RenderCopy(r, tex, &src, dst);
    }
    MonitorDraw_Outline(dst, k->line, COLOR_LINE);
}

static void drawItem(
    const DialogLogEntry *e, MsgCache *m, const Metrics *k,
    const SDL_Rect *pane, int y, int item_h
){
    SDL_Rect bubble, avatar_rect;
    SDL_Color fill;
    const char *name;
    int bubble_w, bubble_h, name_w, right_side, x, text_x, text_y;

    if(!m->wrapped){
        return;
    }
    if(!m->text){
        m->text = newText();
    }

    if(e->kind == DIALOG_LOG_NOTE){
        // Aviso neutro: texto cinza centralizado, sem balao
        x = pane->x + (pane->w - m->width) / 2;
        drawText(m->text, m->wrapped, x, y + k->pad_y / 2, TEXT_NOTE, k->gs);
        return;
    }

    right_side = (e->kind == DIALOG_LOG_CHOICE || e->speaker == C_PEDRO);
    fill = (e->kind == DIALOG_LOG_CHOICE)? COLOR_CHOICE: right_side? COLOR_PLAYER: COLOR_OTHER;
    name = nameOf(e);
    name_w = textWidth(name, k->gs);
    bubble_w = MAX(m->width, name_w) + 2 * k->pad_x;
    bubble_h = 2 * k->pad_y + k->ls + m->lines * k->ls;

    if(right_side){
        avatar_rect = (SDL_Rect){pane->x + pane->w - k->margin - k->avatar - lenX(SCROLLBAR_W) * 2, y, k->avatar, k->avatar};
        bubble.x = avatar_rect.x - k->avatar_gap - bubble_w;
    }
    else {
        avatar_rect = (SDL_Rect){pane->x + k->margin, y, k->avatar, k->avatar};
        bubble.x = avatar_rect.x + k->avatar + k->avatar_gap;
    }
    bubble.y = y;
    bubble.w = bubble_w;
    bubble.h = bubble_h;
    (void)item_h;

    drawAvatar(e->speaker, (e->kind == DIALOG_LOG_CHOICE)? CS_NORMAL: e->state, &avatar_rect, k);
    MonitorDraw_Fill(&bubble, fill);
    MonitorDraw_Outline(&bubble, k->line, COLOR_LINE);

    text_x = bubble.x + k->pad_x;
    text_y = bubble.y + k->pad_y;
    drawText(nameTextOf(e), name, text_x, text_y, TEXT_NAME, k->gs);
    drawText(m->text, m->wrapped, text_x, text_y + k->ls, TEXT_BLACK, k->gs);
}

static void drawScrollbar(const SDL_Rect *view, int content_h, int offset_from_top)
{
    SDL_Rect thumb;
    int thumb_h, track_w, max_scroll;

    if(content_h <= view->h){
        return;
    }
    track_w = lenX(SCROLLBAR_W);
    thumb_h = MAX(view->h * view->h / content_h, lenY(40));
    max_scroll = content_h - view->h;
    thumb = (SDL_Rect){
        view->x + view->w - track_w - 2,
        view->y + (int)((double)(view->h - thumb_h) * offset_from_top / max_scroll),
        track_w,
        thumb_h
    };
    MonitorDraw_Fill(&thumb, COLOR_SCROLL);
}

// Define o clip como (rect dentro da area do desktop)
static void setClip(const SDL_Rect *rect)
{
    SDL_Rect desktop = MonitorSpace_DesktopOnScreen();
    SDL_Rect clip;

    if(SDL_IntersectRect(rect, &desktop, &clip)){
        SDL_RenderSetClipRect(r, &clip);
    }
    else {
        SDL_RenderSetClipRect(r, &(SDL_Rect){0, 0, 0, 0});
    }
}

static void drawMessages(const SDL_Rect *pane, int conv)
{
    const DialogLogConversation *c = DialogLog_Get((size_t)conv);
    SDL_Rect header, view;
    Metrics k;
    int total_h, max_scroll, y, item_h;
    MsgCache *m;
    size_t n;

    if(!c || !ensureMsgCapacity(c->count)){
        return;
    }

    header = (SDL_Rect){pane->x, pane->y, pane->w, lenY(HEADER_H)};
    view = (SDL_Rect){pane->x, pane->y + header.h, pane->w, pane->h - header.h};
    k = makeMetrics(pane);

    // Altura total
    total_h = k.margin * 2;
    for(n = 0; n < c->count; ++n){
        m = ensureMsg(n, &c->entries[n], &k);
        total_h += itemHeight(&c->entries[n], m, &k) + k.gap;
    }
    total_h -= k.gap;

    max_scroll = MAX(total_h - view.h, 0);
    msg_scroll_up = (msg_scroll_up < 0)? 0: (msg_scroll_up > max_scroll)? max_scroll: msg_scroll_up;

    // Mensagens mais novas ficam coladas embaixo quando o historico passa da altura
    y = view.y + k.margin;
    if(total_h > view.h){
        y = view.y + view.h - total_h + k.margin + msg_scroll_up;
    }

    setClip(&view);
    for(n = 0; n < c->count; ++n){
        m = &msgs[n];
        item_h = itemHeight(&c->entries[n], m, &k);
        if(y + item_h >= view.y && y <= view.y + view.h){
            drawItem(&c->entries[n], m, &k, pane, y, item_h);
        }
        y += item_h + k.gap;
    }
    drawScrollbar(&view, total_h, max_scroll - msg_scroll_up);

    // Titulo (por cima, fora do clip das mensagens)
    setClip(&header);
    MonitorDraw_Fill(&header, COLOR_HEADER_BG);
    MonitorDraw_Fill(&(SDL_Rect){header.x, header.y + header.h - k.line, header.w, k.line}, COLOR_LINE);
    drawText(
        title_text[conv], c->title,
        header.x + k.margin, header.y + (header.h - k.ls) / 2,
        TEXT_BLACK, k.gs
    );
}

//
// Desenho
//
static const DialogLogEntry* firstOtherSpeech(const DialogLogConversation *c)
{
    size_t i;

    for(i = 0; i < c->count; ++i){
        if(c->entries[i].kind == DIALOG_LOG_SPEECH && c->entries[i].speaker != C_PEDRO){
            return &c->entries[i];
        }
    }
    return NULL;
}

static void drawList(const SDL_Rect *list)
{
    const DialogLogConversation *c;
    const DialogLogEntry *other;
    SDL_Rect row, avatar, dot;
    Metrics k;
    int order[DIALOG_LOG_MAX_CONVERSATIONS];
    int count, i, conv, text_x;

    setClip(list);
    MonitorDraw_Fill(list, COLOR_LIST_BG);
    count = buildOrder(order);
    k.gs = glyphScale();
    k.ls = MAX((int)(*Text_lineskip(probe) * k.gs), 1);
    k.line = lineThickness();

    if(count == 0){
        drawText(
            empty_list_text, "Nenhuma conversa ainda",
            list->x + lenX(LIST_PAD), list->y + lenY(LIST_PAD), TEXT_NOTE, k.gs
        );
        return;
    }

    clampListScroll(list, count);
    for(i = 0; i < count; ++i){
        conv = order[i];
        c = DialogLog_Get((size_t)conv);
        row = rowRect(list, i);
        if(row.y + row.h < list->y || row.y > list->y + list->h){
            continue;
        }

        MonitorDraw_Fill(&row, (conv == selected)? COLOR_ROW_SELECT: COLOR_ROW);
        if(i == hover_row){
            MonitorDraw_Fill(&row, COLOR_HOVER);
        }
        MonitorDraw_Outline(&row, k.line, COLOR_LINE);

        text_x = row.x + lenX(10);
        other = firstOtherSpeech(c);
        if(other){
            avatar = (SDL_Rect){row.x + lenX(10), row.y + (row.h - lenY(LIST_AVATAR)) / 2, lenY(LIST_AVATAR), lenY(LIST_AVATAR)};
            drawAvatar(other->speaker, CS_NORMAL, &avatar, &k);
            text_x = avatar.x + avatar.w + lenX(12);
        }
        drawText(title_text[conv], c->title, text_x, row.y + (row.h - k.ls) / 2, TEXT_BLACK, k.gs);

        if(c->unread > 0 && conv != selected){
            dot = (SDL_Rect){row.x + row.w - lenX(UNREAD_DOT) - lenX(12), row.y + (row.h - lenX(UNREAD_DOT)) / 2, lenX(UNREAD_DOT), lenX(UNREAD_DOT)};
            MonitorDraw_Fill(&dot, COLOR_UNREAD);
            MonitorDraw_Outline(&dot, k.line, COLOR_LINE);
        }
    }
    drawScrollbar(list, listContentH(count), list_scroll);
}

void AppBateBoca_Draw(const SDL_Rect *body)
{
    SDL_Rect list, pane, body_clip;
    int order[DIALOG_LOG_MAX_CONVERSATIONS];
    int count, line;
    double gs;

    list = listRect(body);
    pane = paneRect(body);
    line = lineThickness();
    count = (int)DialogLog_Count();

    // A conversa aberta pode ter sumido (limite de conversas / reinicio)
    if(selected >= count){
        selectConversation(-1);
    }
    // Sem nada aberto, mostra a mais recente
    if(selected < 0 && count > 0){
        buildOrder(order);
        selectConversation(order[0]);
    }
    // Uma conversa aberta esta sempre "lida", mesmo que chegue mensagem nova
    if(selected >= 0){
        DialogLog_MarkRead((size_t)selected);
    }

    MonitorDraw_Fill(&pane, COLOR_PANE_BG);
    if(selected >= 0){
        drawMessages(&pane, selected);
    }
    else {
        gs = glyphScale();
        drawText(
            empty_pane_text, "Selecione uma conversa",
            pane.x + lenX(PANE_PAD), pane.y + lenY(PANE_PAD), TEXT_NOTE, gs
        );
    }

    drawList(&list);
    MonitorDraw_Fill(&(SDL_Rect){list.x + list.w - line, list.y, line, list.h}, COLOR_LINE);

    // Devolve o clip que a janela espera (corpo da janela dentro da area do desktop)
    body_clip = *body;
    setClip(&body_clip);
}
