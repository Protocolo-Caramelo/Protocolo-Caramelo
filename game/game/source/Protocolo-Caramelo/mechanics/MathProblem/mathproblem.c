#include "mathproblem.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mathproblem_generators.h"

/* ======================================================================
 * Tempo e aleatoriedade
 * ====================================================================== */

/*
 * Segundos desde o início do programa.
 * clock() mede o tempo de CPU do processo; se o seu programa usa SDL, pode
 * trocar por SDL_GetTicks() / 1000.0 para medir o tempo real.
 */
static double SecondsSinceProgramStart(void)
{
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

/* Semeia o rand() uma única vez, na primeira geração de problema. */
static void SeedRandomOnce(void)
{
    static bool seeded = false;

    if (!seeded) {
        srand((unsigned)time(NULL) ^ (unsigned)clock());
        seeded = true;
    }
}

static void FillRandomBytes(uint8_t *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        buffer[i] = (uint8_t)(rand() >> 3);
    }
}

/* memset que o compilador não pode remover como "código morto". */
static void SecureZero(void *buffer, size_t size)
{
    volatile uint8_t *bytes = (volatile uint8_t *)buffer;
    while (size-- > 0) {
        *bytes++ = 0;
    }
}

/* ======================================================================
 * Hexadecimal (a resposta cifrada é guardada como texto)
 * ====================================================================== */

static void BytesToHex(const uint8_t *bytes, size_t length, char *hex)
{
    static const char digits[] = "0123456789abcdef";

    for (size_t i = 0; i < length; i++) {
        hex[2 * i]     = digits[bytes[i] >> 4];
        hex[2 * i + 1] = digits[bytes[i] & 0x0f];
    }
    hex[2 * length] = '\0';
}

/* Devolve o valor de um dígito hexadecimal, ou -1 se o caractere for inválido. */
static int HexDigitValue(char digit)
{
    if (digit >= '0' && digit <= '9') return digit - '0';
    if (digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
    if (digit >= 'A' && digit <= 'F') return digit - 'A' + 10;
    return -1;
}

/* Converte o texto hexadecimal em bytes. Devolve quantos bytes foram gravados. */
static size_t HexToBytes(const char *hex, uint8_t *bytes, size_t capacity)
{
    size_t length = 0;

    while (length < capacity && hex[2 * length] != '\0' && hex[2 * length + 1] != '\0') {
        int high = HexDigitValue(hex[2 * length]);
        int low  = HexDigitValue(hex[2 * length + 1]);
        if (high < 0 || low < 0) {
            break;
        }
        bytes[length++] = (uint8_t)((high << 4) | low);
    }
    return length;
}

/* ======================================================================
 * Criptografia da resposta
 * ====================================================================== */

/* Gera uma chave e um IV novos e grava a resposta cifrada (em hexadecimal). */
static void EncryptAnswer(MathProblem *problem, const char *plain_answer)
{
    uint8_t cipher[MATHPROBLEM_ANSWER_SIZE];
    size_t length = strlen(plain_answer);

    FillRandomBytes(problem->key, sizeof problem->key);
    FillRandomBytes(problem->iv, sizeof problem->iv);

    AES128_CtrCrypt(problem->key, problem->iv, (const uint8_t *)plain_answer, cipher, length);
    BytesToHex(cipher, length, problem->response);
}

/* Descriptografa a resposta para `plain_answer` (buffer de MATHPROBLEM_ANSWER_SIZE bytes). */
static void DecryptAnswer(const MathProblem *problem, char *plain_answer)
{
    uint8_t cipher[MATHPROBLEM_ANSWER_SIZE];
    size_t length = HexToBytes(problem->response, cipher, MATHPROBLEM_ANSWER_SIZE - 1);

    AES128_CtrCrypt(problem->key, problem->iv, cipher, (uint8_t *)plain_answer, length);
    plain_answer[length] = '\0';
}

/* ======================================================================
 * API pública
 * ====================================================================== */

void MathProblem_GenProblem(MathProblem *problem, MathProblem_Type type, MathProblem_Difficulty difficulty)
{
    MathProblem_Generator generator = MathProblem_GetGenerator(type);
    MathProblem_Draft draft;

    if (problem == NULL || generator == NULL ||
        (int)difficulty < 0 || (int)difficulty >= MATHPROBLEM_DIFFICULTY_COUNT) {
        return;
    }

    SeedRandomOnce();
    generator(&draft, difficulty);

    memset(problem, 0, sizeof *problem);
    memcpy(problem->question, draft.question, sizeof problem->question);
    EncryptAnswer(problem, draft.answer);
    problem->time = SecondsSinceProgramStart();
    problem->type = type;
    problem->difficulty = difficulty;

    SecureZero(&draft, sizeof draft);
}

void MathProblem_GetProblem(MathProblem *problem)
{
    if (problem == NULL) {
        return;
    }
    DecryptAnswer(problem, problem->answer);
}

void MathProblem_HideAnswer(MathProblem *problem)
{
    if (problem == NULL) {
        return;
    }
    SecureZero(problem->answer, sizeof problem->answer);
}

bool MathProblem_CheckAnswer(const MathProblem *problem, const char *attempt)
{
    char plain_answer[MATHPROBLEM_ANSWER_SIZE];
    bool correct;

    if (problem == NULL || attempt == NULL) {
        return false;
    }

    DecryptAnswer(problem, plain_answer);
    correct = (strcmp(plain_answer, attempt) == 0);
    SecureZero(plain_answer, sizeof plain_answer);
    return correct;
}
