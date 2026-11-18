#include "dialog_fx.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FX_TWO_PI (2.0 * M_PI)

static double fx_clamp01(double v)
{
    return (v < 0.0)? 0.0: (v > 1.0)? 1.0: v;
}

/* ------------------------------------------------------------------ */
/* Easing                                                              */
/* ------------------------------------------------------------------ */
double DialogEase_Linear(double t)
{
    return fx_clamp01(t);
}

double DialogEase_OutQuad(double t)
{
    t = fx_clamp01(t);
    return 1.0 - (1.0 - t) * (1.0 - t);
}

double DialogEase_OutCubic(double t)
{
    t = fx_clamp01(t);
    return 1.0 - pow(1.0 - t, 3.0);
}

double DialogEase_OutBack(double t)
{
    const double c1 = 1.70158;
    const double c3 = c1 + 1.0;

    t = fx_clamp01(t);
    return 1.0 + c3 * pow(t - 1.0, 3.0) + c1 * pow(t - 1.0, 2.0);
}

double DialogEase_InOutSine(double t)
{
    t = fx_clamp01(t);
    return -(cos(M_PI * t) - 1.0) / 2.0;
}

/* ------------------------------------------------------------------ */
/* UTF-8                                                               */
/* ------------------------------------------------------------------ */
int DialogFx_NextCodepoint(const char **p)
{
    const unsigned char *s = (const unsigned char*)*p;
    int cp;
    int extra;
    int i;

    if(s[0] == '\0'){
        return 0;
    }
    if(s[0] < 0x80){
        *p += 1;
        return s[0];
    }
    if((s[0] & 0xE0) == 0xC0){
        cp = s[0] & 0x1F;
        extra = 1;
    }
    else if((s[0] & 0xF0) == 0xE0){
        cp = s[0] & 0x0F;
        extra = 2;
    }
    else if((s[0] & 0xF8) == 0xF0){
        cp = s[0] & 0x07;
        extra = 3;
    }
    else {
        *p += 1;
        return 0xFFFD;
    }

    for(i = 1; i <= extra; ++i){
        if((s[i] & 0xC0) != 0x80){
            *p += 1;
            return 0xFFFD;
        }
        cp = (cp << 6) | (s[i] & 0x3F);
    }
    *p += extra + 1;
    return cp;
}

size_t DialogFx_CountChars(const char *utf8)
{
    size_t n = 0;

    while(utf8 && *utf8){
        DialogFx_NextCodepoint(&utf8);
        ++n;
    }
    return n;
}

/* ------------------------------------------------------------------ */
/* Cronograma                                                          */
/* ------------------------------------------------------------------ */
static int fx_isSpaceLike(int cp)
{
    return cp == '\0' || cp == ' ' || cp == '\n' || cp == '\t';
}

static int fx_isCommaLike(int cp)
{
    return cp == ',' || cp == ';' || cp == ':';
}

static int fx_isStopLike(int cp)
{
    return cp == '.' || cp == '!' || cp == '?' || cp == 0x2026; /* U+2026 = ... */
}

size_t DialogFx_BuildSchedule(
    const char *utf8,
    const DialogRevealStyle *style,
    double *start_ms, size_t cap,
    double *total_ms
){
    const char *cursor = utf8;
    const char *peek;
    double t = 0.0;
    double last_start = 0.0;
    size_t n = 0;
    int cp, next;

    while(cursor && *cursor && n < cap){
        cp = DialogFx_NextCodepoint(&cursor);

        start_ms[n++] = t;
        last_start = t;
        t += style->char_interval_ms;

        /* Pausa so quando a pontuacao termina a palavra ("3.14" nao pausa) */
        peek = cursor;
        next = DialogFx_NextCodepoint(&peek);
        if(fx_isSpaceLike(next)){
            if(fx_isCommaLike(cp)){
                t += style->pause_comma_ms;
            }
            else if(fx_isStopLike(cp)){
                t += style->pause_stop_ms;
            }
        }
    }

    if(total_ms){
        *total_ms = (n > 0)? last_start + style->char_duration_ms: 0.0;
    }
    return n;
}

double DialogFx_Reveal(double elapsed_ms, double start_ms, double duration_ms, DialogEaseFn ease)
{
    double p;

    if(duration_ms <= 0.0){
        return (elapsed_ms >= start_ms)? 1.0: 0.0;
    }
    p = fx_clamp01((elapsed_ms - start_ms) / duration_ms);
    if(!ease){
        ease = DialogEase_OutCubic;
    }
    return ease(p);
}

double DialogFx_Alpha(double elapsed_ms, double start_ms, double duration_ms)
{
    double p;

    if(duration_ms <= 0.0){
        return (elapsed_ms >= start_ms)? 1.0: 0.0;
    }
    /* Opacidade completa aos 60% da animacao (smoothstep) */
    p = fx_clamp01((elapsed_ms - start_ms) / (duration_ms * 0.6));
    return p * p * (3.0 - 2.0 * p);
}

/* ------------------------------------------------------------------ */
/* Ondulacao                                                           */
/* ------------------------------------------------------------------ */
double DialogFx_Wave(const DialogWaveStyle *wave, double time_ms, double column)
{
    if(!wave || wave->amplitude_px == 0.0){
        return 0.0;
    }
    return wave->amplitude_px * sin(
        FX_TWO_PI * wave->frequency_hz * (time_ms / 1000.0)
        + wave->phase_per_char * column
    );
}

/* ------------------------------------------------------------------ */
/* Gelatina                                                            */
/* ------------------------------------------------------------------ */
double DialogFx_JellyEnergy(const DialogJellyStyle *j, double energy, int speaking, double dt_ms)
{
    double target = speaking? 1.0: 0.0;
    double tau = speaking? j->ramp_in_ms: j->ramp_out_ms;

    if(dt_ms <= 0.0){
        return energy;
    }
    if(tau <= 0.0){
        return target;
    }
    /* Aproximacao exponencial: independe da taxa de quadros */
    energy += (target - energy) * (1.0 - exp(-dt_ms / tau));
    return fx_clamp01(energy);
}

/* Duas senoides de frequencias diferentes dao um balanco menos "mecanico" */
static double fx_jellyWobble(const DialogJellyStyle *j, double time_ms)
{
    double w = FX_TWO_PI * j->frequency_hz * (time_ms / 1000.0);

    return 0.7 * sin(w) + 0.3 * sin(1.73 * w + 1.1);
}

void DialogFx_JellyScale(const DialogJellyStyle *j, double energy, double time_ms, double *sx, double *sy)
{
    double q = j->squash * energy * fx_jellyWobble(j, time_ms);

    /* Se estica na horizontal, achata na vertical (e vice-versa) */
    if(sx){
        *sx = 1.0 + q;
    }
    if(sy){
        *sy = 1.0 - 0.9 * q;
    }
}

double DialogFx_JellySway(const DialogJellyStyle *j, double energy, double time_ms, double y_frac)
{
    double weight;
    double w;

    y_frac = fx_clamp01(y_frac);
    /* 1 no topo, 0 na base: os "pes" ficam firmes e a cabeca balanca */
    weight = pow(1.0 - y_frac, 1.6);
    w = FX_TWO_PI * j->frequency_hz * 0.8 * (time_ms / 1000.0);

    return j->sway_px * energy * weight * sin(w - y_frac * j->sway_phase);
}

/* ------------------------------------------------------------------ */
/* Quebra de linha                                                     */
/* ------------------------------------------------------------------ */
char *DialogFx_WordWrap(
    const char *utf8, int max_width, int chr_dist,
    DialogGlyphWidthFn width_of, void *ctx
){
    size_t len;
    char *out;
    const char *cursor;
    const char *start;
    int cp, w;
    long last_space = -1;   /* indice (em bytes) do ultimo espaco da linha atual */
    int line_w = 0;         /* largura acumulada da linha atual */
    int since_space = 0;    /* largura do que vem depois do ultimo espaco */

    if(!utf8){
        return NULL;
    }
    len = strlen(utf8);
    out = malloc(len + 1);
    if(!out){
        return NULL;
    }
    memcpy(out, utf8, len + 1);

    if(max_width <= 0 || !width_of){
        return out;
    }

    start = utf8;
    cursor = utf8;
    while(*cursor){
        const char *here = cursor;

        cp = DialogFx_NextCodepoint(&cursor);
        if(cp == '\n'){
            line_w = since_space = 0;
            last_space = -1;
            continue;
        }

        w = width_of(ctx, cp);

        if(cp == ' '){
            last_space = (long)(here - start);
            line_w += w + chr_dist;
            since_space = 0;
            continue;
        }

        /* Mesmo criterio da libgraphical: x + largura > wrap */
        if(line_w + w > max_width && last_space >= 0){
            out[last_space] = '\n';
            line_w = since_space;
            last_space = -1;
        }
        line_w += w + chr_dist;
        since_space += w + chr_dist;
    }

    return out;
}

/* ------------------------------------------------------------------ */
/* Menu de opcoes                                                      */
/* ------------------------------------------------------------------ */
int DialogFx_MenuStep(int current, int count, int step)
{
    int next;

    if(count <= 0){
        return 0;
    }
    if(current < 0 || current >= count){
        current = 0;
    }
    next = (current + step) % count;
    return (next < 0)? next + count: next;
}

int DialogFx_MenuRowAt(int y, int top, int row_h, int count)
{
    if(row_h <= 0 || count <= 0 || y < top){
        return -1;
    }
    y = (y - top) / row_h;
    return (y < count)? y: -1;
}

double DialogFx_MenuStart(int index, double stagger_ms)
{
    return (index > 0)? index * stagger_ms: 0.0;
}
