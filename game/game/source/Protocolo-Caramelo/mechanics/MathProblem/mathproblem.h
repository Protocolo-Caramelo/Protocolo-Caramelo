#ifndef MATHPROBLEM_H
#define MATHPROBLEM_H

/*
 * mathproblem.h - Estrutura MathProblem: um problema matemático com
 * enunciado em texto e resposta criptografada em AES-128.
 *
 * Uso básico:
 *
 *     MathProblem problem;
 *     MathProblem_GenProblem(&problem, MATHPROBLEM_EQUATION_1, MATHPROBLEM_DIFFICULTY_EASY);
 *     printf("%s\n", problem.question);
 *
 *     if (MathProblem_CheckAnswer(&problem, "7")) { ... }  // sem expor a resposta
 *
 *     MathProblem_GetProblem(&problem);                    // revela em problem.answer
 *     MathProblem_HideAnswer(&problem);                    // apaga problem.answer
 *
 * Observação: o arquivo deve ser salvo em UTF-8 (os enunciados têm acentos).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "aes128.h"

/* Tamanhos dos buffers (incluem o '\0' final). */
#define MATHPROBLEM_QUESTION_SIZE 256
#define MATHPROBLEM_ANSWER_SIZE   64
#define MATHPROBLEM_RESPONSE_SIZE (2 * MATHPROBLEM_ANSWER_SIZE + 1) /* resposta cifrada em hexadecimal */

typedef enum MathProblem_Type {
    MATHPROBLEM_NORMAL,                 /* apenas contas */
    MATHPROBLEM_EQUATION_1,             /* equação do 1º grau */
    MATHPROBLEM_EQUATION_2,             /* equação do 2º grau */
    MATHPROBLEM_NEWTON,                 /* binômio de Newton */
    MATHPROBLEM_FUNCTION_LINEAR,        /* função linear */
    MATHPROBLEM_FUNCTION_QUADRACTIC,    /* função quadrática */
    MATHPROBLEM_FUNCTION_EXPONENTIAL,   /* função exponencial */
    MATHPROBLEM_FUNCTION_LOGARITHMICA,  /* função logarítmica */
    MATHPROBLEM_TYPE_COUNT              /* quantidade de tipos (não é um tipo válido) */
} MathProblem_Type;

/*
 * Referência: conhecimento médio de um universitário de engenharia,
 * resolvendo em cerca de 1 minuto.
 */
typedef enum MathProblem_Difficulty {
    MATHPROBLEM_DIFFICULTY_EASY,        /* dá para resolver de cabeça */
    MATHPROBLEM_DIFFICULTY_MEDIUM,      /* de cabeça com algum esforço */
    MATHPROBLEM_DIFFICULTY_HARD,        /* precisa de papel e caneta */
    MATHPROBLEM_DIFFICULTY_IMPOSSIBLE,  /* só com calculadora */
    MATHPROBLEM_DIFFICULTY_COUNT        /* quantidade de dificuldades (não é uma dificuldade válida) */
} MathProblem_Difficulty;

typedef struct MathProblem {
    char question[MATHPROBLEM_QUESTION_SIZE];   /* enunciado + problema, ex.: "Calcule: 3 + 4" */
    char response[MATHPROBLEM_RESPONSE_SIZE];   /* resposta cifrada em AES, escrita em hexadecimal */
    char answer[MATHPROBLEM_ANSWER_SIZE];       /* resposta em texto puro; vazia até chamar GetProblem */
    uint8_t key[AES128_KEY_SIZE];               /* chave AES (gerada a cada problema) */
    uint8_t iv[AES128_BLOCK_SIZE];              /* valor inicial do contador do modo CTR */
    double time;                                /* segundos desde o início do programa até a criação */
    MathProblem_Type type;
    MathProblem_Difficulty difficulty;
} MathProblem;

/*
 * Gera (ou sobrescreve) um problema: preenche question, response (cifrada),
 * time, type e difficulty. Se type ou difficulty forem inválidos, o problema
 * não é alterado.
 */
void MathProblem_GenProblem(MathProblem *problem, MathProblem_Type type, MathProblem_Difficulty difficulty);

/* Descriptografa a resposta e a grava em problem->answer. */
void MathProblem_GetProblem(MathProblem *problem);

/* Apaga problem->answer (texto puro) da memória. */
void MathProblem_HideAnswer(MathProblem *problem);

/*
 * Compara a tentativa com a resposta correta sem deixar o texto puro
 * guardado na estrutura. A comparação é exata (ex.: "-3 e 2").
 */
bool MathProblem_CheckAnswer(const MathProblem *problem, const char *attempt);

#endif /* MATHPROBLEM_H */
