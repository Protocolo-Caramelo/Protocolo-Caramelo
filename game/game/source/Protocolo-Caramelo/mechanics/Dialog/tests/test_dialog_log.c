/*
 * Testes do registro de conversas (sem SDL).
 *
 * Compilar e rodar (a partir desta pasta):
 *   gcc -std=c99 -Wall -Wextra -o /tmp/test_dialog_log tests/test_dialog_log.c dialog_log.c && /tmp/test_dialog_log
 */
#include "../dialog_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_checks = 0;
#define CHECK(cond) do { ++g_checks; if(!(cond)){ \
    printf("FALHOU %s:%d: %s\n", __FILE__, __LINE__, #cond); exit(1); } } while(0)

static void test_basic(void)
{
    const DialogLogConversation *c;
    char buffer[16] = "temporario";

    DialogLog_Reset();
    CHECK(DialogLog_Count() == 0);
    CHECK(DialogLog_Get(0) == NULL);
    CHECK(DialogLog_Find("Felipe") == -1);
    CHECK(DialogLog_Find(NULL) == -1);

    DialogLog_AddSpeech("Felipe", 1, 2, buffer);
    strcpy(buffer, "mudou");                       /* o texto foi copiado */
    DialogLog_AddSpeech("Felipe", 0, 3, "Oi");
    DialogLog_AddChoice("Felipe", 0, "Aceitar");
    DialogLog_AddNote("Felipe", "Chamada encerrada");

    CHECK(DialogLog_Count() == 1);
    CHECK(DialogLog_Find("Felipe") == 0);
    c = DialogLog_Get(0);
    CHECK(strcmp(c->title, "Felipe") == 0);
    CHECK(c->count == 4);
    CHECK(c->entries[0].kind == DIALOG_LOG_SPEECH);
    CHECK(strcmp(c->entries[0].text, "temporario") == 0);
    CHECK(c->entries[0].speaker == 1 && c->entries[0].state == 2);
    CHECK(c->entries[2].kind == DIALOG_LOG_CHOICE);
    CHECK(c->entries[3].kind == DIALOG_LOG_NOTE);
    CHECK(c->unread == 4);
    CHECK(DialogLog_TotalUnread() == 4);

    DialogLog_MarkRead(0);
    CHECK(DialogLog_Get(0)->unread == 0);
    CHECK(DialogLog_TotalUnread() == 0);
    DialogLog_MarkRead(99);                        /* invalido: ignorado */

    /* texto/titulo NULL nao quebram */
    DialogLog_AddSpeech("Ana", 2, 0, NULL);
    CHECK(strcmp(DialogLog_Get(1)->entries[0].text, "") == 0);
    DialogLog_AddSpeech(NULL, 2, 0, "x");
    CHECK(DialogLog_Count() == 2);

    /* recencia */
    CHECK(DialogLog_Get(1)->last_update > DialogLog_Get(0)->last_update);
    DialogLog_AddSpeech("Felipe", 1, 0, "de novo");
    CHECK(DialogLog_Get(0)->last_update > DialogLog_Get(1)->last_update);

    DialogLog_Reset();
    CHECK(DialogLog_Count() == 0);
}

static void test_limits(void)
{
    char title[32];
    int i;
    const DialogLogConversation *c;

    DialogLog_Reset();

    /* entradas: passa do limite, as mais antigas saem */
    for(i = 0; i < DIALOG_LOG_MAX_ENTRIES + 25; ++i){
        char txt[16];
        snprintf(txt, sizeof(txt), "m%d", i);
        DialogLog_AddSpeech("Longa", 0, 0, txt);
    }
    c = DialogLog_Get(0);
    CHECK(c->count == DIALOG_LOG_MAX_ENTRIES);
    CHECK(strcmp(c->entries[0].text, "m25") == 0);
    CHECK(strcmp(c->entries[c->count - 1].text, "m424") == 0);
    CHECK(c->unread <= c->count);

    /* conversas: passa do limite, a menos recente sai */
    DialogLog_Reset();
    for(i = 0; i < DIALOG_LOG_MAX_CONVERSATIONS; ++i){
        snprintf(title, sizeof(title), "c%d", i);
        DialogLog_AddSpeech(title, 0, 0, "oi");
    }
    DialogLog_AddSpeech("c0", 0, 0, "de novo");    /* c0 vira a mais recente; c1 e a mais antiga */
    DialogLog_AddSpeech("nova", 0, 0, "oi");
    CHECK(DialogLog_Count() == DIALOG_LOG_MAX_CONVERSATIONS);
    CHECK(DialogLog_Find("c1") == -1);
    CHECK(DialogLog_Find("c0") >= 0);
    CHECK(DialogLog_Find("nova") >= 0);
    CHECK(DialogLog_Get((size_t)DialogLog_Find("c0"))->count == 2);

    DialogLog_Reset();
}

int main(void)
{
    test_basic();
    test_limits();
    printf("OK: %d verificacoes passaram\n", g_checks);
    return 0;
}
