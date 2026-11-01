#include"monitor_draw.h"
#include"game/game_globals.h"

typedef struct DrawState {
    Uint8 r, g, b, a;
    SDL_BlendMode blend;
} DrawState;

static void drawBegin(SDL_Color c, DrawState *prev)
{
    SDL_GetRenderDrawColor(r, &prev->r, &prev->g, &prev->b, &prev->a);
    SDL_GetRenderDrawBlendMode(r, &prev->blend);

    SDL_SetRenderDrawBlendMode(r, (c.a == 255)? SDL_BLENDMODE_NONE: SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

static void drawEnd(const DrawState *prev)
{
    SDL_SetRenderDrawColor(r, prev->r, prev->g, prev->b, prev->a);
    SDL_SetRenderDrawBlendMode(r, prev->blend);
}

void MonitorDraw_Fill(const SDL_Rect *rect, SDL_Color color)
{
    DrawState prev;

    drawBegin(color, &prev);
    SDL_RenderFillRect(r, rect);
    drawEnd(&prev);
}

void MonitorDraw_Outline(const SDL_Rect *rect, int thickness, SDL_Color color)
{
    DrawState prev;
    SDL_Rect sides[4];
    int t;

    t = (thickness < 1)? 1: thickness;
    if(t * 2 > rect->w || t * 2 > rect->h){
        MonitorDraw_Fill(rect, color);
        return;
    }

    sides[0] = (SDL_Rect){rect->x,                rect->y,                 rect->w, t};        // top
    sides[1] = (SDL_Rect){rect->x,                rect->y + rect->h - t,   rect->w, t};        // bottom
    sides[2] = (SDL_Rect){rect->x,                rect->y + t,             t, rect->h - 2 * t}; // left
    sides[3] = (SDL_Rect){rect->x + rect->w - t,  rect->y + t,             t, rect->h - 2 * t}; // right

    drawBegin(color, &prev);
    SDL_RenderFillRects(r, sides, 4);
    drawEnd(&prev);
}

void MonitorDraw_Line(int x1, int y1, int x2, int y2, int thickness, SDL_Color color)
{
    DrawState prev;
    int t, i, steep;

    t = (thickness < 1)? 1: thickness;
    steep = abs(y2 - y1) > abs(x2 - x1);

    drawBegin(color, &prev);
    // A thick line is a bundle of parallel 1px lines shifted along the minor axis
    for(i = -(t - 1) / 2; i <= t / 2; ++i){
        if(steep){
            SDL_RenderDrawLine(r, x1 + i, y1, x2 + i, y2);
        }
        else {
            SDL_RenderDrawLine(r, x1, y1 + i, x2, y2 + i);
        }
    }
    drawEnd(&prev);
}
