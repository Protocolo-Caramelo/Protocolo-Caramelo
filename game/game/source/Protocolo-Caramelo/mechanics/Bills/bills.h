#ifndef BILLS_H
#define BILLS_H
/*
 * Contas semanais do jogador (aluguel, luz/agua, internet, alimentacao).
 *
 * Logica pura (sem SDL). Valores em "centavos de caramelo", a mesma unidade de
 * `i_caramelos` (exibido como i_caramelos / 100.0).
 *
 * Funcionamento:
 *   - No inicio do jogo as contas da semana 1 ficam pendentes (vencem em 7 dias).
 *   - Bills_Advance(seg) deve receber todo o tempo de jogo que passa (relogio
 *     normal E sono). Ao chegar o vencimento: se sobrou conta nao paga,
 *     Bills_Failed() passa a valer 1 (game over); senao a semana seguinte
 *     e emitida, com valores maiores (inflacao semanal exponencial).
 *   - Depois da ultima semana (BILLS_TOTAL_WEEKS) nao ha mais contas.
 *
 * Equilibrio: todos os numeros abaixo ficam aqui, em um lugar so.
 */

#define BILLS_KIND_COUNT   4
#define BILLS_TOTAL_WEEKS  4                 /* 30 dias => vencimentos nos dias 7, 14, 21 e 28 */
#define BILLS_WEEK_SEC     (7.0 * 24 * 3600) /* segundos de JOGO por semana */

/* Renda semanal de referencia (em caramelos) de um jogador "competente":
 * ~40 respostas certas por semana a ~1500 caramelos cada. */
#define BILLS_REFERENCE_WEEKLY_INCOME 50000.0
/* Cada conta como fracao da renda de referencia, na semana 1 (soma = 0.40) */
#define BILLS_FRACTION_RENT      0.22
#define BILLS_FRACTION_POWER     0.05
#define BILLS_FRACTION_INTERNET  0.04
#define BILLS_FRACTION_FOOD      0.09
/* Aumento por semana (x1.12 => semana 4 custa ~1.4x a semana 1) */
#define BILLS_WEEKLY_GROWTH 1.12

typedef struct Bill {
    const char *name;
    long        amount;   /* centavos de caramelo */
    int         paid;
} Bill;

typedef enum BillsPayResult {
    BILLS_PAY_OK,
    BILLS_PAY_NO_FUNDS,
    BILLS_PAY_NOTHING        /* nada pendente / indice invalido */
} BillsPayResult;

/* cost_factor: multiplicador de dificuldade (ex.: 0.8 facil, 1.0 normal...) */
void   Bills_Reset(double cost_factor);
void   Bills_Advance(double game_sec);

int    Bills_Week(void);               /* semana em curso (1..TOTAL), 0 = sem contas */
double Bills_SecondsUntilDue(void);    /* < 0 se nao ha vencimento futuro */
int    Bills_PendingCount(void);
long   Bills_PendingTotal(void);
Bill  *Bills_PendingAt(int n);         /* n-esima conta pendente, NULL se nao existe */
int    Bills_Failed(void);             /* 1 = venceu conta nao paga */
const char *Bills_FailedName(void);    /* nome de uma conta que venceu sem pagar */

/* Pagam usando `wallet` (i_caramelos). */
BillsPayResult Bills_Pay(Bill *b, int *wallet);
BillsPayResult Bills_PayAll(int *wallet);   /* tudo ou nada */

#endif
