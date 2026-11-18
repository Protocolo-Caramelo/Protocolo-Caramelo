#include "dialog_style.h"
#include "dialog.h"

#include <math.h>
#include <string.h>

DialogStyle dialog_styleDefault;
DialogStyle dialog_styleWhisper;
DialogStyle dialog_styleShout;

/* ------------------------------------------------------------------ */
/* Pequenos auxiliares                                                 */
/* ------------------------------------------------------------------ */
static Uint8 style_lerp8(Uint8 a, Uint8 b, double k)
{
    return (Uint8)lround(a + (b - a) * k);
}

static SDL_Color style_hsv(double h, double s, double v)
{
    double c = v * s;
    double hh = fmod(h, 360.0) / 60.0;
    double x = c * (1.0 - fabs(fmod(hh, 2.0) - 1.0));
    double m = v - c;
    double r = 0, g = 0, b = 0;
    SDL_Color out;

    if(hh < 1){ r = c; g = x; }
    else if(hh < 2){ r = x; g = c; }
    else if(hh < 3){ g = c; b = x; }
    else if(hh < 4){ g = x; b = c; }
    else if(hh < 5){ r = x; b = c; }
    else { r = c; b = x; }

    out.r = (Uint8)lround((r + m) * 255.0);
    out.g = (Uint8)lround((g + m) * 255.0);
    out.b = (Uint8)lround((b + m) * 255.0);
    out.a = 255;
    return out;
}

/* ------------------------------------------------------------------ */
/* Decoradores de caixa (func_alterPadding / func_alterBorder)         */
/* ------------------------------------------------------------------ */
void DialogDecor_PaddingGradient(Text *t, Texture *tex)
{
    const DialogStyle *st = Dialog_CurrentStyle();
    SDL_Color top, bottom;
    int w, h, y;
    double k;

    (void)t;
    if(!st || !tex || !tex->raw){
        return;
    }
    SDL_QueryTexture(tex->raw, NULL, NULL, &w, &h);
    top = st->box.padding.color;
    bottom = st->box.accent;

    /* O alvo de renderizacao ja e a textura do preenchimento */
    for(y = 0; y < h; ++y){
        k = (h > 1)? y / (double)(h - 1): 0.0;
        SDL_SetRenderDrawColor(
            tex->renderer,
            style_lerp8(top.r, bottom.r, k),
            style_lerp8(top.g, bottom.g, k),
            style_lerp8(top.b, bottom.b, k),
            style_lerp8(top.a, bottom.a, k)
        );
        SDL_RenderDrawLine(tex->renderer, 0, y, w - 1, y);
    }
}

void DialogDecor_BorderPulse(Text *t, Texture *tex)
{
    const DialogStyle *st = Dialog_CurrentStyle();
    double k;

    (void)t;
    if(!st || !tex || !tex->renderer){
        return;
    }
    k = 0.5 + 0.5 * sin(2.0 * M_PI * 2.2 * (SDL_GetTicks() / 1000.0));
    SDL_SetRenderDrawColor(
        tex->renderer,
        style_lerp8(st->box.border.color.r, st->box.accent.r, k),
        style_lerp8(st->box.border.color.g, st->box.accent.g, k),
        style_lerp8(st->box.border.color.b, st->box.accent.b, k),
        255
    );
    SDL_RenderClear(tex->renderer);
}

/* ------------------------------------------------------------------ */
/* Ganchos por letra (alter_char)                                      */
/* ------------------------------------------------------------------ */
void DialogChar_Shake(TextChar *c, const DialogCharInfo *info)
{
    /* So deslocamentos positivos em x: a area do texto nao tem folga a esquerda.
     * Em y a folga e dada por DialogStyle.box.text_slack. */
    c->x += (1.5 + 1.5 * sin(info->time_ms * 0.09 + info->index * 12.9898)) * info->ui_scale;
    c->y += 1.2 * sin(info->time_ms * 0.11 + info->index * 78.233) * info->ui_scale;
}

void DialogChar_Rainbow(TextChar *c, const DialogCharInfo *info)
{
    Uint8 alpha = c->color.a;

    c->color = style_hsv(info->time_ms * 0.12 + info->index * 22.0, 0.55, 1.0);
    c->color.a = alpha;
}

/* ------------------------------------------------------------------ */
/* Estilos prontos                                                     */
/* ------------------------------------------------------------------ */
DialogStyle DialogStyle_Make(void)
{
    DialogStyle s;

    memset(&s, 0, sizeof(s));

    /* Caixa */
    s.box.screen_margin = 24;
    s.box.bottom_gap    = 20;
    s.box.lines         = 3;
    s.box.border  = (Border){
        .top = 4, .right = 4, .bottom = 4, .left = 4,
        .color = (SDL_Color){235, 235, 245, 255}
    };
    s.box.padding = (Padding){
        .top = 14, .right = 18, .bottom = 14, .left = 18,
        .color = (SDL_Color){12, 14, 28, 240}
    };
    s.box.accent       = (SDL_Color){30, 36, 78, 240};
    s.box.alterPadding = DialogDecor_PaddingGradient;

    /* Texto */
    s.text.font  = GFONT_REGULAR;
    s.text.scale = 1 / 3.6;
    s.text.color = (SDL_Color){245, 245, 255, 255};
    s.text.fmt   = CHAR_FMT_REGULAR;

    /* Aparicao das letras */
    s.reveal = (DialogRevealStyle){
        .char_interval_ms = 38,
        .char_duration_ms = 260,
        .rise_px          = 10,
        .pause_comma_ms   = 120,
        .pause_stop_ms    = 260,
        .ease             = DialogEase_OutCubic
    };

    /* Ondulacao */
    s.wave = (DialogWaveStyle){
        .amplitude_px   = 2.5,
        .frequency_hz   = 1.1,
        .phase_per_char = 0.55
    };

    /* Rosto */
    s.face.side  = DIALOG_FACE_LEFT;
    s.face.inset = 8;
    s.face.gap   = 16;
    s.face.jelly = (DialogJellyStyle){
        .squash       = 0.045,
        .sway_px      = 5,
        .frequency_hz = 2.6,
        .sway_phase   = 2.2,
        .ramp_in_ms   = 90,
        .ramp_out_ms  = 420,
        .strips       = 28
    };

    /* Setinha de "continuar" */
    s.prompt = (DialogPromptStyle){
        .size = 9, .color = (SDL_Color){245, 245, 255, 255}, .bounce_px = 4
    };

    /* Menu de opcoes */
    s.choice = (DialogChoiceStyle){
        .align            = DIALOG_CHOICE_ALIGN_RIGHT,
        .gap              = 10,
        .min_width        = 260,
        .row_gap          = 6,
        .idle_color       = (SDL_Color){150, 156, 190, 255},
        .selected_color   = (SDL_Color){255, 236, 120, 255},
        .marker_size      = 9,
        .marker_gap       = 12,
        .marker_color     = (SDL_Color){255, 236, 120, 255},
        .marker_bounce_px = 3,
        .selected_wave    = { .amplitude_px = 2.2, .frequency_hz = 1.6, .phase_per_char = 0.6 },
        .delay_ms         = 220,
        .stagger_ms       = 90,
        .duration_ms      = 260,
        .rise_px          = 8,
        .alter_char       = NULL
    };

    s.alter_char = NULL;
    return s;
}

void DialogStyles_Init(void)
{
    dialog_styleDefault = DialogStyle_Make();

    /* Sussurro: mais lento, apagado e quase parado */
    dialog_styleWhisper = DialogStyle_Make();
    dialog_styleWhisper.text.color            = (SDL_Color){170, 176, 205, 255};
    dialog_styleWhisper.box.border.color      = (SDL_Color){110, 116, 150, 255};
    dialog_styleWhisper.reveal.char_interval_ms = 62;
    dialog_styleWhisper.reveal.char_duration_ms = 380;
    dialog_styleWhisper.reveal.rise_px          = 6;
    dialog_styleWhisper.wave.amplitude_px       = 1.2;
    dialog_styleWhisper.wave.frequency_hz       = 0.7;
    dialog_styleWhisper.face.jelly.squash       = 0.025;
    dialog_styleWhisper.face.jelly.sway_px      = 2.5;
    dialog_styleWhisper.face.jelly.frequency_hz = 1.6;
    dialog_styleWhisper.choice.idle_color       = (SDL_Color){110, 116, 150, 255};
    dialog_styleWhisper.choice.selected_color   = (SDL_Color){200, 206, 235, 255};
    dialog_styleWhisper.choice.marker_color     = (SDL_Color){200, 206, 235, 255};
    dialog_styleWhisper.choice.stagger_ms       = 140;

    /* Grito: rapido, tremendo, com a borda pulsando em vermelho */
    dialog_styleShout = DialogStyle_Make();
    dialog_styleShout.text.color                 = (SDL_Color){255, 226, 226, 255};
    dialog_styleShout.box.border.color           = (SDL_Color){255, 70, 70, 255};
    dialog_styleShout.box.padding.color          = (SDL_Color){50, 10, 16, 240};
    dialog_styleShout.box.accent                 = (SDL_Color){150, 20, 30, 255};
    dialog_styleShout.box.alterBorder            = DialogDecor_BorderPulse;
    dialog_styleShout.box.text_slack             = 2;
    dialog_styleShout.reveal.char_interval_ms    = 22;
    dialog_styleShout.reveal.char_duration_ms    = 180;
    dialog_styleShout.reveal.rise_px             = 14;
    dialog_styleShout.reveal.pause_comma_ms      = 40;
    dialog_styleShout.reveal.pause_stop_ms       = 120;
    dialog_styleShout.reveal.ease                = DialogEase_OutBack;
    dialog_styleShout.wave.amplitude_px          = 4.5;
    dialog_styleShout.wave.frequency_hz          = 2.4;
    dialog_styleShout.wave.phase_per_char        = 0.9;
    dialog_styleShout.face.jelly.squash          = 0.09;
    dialog_styleShout.face.jelly.sway_px         = 9;
    dialog_styleShout.face.jelly.frequency_hz    = 4.4;
    dialog_styleShout.face.jelly.ramp_in_ms      = 40;
    dialog_styleShout.face.jelly.ramp_out_ms     = 600;
    dialog_styleShout.face.jelly.strips          = 32;
    dialog_styleShout.alter_char                 = DialogChar_Shake;
    dialog_styleShout.choice.idle_color          = (SDL_Color){190, 120, 120, 255};
    dialog_styleShout.choice.selected_color      = (SDL_Color){255, 90, 90, 255};
    dialog_styleShout.choice.marker_color        = (SDL_Color){255, 90, 90, 255};
    dialog_styleShout.choice.selected_wave.amplitude_px = 3.4;
    dialog_styleShout.choice.selected_wave.frequency_hz = 2.6;
    dialog_styleShout.choice.stagger_ms          = 50;
}
