/* exemplo.c - Gera um problema de cada tipo e dificuldade e mostra o resultado. */

#include <stdio.h>

#include "mathproblem.h"

static const char *const TYPE_NAMES[MATHPROBLEM_TYPE_COUNT] = {
    "Normal", "Equacao 1o grau", "Equacao 2o grau", "Binomio de Newton",
    "Funcao linear", "Funcao quadratica", "Funcao exponencial", "Funcao logaritmica"
};

static const char *const DIFFICULTY_NAMES[MATHPROBLEM_DIFFICULTY_COUNT] = {
    "Facil", "Mediano", "Dificil", "Impossivel"
};

int main(void)
{
    MathProblem problem;

    for (int type = 0; type < MATHPROBLEM_TYPE_COUNT; type++) {
        for (int difficulty = 0; difficulty < MATHPROBLEM_DIFFICULTY_COUNT; difficulty++) {
            MathProblem_GenProblem(&problem, (MathProblem_Type)type, (MathProblem_Difficulty)difficulty);

            printf("[%s | %s | t=%.3fs]\n", TYPE_NAMES[type], DIFFICULTY_NAMES[difficulty], problem.time);
            printf("  %s\n", problem.question);
            printf("  resposta cifrada: %s\n", problem.response);

            MathProblem_GetProblem(&problem);
            printf("  resposta: %s\n\n", problem.answer);
            MathProblem_HideAnswer(&problem);
        }
    }
    return 0;
}
