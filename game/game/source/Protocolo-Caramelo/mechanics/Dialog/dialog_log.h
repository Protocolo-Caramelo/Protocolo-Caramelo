#ifndef DIALOG_LOG_H
#define DIALOG_LOG_H

/*
 * DialogLog: o "arquivo" das conversas guardadas (e o que o app Bate-boca mostra).
 *
 * Este modulo NAO depende de SDL nem do resto do jogo (quem fala e a expressao
 * sao guardados como inteiros: Character e Character_State). Assim ele pode
 * ser testado sozinho (tests/test_dialog_log.c).
 *
 * Uma CONVERSA e identificada pelo titulo (ex.: "Felipe", "Ana"). Varias cenas
 * com o mesmo titulo se acumulam na mesma conversa, como um historico de chat.
 * Os textos sao COPIADOS: o chamador nao precisa mante-los vivos.
 *
 * Quem grava? Normalmente o proprio motor de dialogo: toda DialogScene com
 * `archive_title` grava cada fala quando ela aparece e cada opcao escolhida.
 * Voce tambem pode gravar a mao com as funcoes DialogLog_Add*.
 */

#include <stddef.h>

/* Limites (os mais antigos saem primeiro quando passam do limite) */
#define DIALOG_LOG_MAX_CONVERSATIONS  32
#define DIALOG_LOG_MAX_ENTRIES        400

typedef enum DialogLogKind {
    DIALOG_LOG_SPEECH,    /* fala de um personagem */
    DIALOG_LOG_CHOICE,    /* opcao que o jogador escolheu */
    DIALOG_LOG_NOTE       /* aviso neutro: "Chamada encerrada", "+10 de amizade"... */
} DialogLogKind;

typedef struct DialogLogEntry {
    DialogLogKind kind;
    int speaker;          /* Character (ignorado em DIALOG_LOG_NOTE) */
    int state;            /* Character_State: qual rosto mostrar */
    char *text;           /* UTF-8, copia propria */
} DialogLogEntry;

typedef struct DialogLogConversation {
    char *title;
    DialogLogEntry *entries;
    size_t count;
    size_t capacity;
    size_t unread;               /* entradas novas desde a ultima DialogLog_MarkRead */
    unsigned long last_update;   /* contador crescente: maior = mais recente */
} DialogLogConversation;

/* ------------------------------------------------------------------ */
/* Gravar (cria a conversa se ela ainda nao existir)                   */
/* ------------------------------------------------------------------ */
void DialogLog_AddSpeech(const char *title, int speaker, int state, const char *text);
void DialogLog_AddChoice(const char *title, int speaker, const char *label);
void DialogLog_AddNote(const char *title, const char *text);

/* ------------------------------------------------------------------ */
/* Ler                                                                 */
/* ------------------------------------------------------------------ */
size_t DialogLog_Count(void);
/* NULL se i for invalido. O ponteiro vale ate a proxima gravacao/Reset. */
const DialogLogConversation *DialogLog_Get(size_t i);
/* Indice da conversa com esse titulo, ou -1. */
int DialogLog_Find(const char *title);

/* ------------------------------------------------------------------ */
/* "Nao lidas"                                                         */
/* ------------------------------------------------------------------ */
void DialogLog_MarkRead(size_t i);
size_t DialogLog_TotalUnread(void);

/* Apaga tudo (novo jogo / encerramento). */
void DialogLog_Reset(void);

#endif
