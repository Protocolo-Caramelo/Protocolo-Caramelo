#include "dialog_log.h"

#include <stdlib.h>
#include <string.h>

static DialogLogConversation *convs;
static size_t conv_count;
static size_t conv_cap;
static unsigned long clock_tick;     /* "relogio" logico: sobe a cada gravacao */

static char *dup_str(const char *s)
{
    size_t n;
    char *copy;

    if(!s){
        s = "";
    }
    n = strlen(s) + 1;
    copy = malloc(n);
    if(copy){
        memcpy(copy, s, n);
    }
    return copy;
}

static void free_entry(DialogLogEntry *e)
{
    free(e->text);
    e->text = NULL;
}

static void free_conversation(DialogLogConversation *c)
{
    size_t i;

    for(i = 0; i < c->count; ++i){
        free_entry(&c->entries[i]);
    }
    free(c->entries);
    free(c->title);
    memset(c, 0, sizeof(*c));
}

void DialogLog_Reset(void)
{
    size_t i;

    for(i = 0; i < conv_count; ++i){
        free_conversation(&convs[i]);
    }
    free(convs);
    convs = NULL;
    conv_count = conv_cap = 0;
    clock_tick = 0;
}

size_t DialogLog_Count(void)
{
    return conv_count;
}

const DialogLogConversation *DialogLog_Get(size_t i)
{
    return (i < conv_count)? &convs[i]: NULL;
}

int DialogLog_Find(const char *title)
{
    size_t i;

    if(!title){
        return -1;
    }
    for(i = 0; i < conv_count; ++i){
        if(strcmp(convs[i].title, title) == 0){
            return (int)i;
        }
    }
    return -1;
}

/* Devolve a conversa (criando-a). NULL se faltar memoria. */
static DialogLogConversation *get_or_create(const char *title)
{
    DialogLogConversation *grown;
    int found;
    size_t new_cap;

    found = DialogLog_Find(title);
    if(found >= 0){
        return &convs[found];
    }

    if(conv_count >= DIALOG_LOG_MAX_CONVERSATIONS){
        /* descarta a conversa menos recente */
        size_t oldest = 0, i;

        for(i = 1; i < conv_count; ++i){
            if(convs[i].last_update < convs[oldest].last_update){
                oldest = i;
            }
        }
        free_conversation(&convs[oldest]);
        memmove(&convs[oldest], &convs[oldest + 1], (conv_count - oldest - 1) * sizeof(*convs));
        --conv_count;
    }
    if(conv_count == conv_cap){
        new_cap = conv_cap? conv_cap * 2: 4;
        grown = realloc(convs, new_cap * sizeof(*convs));
        if(!grown){
            return NULL;
        }
        convs = grown;
        conv_cap = new_cap;
    }

    memset(&convs[conv_count], 0, sizeof(convs[conv_count]));
    convs[conv_count].title = dup_str(title);
    if(!convs[conv_count].title){
        return NULL;
    }
    return &convs[conv_count++];
}

static void add_entry(const char *title, DialogLogKind kind, int speaker, int state, const char *text)
{
    DialogLogConversation *c;
    DialogLogEntry *grown;
    DialogLogEntry entry;
    size_t new_cap;

    if(!title){
        return;
    }
    c = get_or_create(title);
    if(!c){
        return;
    }

    entry.kind = kind;
    entry.speaker = speaker;
    entry.state = state;
    entry.text = dup_str(text);
    if(!entry.text){
        return;
    }

    if(c->count >= DIALOG_LOG_MAX_ENTRIES){
        /* descarta a entrada mais antiga */
        free_entry(&c->entries[0]);
        memmove(&c->entries[0], &c->entries[1], (c->count - 1) * sizeof(*c->entries));
        --c->count;
        if(c->unread > c->count){
            c->unread = c->count;
        }
    }
    if(c->count == c->capacity){
        new_cap = c->capacity? c->capacity * 2: 16;
        grown = realloc(c->entries, new_cap * sizeof(*c->entries));
        if(!grown){
            free(entry.text);
            return;
        }
        c->entries = grown;
        c->capacity = new_cap;
    }

    c->entries[c->count++] = entry;
    c->unread++;
    c->last_update = ++clock_tick;
}

void DialogLog_AddSpeech(const char *title, int speaker, int state, const char *text)
{
    add_entry(title, DIALOG_LOG_SPEECH, speaker, state, text);
}

void DialogLog_AddChoice(const char *title, int speaker, const char *label)
{
    add_entry(title, DIALOG_LOG_CHOICE, speaker, 0, label);
}

void DialogLog_AddNote(const char *title, const char *text)
{
    add_entry(title, DIALOG_LOG_NOTE, 0, 0, text);
}

void DialogLog_MarkRead(size_t i)
{
    if(i < conv_count){
        convs[i].unread = 0;
    }
}

size_t DialogLog_TotalUnread(void)
{
    size_t i, total = 0;

    for(i = 0; i < conv_count; ++i){
        total += convs[i].unread;
    }
    return total;
}
