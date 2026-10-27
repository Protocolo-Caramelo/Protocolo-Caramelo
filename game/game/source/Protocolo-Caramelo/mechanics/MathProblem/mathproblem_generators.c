#include "mathproblem_generators.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(array)  (sizeof(array) / sizeof((array)[0]))
#define STATEMENT_SIZE     160
#define PROBLEM_SIZE       96

/* Sorteia um enunciado de uma lista (veja as listas "statements" abaixo). */
#define PICK_STATEMENT(statements) PickStatement((statements), ARRAY_SIZE(statements))

/*
 * Convenções deste arquivo:
 *  - Todo gerador sorteia valores, monta o problema como texto e calcula
 *    a resposta junto, sempre a partir da solução (ex.: escolhe-se x e
 *    depois se calcula o termo independente), garantindo respostas inteiras.
 *  - A dificuldade só muda a "escala" dos números (ByDifficulty) e,
 *    em alguns tipos, a forma do problema.
 *  - Os enunciados são formatos printf com no máximo um %lld.
 */

/* ======================================================================
 * Utilitários numéricos
 * ====================================================================== */

/* rand() pode ter só 15 bits (Windows), então juntamos várias chamadas. */
static unsigned long long RandomBits(void)
{
    return ((unsigned long long)rand() << 30)
         ^ ((unsigned long long)rand() << 15)
         ^  (unsigned long long)rand();
}

/* Inteiro aleatório em [min, max]. */
static long long RandomRange(long long min, long long max)
{
    return min + (long long)(RandomBits() % (unsigned long long)(max - min + 1));
}

/* Inteiro aleatório em [min, max], diferente de zero. */
static long long RandomNonZero(long long min, long long max)
{
    long long value;
    do {
        value = RandomRange(min, max);
    } while (value == 0);
    return value;
}

/* Escolhe um dos quatro valores conforme a dificuldade. */
static long long ByDifficulty(MathProblem_Difficulty difficulty,
                              long long easy, long long medium,
                              long long hard, long long impossible)
{
    switch (difficulty) {
    case MATHPROBLEM_DIFFICULTY_EASY:   return easy;
    case MATHPROBLEM_DIFFICULTY_MEDIUM: return medium;
    case MATHPROBLEM_DIFFICULTY_HARD:   return hard;
    default:                            return impossible;
    }
}

static long long Power(long long base, int exponent)
{
    long long result = 1;
    while (exponent-- > 0) {
        result *= base;
    }
    return result;
}

/* Coeficiente binomial C(n, k). Cada passo da divisão é exato. */
static long long Binomial(int n, int k)
{
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

static char SignOf(long long value)
{
    return value < 0 ? '-' : '+';
}

static long long Magnitude(long long value)
{
    return value < 0 ? -value : value;
}

/* ======================================================================
 * Utilitários de texto
 * ====================================================================== */

static const char *PickStatement(const char *const *statements, size_t count)
{
    return statements[rand() % count];
}

/* snprintf que acrescenta ao final do que já está no buffer. */
static void Append(char *buffer, size_t size, const char *format, ...)
{
    size_t used = strlen(buffer);
    va_list arguments;

    if (used + 1 >= size) {
        return;
    }
    va_start(arguments, format);
    vsnprintf(buffer + used, size - used, format, arguments);
    va_end(arguments);
}

/*
 * Acrescenta o polinômio coefficients[degree]*x^degree + ... + coefficients[0]
 * no formato natural, ex.: "3x^2 - x + 5".
 */
static void AppendPolynomial(char *buffer, size_t size, const long long *coefficients, int degree)
{
    bool first = true;

    for (int power = degree; power >= 0; power--) {
        long long coefficient = coefficients[power];
        if (coefficient == 0) {
            continue;
        }

        if (first) {
            if (coefficient < 0) {
                Append(buffer, size, "-");
            }
        } else {
            Append(buffer, size, " %c ", SignOf(coefficient));
        }

        if (power == 0) {
            Append(buffer, size, "%lld", Magnitude(coefficient));
        } else {
            if (Magnitude(coefficient) != 1) {
                Append(buffer, size, "%lld", Magnitude(coefficient));
            }
            Append(buffer, size, power == 1 ? "x" : "x^%d", power);
        }
        first = false;
    }

    if (first) {
        Append(buffer, size, "0");
    }
}

/* Monta o enunciado (com `argument` no lugar do %lld, se houver) seguido do problema. */
static void WriteQuestion(MathProblem_Draft *draft, const char *statement,
                          long long argument, const char *problem)
{
    char text[STATEMENT_SIZE];

    snprintf(text, sizeof text, statement, argument);
    snprintf(draft->question, sizeof draft->question, "%s%s", text, problem);
}

static void WriteAnswer(MathProblem_Draft *draft, long long value)
{
    snprintf(draft->answer, sizeof draft->answer, "%lld", value);
}

/* Resposta com duas raízes, sempre da menor para a maior: "-3 e 2". */
static void WriteRootsAnswer(MathProblem_Draft *draft, long long first, long long second)
{
    if (first > second) {
        long long swap = first;
        first = second;
        second = swap;
    }
    snprintf(draft->answer, sizeof draft->answer, "%lld e %lld", first, second);
}

/* ======================================================================
 * Enunciados compartilhados
 * ====================================================================== */

/* Problemas de "substituir x e calcular" (equações de 1º e 2º grau). */
static const char *const EVALUATE_STATEMENTS[] = {
    "Sendo x = %lld, verifique o resultado dessa equação: ",
    "Para x = %lld, calcule o valor da expressão: ",
    "Substituindo x por %lld, qual é o resultado de: ",
    "Dado x = %lld, determine o valor numérico de: "
};

/*
 * Calcula o valor de um polinômio de grau 1 ou 2 para um x dado.
 * Serve tanto para a equação do 1º grau quanto para a do 2º grau.
 */
static void GenerateEvaluation(MathProblem_Draft *draft, MathProblem_Difficulty difficulty, int degree)
{
    long long coefficient_limit = ByDifficulty(difficulty, 5, 15, 50, 999);
    long long x_limit = ByDifficulty(difficulty, 5, 10, 20, 99);
    long long x = RandomNonZero(-x_limit, x_limit);
    long long coefficients[3] = {0, 0, 0};
    long long result = 0;
    char expression[PROBLEM_SIZE] = "";

    coefficients[degree] = RandomNonZero(-coefficient_limit, coefficient_limit);
    for (int power = 0; power < degree; power++) {
        coefficients[power] = RandomNonZero(-coefficient_limit, coefficient_limit);
    }

    for (int power = degree; power >= 0; power--) {   /* método de Horner */
        result = result * x + coefficients[power];
    }

    AppendPolynomial(expression, sizeof expression, coefficients, degree);
    WriteQuestion(draft, PICK_STATEMENT(EVALUATE_STATEMENTS), x, expression);
    WriteAnswer(draft, result);
}

/* ======================================================================
 * MATHPROBLEM_NORMAL - apenas contas
 * ====================================================================== */

typedef void (*ExpressionBuilder)(char *expression, size_t size, long long *result);

static void BuildEasyExpression(char *expression, size_t size, long long *result)
{
    long long a, b;

    switch (rand() % 4) {
    case 0:
        a = RandomRange(10, 99);
        b = RandomRange(10, 99);
        snprintf(expression, size, "%lld + %lld", a, b);
        *result = a + b;
        break;
    case 1:
        a = RandomRange(20, 99);
        b = RandomRange(10, a);
        snprintf(expression, size, "%lld - %lld", a, b);
        *result = a - b;
        break;
    case 2:
        a = RandomRange(2, 12);
        b = RandomRange(2, 12);
        snprintf(expression, size, "%lld * %lld", a, b);
        *result = a * b;
        break;
    default:    /* divisão exata */
        b = RandomRange(2, 12);
        *result = RandomRange(2, 12);
        snprintf(expression, size, "%lld / %lld", *result * b, b);
        break;
    }
}

static void BuildMediumExpression(char *expression, size_t size, long long *result)
{
    long long a, b, c, d;

    switch (rand() % 4) {
    case 0:
        a = RandomRange(11, 25);
        b = RandomRange(3, 9);
        c = RandomRange(10, 99);
        snprintf(expression, size, "%lld * %lld + %lld", a, b, c);
        *result = a * b + c;
        break;
    case 1:
        a = RandomRange(3, 15);
        b = RandomRange(3, 15);
        c = RandomRange(3, 15);
        d = RandomRange(3, 15);
        snprintf(expression, size, "%lld * %lld - %lld * %lld", a, b, c, d);
        *result = a * b - c * d;
        break;
    case 2:
        a = RandomRange(10, 50);
        b = RandomRange(10, 50);
        c = RandomRange(2, 9);
        snprintf(expression, size, "(%lld + %lld) * %lld", a, b, c);
        *result = (a + b) * c;
        break;
    default:
        a = RandomRange(100, 999);
        b = RandomRange(100, 999);
        c = RandomRange(100, 999);
        snprintf(expression, size, "%lld + %lld - %lld", a, b, c);
        *result = a + b - c;
        break;
    }
}

static void BuildHardExpression(char *expression, size_t size, long long *result)
{
    long long a, b, c, d, e;

    switch (rand() % 4) {
    case 0:
        a = RandomRange(11, 60);
        b = RandomRange(11, 60);
        c = RandomRange(100, 999);
        snprintf(expression, size, "%lld * %lld + %lld", a, b, c);
        *result = a * b + c;
        break;
    case 1:
        a = RandomRange(20, 99);
        b = RandomRange(20, 99);
        c = RandomRange(30, 99);
        d = RandomRange(10, c);
        snprintf(expression, size, "(%lld + %lld) * (%lld - %lld)", a, b, c, d);
        *result = (a + b) * (c - d);
        break;
    case 2:     /* a é múltiplo de c, então a divisão é exata */
        c = RandomRange(3, 19);
        a = c * RandomRange(11, 40);
        b = RandomRange(11, 99);
        snprintf(expression, size, "%lld * %lld / %lld", a, b, c);
        *result = a / c * b;
        break;
    default:
        a = RandomRange(11, 39);
        b = RandomRange(11, 39);
        c = RandomRange(11, 39);
        d = RandomRange(11, 39);
        e = RandomRange(100, 999);
        snprintf(expression, size, "%lld * %lld - %lld * %lld + %lld", a, b, c, d, e);
        *result = a * b - c * d + e;
        break;
    }
}

static void BuildImpossibleExpression(char *expression, size_t size, long long *result)
{
    long long a, b, c, d, e;

    switch (rand() % 4) {
    case 0:
        a = RandomRange(1000, 9999);
        b = RandomRange(1000, 9999);
        c = RandomRange(1000, 9999);
        d = RandomRange(1000, 9999);
        snprintf(expression, size, "%lld * %lld + %lld * %lld", a, b, c, d);
        *result = a * b + c * d;
        break;
    case 1:
        a = RandomRange(1000, 9999);
        b = RandomRange(1000, 9999);
        c = RandomRange(1000, 9999);
        d = RandomRange(1000, 9999);
        snprintf(expression, size, "(%lld + %lld) * (%lld - %lld)", a, b, c, d);
        *result = (a + b) * (c - d);
        break;
    case 2:
        a = RandomRange(100, 999);
        b = RandomRange(100, 999);
        c = RandomRange(100, 999);
        d = RandomRange(100, 999);
        e = RandomRange(10000, 99999);
        snprintf(expression, size, "%lld * %lld - %lld * %lld + %lld", a, b, c, d, e);
        *result = a * b - c * d + e;
        break;
    default:
        a = RandomRange(100, 999);
        b = RandomRange(11, 99);
        c = RandomRange(11, 99);
        snprintf(expression, size, "%lld * %lld * %lld", a, b, c);
        *result = a * b * c;
        break;
    }
}

static const ExpressionBuilder EXPRESSION_BUILDERS[MATHPROBLEM_DIFFICULTY_COUNT] = {
    [MATHPROBLEM_DIFFICULTY_EASY]       = BuildEasyExpression,
    [MATHPROBLEM_DIFFICULTY_MEDIUM]     = BuildMediumExpression,
    [MATHPROBLEM_DIFFICULTY_HARD]       = BuildHardExpression,
    [MATHPROBLEM_DIFFICULTY_IMPOSSIBLE] = BuildImpossibleExpression
};

static void GenerateNormal(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const statements[] = {
        "Calcule o resultado da conta: ",
        "Efetue a operação: ",
        "Quanto é o resultado de: ",
        "Resolva a expressão: ",
        "Qual é o valor da conta: ",
        "Determine o resultado da seguinte operação: "
    };
    char expression[PROBLEM_SIZE] = "";
    long long result = 0;

    EXPRESSION_BUILDERS[difficulty](expression, sizeof expression, &result);
    WriteQuestion(draft, PICK_STATEMENT(statements), 0, expression);
    WriteAnswer(draft, result);
}

/* ======================================================================
 * MATHPROBLEM_EQUATION_1 - equação do 1º grau
 *
 * Cada construtor devolve a solução x e escreve a equação em `equation`.
 * ====================================================================== */

/* a*x + b = c */
static long long BuildSimpleLinear(char *equation, size_t size,
                                   long long coefficient_max, long long x_max, long long constant_max)
{
    long long x = RandomNonZero(-x_max, x_max);
    long long a = RandomRange(2, coefficient_max);
    long long b = RandomNonZero(-constant_max, constant_max);
    long long left[2] = {b, a};

    AppendPolynomial(equation, size, left, 1);
    Append(equation, size, " = %lld", a * x + b);
    return x;
}

/* a*x + b = c*x + d, com a != c */
static long long BuildBothSidesLinear(char *equation, size_t size,
                                      long long coefficient_max, long long x_max, long long constant_max)
{
    long long x = RandomNonZero(-x_max, x_max);
    long long a = RandomRange(2, coefficient_max);
    long long b = RandomNonZero(-constant_max, constant_max);
    long long c;
    long long left[2], right[2];

    do {
        c = RandomRange(1, coefficient_max);
    } while (c == a);

    left[0] = b;
    left[1] = a;
    right[0] = (a - c) * x + b;     /* garante que x seja a solução */
    right[1] = c;

    AppendPolynomial(equation, size, left, 1);
    Append(equation, size, " = ");
    AppendPolynomial(equation, size, right, 1);
    return x;
}

/* a(x + b) + c = d*x + e, com a != d */
static long long BuildParenthesizedLinear(char *equation, size_t size,
                                          long long coefficient_max, long long x_max, long long constant_max)
{
    long long x = RandomNonZero(-x_max, x_max);
    long long a = RandomRange(2, coefficient_max);
    long long b = RandomNonZero(-x_max, x_max);
    long long c = RandomNonZero(-constant_max, constant_max);
    long long d;
    long long right[2];

    do {
        d = RandomRange(1, coefficient_max);
    } while (d == a);

    right[0] = a * (x + b) + c - d * x;     /* garante que x seja a solução */
    right[1] = d;

    Append(equation, size, "%lld(x %c %lld) %c %lld = ",
           a, SignOf(b), Magnitude(b), SignOf(c), Magnitude(c));
    AppendPolynomial(equation, size, right, 1);
    return x;
}

static void GenerateLinearSolution(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const statements[] = {
        "Encontre o valor de x dessa equação: ",
        "Resolva a equação e determine o valor de x: ",
        "Qual é o valor de x que satisfaz a equação: ",
        "Determine a solução da equação: ",
        "Isole x e encontre o seu valor na equação: "
    };
    char equation[PROBLEM_SIZE] = "";
    long long x;

    switch (difficulty) {
    case MATHPROBLEM_DIFFICULTY_EASY:
        x = BuildSimpleLinear(equation, sizeof equation, 9, 10, 20);
        break;
    case MATHPROBLEM_DIFFICULTY_MEDIUM:
        x = BuildBothSidesLinear(equation, sizeof equation, 12, 15, 50);
        break;
    case MATHPROBLEM_DIFFICULTY_HARD:
        x = BuildParenthesizedLinear(equation, sizeof equation, 9, 20, 30);
        break;
    default:
        x = BuildParenthesizedLinear(equation, sizeof equation, 99, 999, 9999);
        break;
    }

    WriteQuestion(draft, PICK_STATEMENT(statements), 0, equation);
    WriteAnswer(draft, x);
}

static void GenerateEquation1(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    if (rand() % 3 == 0) {
        GenerateEvaluation(draft, difficulty, 1);
    } else {
        GenerateLinearSolution(draft, difficulty);
    }
}

/* ======================================================================
 * MATHPROBLEM_EQUATION_2 - equação do 2º grau
 * ====================================================================== */

/* a*x^2 + b*x + c = 0, construída a partir das raízes inteiras. */
static void GenerateQuadraticSolution(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const statements[] = {
        "Encontre as raízes dessa equação, da menor para a maior: ",
        "Determine as duas raízes da equação do 2º grau, em ordem crescente: ",
        "Resolva a equação e informe suas raízes, da menor para a maior: "
    };
    long long root_limit = ByDifficulty(difficulty, 9, 15, 25, 99);
    long long leading = RandomRange(1, ByDifficulty(difficulty, 1, 1, 4, 9));
    long long first = RandomRange(-root_limit, root_limit);
    long long second = RandomRange(-root_limit, root_limit);
    long long coefficients[3];
    char equation[PROBLEM_SIZE] = "";

    coefficients[2] = leading;
    coefficients[1] = -leading * (first + second);
    coefficients[0] = leading * first * second;

    AppendPolynomial(equation, sizeof equation, coefficients, 2);
    Append(equation, sizeof equation, " = 0");
    WriteQuestion(draft, PICK_STATEMENT(statements), 0, equation);
    WriteRootsAnswer(draft, first, second);
}

static void GenerateEquation2(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    if (rand() % 3 == 0) {
        GenerateEvaluation(draft, difficulty, 2);
    } else {
        GenerateQuadraticSolution(draft, difficulty);
    }
}

/* ======================================================================
 * MATHPROBLEM_NEWTON - binômio de Newton: (a*x + b)^n
 * ====================================================================== */

static void GenerateNewton(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const coefficient_statements[] = {
        "Qual é o coeficiente do termo em x^%lld no desenvolvimento de: ",
        "Usando o Binômio de Newton, determine o coeficiente de x^%lld em: ",
        "Desenvolvendo a potência abaixo, qual é o coeficiente de x^%lld? ",
        "Encontre o coeficiente do termo de grau %lld no desenvolvimento de: "
    };
    static const char *const sum_statements[] = {
        "Qual é a soma de todos os coeficientes do desenvolvimento de: ",
        "Usando o Binômio de Newton, calcule a soma dos coeficientes de: ",
        "Determine a soma dos coeficientes do polinômio obtido ao desenvolver: "
    };
    int n = (int)RandomRange(ByDifficulty(difficulty, 2, 4, 5, 8),
                             ByDifficulty(difficulty, 4, 6, 8, 12));
    long long a = RandomRange(1, ByDifficulty(difficulty, 1, 2, 3, 5));
    long long b = RandomRange(1, ByDifficulty(difficulty, 3, 3, 4, 9));
    long long base[2];
    char expression[PROBLEM_SIZE] = "";

    if (difficulty != MATHPROBLEM_DIFFICULTY_EASY && rand() % 2 == 0) {
        b = -b;
    }

    base[0] = b;
    base[1] = a;
    Append(expression, sizeof expression, "(");
    AppendPolynomial(expression, sizeof expression, base, 1);
    Append(expression, sizeof expression, ")^%d", n);

    if (rand() % 2 == 0) {
        /* termo geral: C(n, k) * a^k * b^(n-k) * x^k */
        int k = (int)RandomRange(1, n - 1);
        WriteQuestion(draft, PICK_STATEMENT(coefficient_statements), k, expression);
        WriteAnswer(draft, Binomial(n, k) * Power(a, k) * Power(b, n - k));
    } else {
        /* a soma dos coeficientes é o valor do polinômio em x = 1 */
        WriteQuestion(draft, PICK_STATEMENT(sum_statements), 0, expression);
        WriteAnswer(draft, Power(a + b, n));
    }
}

/* ======================================================================
 * MATHPROBLEM_FUNCTION_LINEAR - f(x) = a*x + b
 * ====================================================================== */

static void WriteLinearQuestion(MathProblem_Draft *draft, const char *statement,
                                long long argument, long long a, long long b)
{
    long long coefficients[2] = {b, a};
    char problem[PROBLEM_SIZE] = "f(x) = ";

    AppendPolynomial(problem, sizeof problem, coefficients, 1);
    WriteQuestion(draft, statement, argument, problem);
}

static void GenerateLinearFunction(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const evaluate_statements[] = {
        "Calcule o valor de f(%lld) na função linear: ",
        "Dada a função linear abaixo, qual é o valor de f(%lld)? ",
        "Determine f(%lld) para a função: "
    };
    static const char *const zero_statements[] = {
        "Encontre a raiz (o zero) da função linear: ",
        "Para qual valor de x a função abaixo se anula? ",
        "Em qual valor de x o gráfico da função abaixo cruza o eixo x? "
    };
    static const char *const inverse_statements[] = {
        "Para qual valor de x temos f(x) = %lld na função: ",
        "Determine x tal que f(x) = %lld, sendo: "
    };
    static const char *const slope_statements[] = {
        "Determine o coeficiente angular da reta que passa pelos pontos: ",
        "Qual é a inclinação (coeficiente angular) da reta que contém os pontos: "
    };
    long long slope_limit = ByDifficulty(difficulty, 5, 12, 40, 999);
    long long constant_limit = ByDifficulty(difficulty, 9, 30, 200, 9999);
    long long x_limit = ByDifficulty(difficulty, 5, 15, 50, 999);
    long long a = RandomNonZero(-slope_limit, slope_limit);
    long long b = RandomNonZero(-constant_limit, constant_limit);
    long long x = RandomNonZero(-x_limit, x_limit);

    switch (rand() % 4) {
    case 0:     /* valor da função em x */
        WriteLinearQuestion(draft, PICK_STATEMENT(evaluate_statements), x, a, b);
        WriteAnswer(draft, a * x + b);
        break;
    case 1:     /* raiz: escolhe-se x e ajusta-se b para que f(x) = 0 */
        WriteLinearQuestion(draft, PICK_STATEMENT(zero_statements), 0, a, -a * x);
        WriteAnswer(draft, x);
        break;
    case 2:     /* x para um valor dado de f(x) */
        WriteLinearQuestion(draft, PICK_STATEMENT(inverse_statements), a * x + b, a, b);
        WriteAnswer(draft, x);
        break;
    default: {  /* coeficiente angular a partir de dois pontos */
        long long second_x = x + RandomNonZero(-x_limit, x_limit);
        char points[PROBLEM_SIZE];

        snprintf(points, sizeof points, "(%lld, %lld) e (%lld, %lld)",
                 x, a * x + b, second_x, a * second_x + b);
        WriteQuestion(draft, PICK_STATEMENT(slope_statements), 0, points);
        WriteAnswer(draft, a);
        break;
    }
    }
}

/* ======================================================================
 * MATHPROBLEM_FUNCTION_QUADRACTIC - f(x) = a*x^2 + b*x + c
 * ====================================================================== */

typedef struct Quadratic {
    long long a, b, c;
} Quadratic;

/* Forma canônica: a(x - h)^2 + k = a*x^2 - 2ah*x + (a*h^2 + k) */
static Quadratic QuadraticFromVertex(long long a, long long h, long long k)
{
    Quadratic quadratic = { a, -2 * a * h, a * h * h + k };
    return quadratic;
}

/* Forma fatorada: a(x - r1)(x - r2) */
static Quadratic QuadraticFromRoots(long long a, long long first, long long second)
{
    Quadratic quadratic = { a, -a * (first + second), a * first * second };
    return quadratic;
}

static void WriteQuadraticQuestion(MathProblem_Draft *draft, const char *statement,
                                   long long argument, Quadratic quadratic)
{
    long long coefficients[3] = {quadratic.c, quadratic.b, quadratic.a};
    char problem[PROBLEM_SIZE] = "f(x) = ";

    AppendPolynomial(problem, sizeof problem, coefficients, 2);
    WriteQuestion(draft, statement, argument, problem);
}

static void GenerateQuadraticFunction(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const evaluate_statements[] = {
        "Calcule o valor de f(%lld) na função quadrática: ",
        "Dada a função abaixo, determine f(%lld): "
    };
    static const char *const vertex_x_statements[] = {
        "Determine a coordenada x do vértice da parábola: ",
        "Qual é o valor de x no vértice do gráfico da função: "
    };
    static const char *const vertex_y_statements[] = {
        "Determine a coordenada y do vértice (valor máximo ou mínimo) da função: ",
        "Qual é o valor máximo ou mínimo da função quadrática: "
    };
    static const char *const zeros_statements[] = {
        "Encontre os zeros da função, do menor para o maior: ",
        "Determine as raízes da função quadrática, em ordem crescente: "
    };
    long long leading_limit = ByDifficulty(difficulty, 2, 4, 9, 20);
    long long vertex_limit = ByDifficulty(difficulty, 3, 6, 15, 99);
    long long offset_limit = ByDifficulty(difficulty, 9, 30, 100, 999);
    long long root_limit = ByDifficulty(difficulty, 6, 12, 25, 99);
    long long x_limit = ByDifficulty(difficulty, 4, 10, 20, 99);
    long long a = RandomNonZero(-leading_limit, leading_limit);
    long long h = RandomNonZero(-vertex_limit, vertex_limit);
    long long k = RandomNonZero(-offset_limit, offset_limit);
    Quadratic quadratic = QuadraticFromVertex(a, h, k);

    switch (rand() % 4) {
    case 0: {   /* valor da função em x */
        long long x = RandomNonZero(-x_limit, x_limit);
        WriteQuadraticQuestion(draft, PICK_STATEMENT(evaluate_statements), x, quadratic);
        WriteAnswer(draft, quadratic.a * x * x + quadratic.b * x + quadratic.c);
        break;
    }
    case 1:     /* x do vértice */
        WriteQuadraticQuestion(draft, PICK_STATEMENT(vertex_x_statements), 0, quadratic);
        WriteAnswer(draft, h);
        break;
    case 2:     /* y do vértice */
        WriteQuadraticQuestion(draft, PICK_STATEMENT(vertex_y_statements), 0, quadratic);
        WriteAnswer(draft, k);
        break;
    default: {  /* zeros da função */
        long long first = RandomRange(-root_limit, root_limit);
        long long second = RandomRange(-root_limit, root_limit);
        WriteQuadraticQuestion(draft, PICK_STATEMENT(zeros_statements), 0,
                               QuadraticFromRoots(a, first, second));
        WriteRootsAnswer(draft, first, second);
        break;
    }
    }
}

/* ======================================================================
 * MATHPROBLEM_FUNCTION_EXPONENTIAL - f(x) = a * b^x
 * ====================================================================== */

static void GenerateExponentialFunction(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const evaluate_statements[] = {
        "Calcule o valor de f(%lld) na função exponencial: ",
        "Dada a função abaixo, determine f(%lld): "
    };
    static const char *const inverse_statements[] = {
        "Para qual valor de x temos f(x) = %lld na função exponencial: ",
        "Determine x tal que f(x) = %lld, sendo: "
    };
    long long base = RandomRange(2, ByDifficulty(difficulty, 3, 5, 9, 12));
    long long a = RandomRange(1, ByDifficulty(difficulty, 1, 5, 9, 99));
    int x = (int)RandomRange(2, ByDifficulty(difficulty, 5, 6, 8, 12));
    long long value = a * Power(base, x);
    char problem[PROBLEM_SIZE] = "f(x) = ";

    if (a != 1) {
        Append(problem, sizeof problem, "%lld * ", a);
    }
    Append(problem, sizeof problem, "%lld^x", base);

    if (rand() % 2 == 0) {
        WriteQuestion(draft, PICK_STATEMENT(evaluate_statements), x, problem);
        WriteAnswer(draft, value);
    } else {
        WriteQuestion(draft, PICK_STATEMENT(inverse_statements), value, problem);
        WriteAnswer(draft, x);
    }
}

/* ======================================================================
 * MATHPROBLEM_FUNCTION_LOGARITHMICA - f(x) = a * log_b(x) + c
 *
 * O argumento do logaritmo é sempre uma potência exata da base,
 * então a resposta é inteira.
 * ====================================================================== */

static void GenerateLogarithmicFunction(MathProblem_Draft *draft, MathProblem_Difficulty difficulty)
{
    static const char *const evaluate_statements[] = {
        "Calcule o valor de f(%lld) na função logarítmica: ",
        "Dada a função abaixo, determine f(%lld): "
    };
    static const char *const inverse_statements[] = {
        "Para qual valor de x temos f(x) = %lld na função logarítmica: ",
        "Determine x tal que f(x) = %lld, sendo: "
    };
    long long base = RandomRange(2, ByDifficulty(difficulty, 3, 5, 10, 20));
    int exponent = (int)RandomRange(1, ByDifficulty(difficulty, 4, 6, 8, 12));
    long long a_limit = ByDifficulty(difficulty, 1, 3, 5, 20);
    long long c_limit = ByDifficulty(difficulty, 0, 5, 20, 99);
    long long a = (a_limit == 1) ? 1 : RandomNonZero(-a_limit, a_limit);
    long long c = (c_limit == 0) ? 0 : RandomRange(-c_limit, c_limit);
    long long argument = Power(base, exponent);
    long long value = a * exponent + c;
    char problem[PROBLEM_SIZE] = "f(x) = ";

    if (a != 1) {
        Append(problem, sizeof problem, "%lld * ", a);
    }
    Append(problem, sizeof problem, "log_%lld(x)", base);
    if (c != 0) {
        Append(problem, sizeof problem, " %c %lld", SignOf(c), Magnitude(c));
    }

    if (rand() % 2 == 0) {
        WriteQuestion(draft, PICK_STATEMENT(evaluate_statements), argument, problem);
        WriteAnswer(draft, value);
    } else {
        WriteQuestion(draft, PICK_STATEMENT(inverse_statements), value, problem);
        WriteAnswer(draft, argument);
    }
}

/* ======================================================================
 * Tabela de geradores (indexada por MathProblem_Type)
 * ====================================================================== */

static const MathProblem_Generator GENERATORS[MATHPROBLEM_TYPE_COUNT] = {
    [MATHPROBLEM_NORMAL]                = GenerateNormal,
    [MATHPROBLEM_EQUATION_1]            = GenerateEquation1,
    [MATHPROBLEM_EQUATION_2]            = GenerateEquation2,
    [MATHPROBLEM_NEWTON]                = GenerateNewton,
    [MATHPROBLEM_FUNCTION_LINEAR]       = GenerateLinearFunction,
    [MATHPROBLEM_FUNCTION_QUADRACTIC]   = GenerateQuadraticFunction,
    [MATHPROBLEM_FUNCTION_EXPONENTIAL]  = GenerateExponentialFunction,
    [MATHPROBLEM_FUNCTION_LOGARITHMICA] = GenerateLogarithmicFunction
};

MathProblem_Generator MathProblem_GetGenerator(MathProblem_Type type)
{
    if ((int)type < 0 || (int)type >= MATHPROBLEM_TYPE_COUNT) {
        return NULL;
    }
    return GENERATORS[type];
}
