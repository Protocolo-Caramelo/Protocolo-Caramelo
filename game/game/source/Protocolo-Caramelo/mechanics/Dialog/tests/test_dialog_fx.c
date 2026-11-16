/*
 * Testes da matematica dos efeitos (sem SDL).
 *
 * Compilar e rodar (a partir desta pasta):
 *   gcc -std=c99 -Wall -Wextra -o /tmp/test_dialog_fx tests/test_dialog_fx.c dialog_fx.c -lm && /tmp/test_dialog_fx
 */
#include "../dialog_fx.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_checks = 0;
#define CHECK(cond) do { ++g_checks; if(!(cond)){ \
    printf("FALHOU %s:%d: %s\n", __FILE__, __LINE__, #cond); exit(1); } } while(0)

static int width_10(void *ctx, int cp)
{
    (void)ctx;
    (void)cp;
    return 10;
}

static void test_easing(void)
{
    DialogEaseFn fns[] = {
        DialogEase_Linear, DialogEase_OutQuad, DialogEase_OutCubic,
        DialogEase_OutBack, DialogEase_InOutSine
    };
    size_t i;

    for(i = 0; i < sizeof(fns) / sizeof(fns[0]); ++i){
        CHECK(fabs(fns[i](0.0)) < 1e-9);
        CHECK(fabs(fns[i](1.0) - 1.0) < 1e-9);
        CHECK(fabs(fns[i](-5.0)) < 1e-9);          /* clamp */
        CHECK(fabs(fns[i](9.0) - 1.0) < 1e-9);     /* clamp */
    }
    CHECK(DialogEase_OutCubic(0.5) > 0.5);          /* sobe rapido */
    CHECK(DialogEase_OutBack(0.7) > 1.0);           /* passa do alvo */
}

static void test_utf8(void)
{
    const char *s = "a\xC3\xA7\xC3\xA3o \xE2\x80\xA6";   /* "açao …" */
    const char *p = s;

    CHECK(DialogFx_CountChars("") == 0);
    CHECK(DialogFx_CountChars("abc") == 3);
    CHECK(DialogFx_CountChars(s) == 6);
    CHECK(DialogFx_NextCodepoint(&p) == 'a');
    CHECK(DialogFx_NextCodepoint(&p) == 0xE7);       /* ç */
    CHECK(DialogFx_NextCodepoint(&p) == 0xE3);       /* ã */
    CHECK(DialogFx_NextCodepoint(&p) == 'o');
    CHECK(DialogFx_NextCodepoint(&p) == ' ');
    CHECK(DialogFx_NextCodepoint(&p) == 0x2026);
    CHECK(*p == '\0');

    p = "\xFF" "a";                                  /* byte invalido */
    CHECK(DialogFx_NextCodepoint(&p) == 0xFFFD);
    CHECK(*p == 'a');
}

static void test_schedule(void)
{
    DialogRevealStyle st = {
        .char_interval_ms = 40, .char_duration_ms = 200,
        .pause_comma_ms = 100, .pause_stop_ms = 300, .rise_px = 8, .ease = NULL
    };
    double start[32];
    double total;
    size_t n;

    n = DialogFx_BuildSchedule("ab", &st, start, 32, &total);
    CHECK(n == 2);
    CHECK(start[0] == 0 && start[1] == 40);
    CHECK(fabs(total - (40 + 200)) < 1e-9);

    /* "a, b": a=0, ','=40, ' ' = 40+40+100 (pausa depois da virgula), b */
    n = DialogFx_BuildSchedule("a, b", &st, start, 32, &total);
    CHECK(n == 4);
    CHECK(start[1] == 40);
    CHECK(start[2] == 40 + 40 + 100);
    CHECK(start[3] == start[2] + 40);

    /* "3.14" nao pausa no ponto (ele nao termina a palavra) */
    n = DialogFx_BuildSchedule("3.14", &st, start, 32, &total);
    CHECK(n == 4);
    CHECK(start[3] == 120);

    /* Pontuacao final tambem pausa (afeta so o total? nao: nao ha proximo char) */
    n = DialogFx_BuildSchedule("oi.", &st, start, 32, &total);
    CHECK(n == 3 && start[2] == 80);

    /* Respeita a capacidade */
    n = DialogFx_BuildSchedule("abcdef", &st, start, 3, &total);
    CHECK(n == 3);

    n = DialogFx_BuildSchedule("", &st, start, 32, &total);
    CHECK(n == 0 && total == 0.0);
}

static void test_reveal(void)
{
    CHECK(DialogFx_Reveal(0, 100, 200, NULL) == 0.0);          /* ainda nao comecou */
    CHECK(DialogFx_Reveal(100, 100, 200, NULL) == 0.0);
    CHECK(fabs(DialogFx_Reveal(300, 100, 200, NULL) - 1.0) < 1e-9);
    CHECK(DialogFx_Reveal(500, 100, 200, NULL) == 1.0);
    CHECK(DialogFx_Reveal(150, 100, 0, NULL) == 1.0);          /* duracao 0 = instantaneo */
    CHECK(DialogFx_Reveal(50, 100, 0, NULL) == 0.0);

    CHECK(DialogFx_Alpha(0, 100, 200) == 0.0);
    CHECK(DialogFx_Alpha(100 + 200 * 0.6, 100, 200) == 1.0);   /* opaco antes do fim da subida */
    CHECK(DialogFx_Alpha(1000, 100, 200) == 1.0);
}

static void test_wave(void)
{
    DialogWaveStyle w = { .amplitude_px = 4, .frequency_hz = 1, .phase_per_char = 0.5 };
    double t;
    size_t i;

    CHECK(fabs(DialogFx_Wave(&w, 0, 0)) < 1e-9);
    for(t = 0; t < 3000; t += 37){
        for(i = 0; i < 20; ++i){
            CHECK(fabs(DialogFx_Wave(&w, t, i)) <= 4.0 + 1e-9);
        }
    }
    /* Letras vizinhas estao defasadas */
    CHECK(DialogFx_Wave(&w, 100, 0) != DialogFx_Wave(&w, 100, 1));
    w.amplitude_px = 0;
    CHECK(DialogFx_Wave(&w, 100, 3) == 0.0);
    CHECK(DialogFx_Wave(NULL, 100, 3) == 0.0);
}

static void test_jelly(void)
{
    DialogJellyStyle j = {
        .squash = 0.05, .sway_px = 6, .frequency_hz = 2.5, .sway_phase = 2.0,
        .ramp_in_ms = 100, .ramp_out_ms = 400, .strips = 24
    };
    double e = 0.0, sx, sy, t;
    int i;

    /* Energia sobe enquanto fala e fica em [0,1] */
    for(i = 0; i < 120; ++i){
        e = DialogFx_JellyEnergy(&j, e, 1, 16.0);
        CHECK(e >= 0.0 && e <= 1.0);
    }
    CHECK(e > 0.99);

    /* Decai quando para de falar, mais devagar do que subiu */
    {
        double e_in = DialogFx_JellyEnergy(&j, 0.0, 1, 100.0);
        double e_out = DialogFx_JellyEnergy(&j, 1.0, 0, 100.0);
        CHECK(e_in > 0.6);                 /* 1 - exp(-1) ~ 0.63 */
        CHECK(e_out > 0.7);                /* 1 - (1 - exp(-0.25)) ~ 0.78 */
    }
    for(i = 0; i < 300; ++i){
        e = DialogFx_JellyEnergy(&j, e, 0, 16.0);
    }
    CHECK(e < 0.01);

    /* Independe da taxa de quadros: 1x100ms == 10x10ms */
    {
        double a = DialogFx_JellyEnergy(&j, 0.0, 1, 100.0);
        double b = 0.0;
        for(i = 0; i < 10; ++i){
            b = DialogFx_JellyEnergy(&j, b, 1, 10.0);
        }
        CHECK(fabs(a - b) < 1e-9);
    }

    /* Sem energia: imagem em repouso */
    for(t = 0; t < 2000; t += 50){
        DialogFx_JellyScale(&j, 0.0, t, &sx, &sy);
        CHECK(fabs(sx - 1.0) < 1e-12 && fabs(sy - 1.0) < 1e-12);
        CHECK(fabs(DialogFx_JellySway(&j, 0.0, t, 0.0)) < 1e-12);
    }
    /* Com energia: limites respeitados, base parada, topo balanca */
    for(t = 0; t < 4000; t += 13){
        DialogFx_JellyScale(&j, 1.0, t, &sx, &sy);
        CHECK(sx >= 0.95 - 1e-9 && sx <= 1.05 + 1e-9);
        CHECK((sx - 1.0) * (sy - 1.0) <= 1e-12);       /* sinais opostos */
        CHECK(fabs(DialogFx_JellySway(&j, 1.0, t, 1.0)) < 1e-9);   /* base */
        CHECK(fabs(DialogFx_JellySway(&j, 1.0, t, 0.0)) <= 6.0 + 1e-9);
    }
}

static void test_wordwrap(void)
{
    char *out;

    /* cada char = 10px; max 50 => 5 chars por linha */
    out = DialogFx_WordWrap("ab cd ef", 50, 0, width_10, NULL);
    CHECK(strcmp(out, "ab cd\nef") == 0);
    free(out);

    /* nao quebra se cabe */
    out = DialogFx_WordWrap("ab cd", 50, 0, width_10, NULL);
    CHECK(strcmp(out, "ab cd") == 0);
    free(out);

    /* respeita '\n' existente e recomeca a contagem */
    out = DialogFx_WordWrap("abcd\nef gh ij", 50, 0, width_10, NULL);
    CHECK(strcmp(out, "abcd\nef gh\nij") == 0);
    free(out);

    /* palavra maior que a linha: nao inventa quebra (a libgraphical quebra por letra) */
    out = DialogFx_WordWrap("abcdefghij kl", 50, 0, width_10, NULL);
    CHECK(strcmp(out, "abcdefghij\nkl") == 0);
    free(out);

    /* chr_dist entra na conta: cada char ocupa 10+2 = 12px.
     * "abcd " ja ocupa 60px; o 'e' nao cabe -> quebra no espaco */
    out = DialogFx_WordWrap("abcd ef", 50, 2, width_10, NULL);
    CHECK(strcmp(out, "abcd\nef") == 0);
    free(out);
    out = DialogFx_WordWrap("abc de fg", 50, 2, width_10, NULL);
    CHECK(strcmp(out, "abc\nde\nfg") == 0);
    free(out);

    /* mesmo tamanho em bytes e UTF-8 preservado */
    out = DialogFx_WordWrap("a\xC3\xA7\xC3\xA3o b c d", 50, 0, width_10, NULL);
    CHECK(strlen(out) == strlen("a\xC3\xA7\xC3\xA3o b c d"));
    CHECK(DialogFx_CountChars(out) == 10);
    free(out);

    /* entradas degeneradas */
    CHECK(DialogFx_WordWrap(NULL, 50, 0, width_10, NULL) == NULL);
    out = DialogFx_WordWrap("ab cd ef", 0, 0, width_10, NULL);
    CHECK(strcmp(out, "ab cd ef") == 0);
    free(out);
    out = DialogFx_WordWrap("", 50, 0, width_10, NULL);
    CHECK(strcmp(out, "") == 0);
    free(out);
}

static void test_menu(void)
{
    /* navegacao circular */
    CHECK(DialogFx_MenuStep(0, 3, +1) == 1);
    CHECK(DialogFx_MenuStep(2, 3, +1) == 0);
    CHECK(DialogFx_MenuStep(0, 3, -1) == 2);
    CHECK(DialogFx_MenuStep(0, 1, +1) == 0);
    CHECK(DialogFx_MenuStep(7, 3, +1) == 1);      /* indice invalido volta ao comeco */
    CHECK(DialogFx_MenuStep(0, 0, +1) == 0);

    /* linha sob o mouse: 3 linhas de 20px comecando em y = 100 */
    CHECK(DialogFx_MenuRowAt(99, 100, 20, 3) == -1);
    CHECK(DialogFx_MenuRowAt(100, 100, 20, 3) == 0);
    CHECK(DialogFx_MenuRowAt(119, 100, 20, 3) == 0);
    CHECK(DialogFx_MenuRowAt(120, 100, 20, 3) == 1);
    CHECK(DialogFx_MenuRowAt(159, 100, 20, 3) == 2);
    CHECK(DialogFx_MenuRowAt(160, 100, 20, 3) == -1);
    CHECK(DialogFx_MenuRowAt(100, 100, 0, 3) == -1);

    CHECK(DialogFx_MenuStart(0, 80) == 0.0);
    CHECK(DialogFx_MenuStart(3, 80) == 240.0);
}

int main(void)
{
    test_easing();
    test_utf8();
    test_schedule();
    test_reveal();
    test_wave();
    test_jelly();
    test_wordwrap();
    test_menu();
    printf("OK: %d verificacoes passaram\n", g_checks);
    return 0;
}
