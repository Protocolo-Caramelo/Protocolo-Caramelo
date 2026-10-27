#ifndef MATHPROBLEM_GENERATORS_H
#define MATHPROBLEM_GENERATORS_H

/*
 * mathproblem_generators.h - Módulo interno: um gerador por tipo de problema.
 *
 * Cada gerador monta um rascunho (enunciado + resposta em texto puro).
 * Quem cifra e preenche a estrutura MathProblem é o mathproblem.c.
 */

#include "mathproblem.h"

typedef struct MathProblem_Draft {
    char question[MATHPROBLEM_QUESTION_SIZE];
    char answer[MATHPROBLEM_ANSWER_SIZE];
} MathProblem_Draft;

typedef void (*MathProblem_Generator)(MathProblem_Draft *draft, MathProblem_Difficulty difficulty);

/* Devolve o gerador do tipo informado, ou NULL se o tipo for inválido. */
MathProblem_Generator MathProblem_GetGenerator(MathProblem_Type type);

#endif /* MATHPROBLEM_GENERATORS_H */
