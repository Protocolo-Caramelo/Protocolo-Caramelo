#include"desktop_icon.h"
#include"monitor_space.h"
#include"monitor_draw.h"
#include"game/game_globals.h"

// Vertical room reserved for the sprite inside the cell, and gap before the label
#define SPRITE_AREA_H   130
#define LABEL_GAP       8

static const SDL_Color COLOR_HOVER    = {255, 180, 27, 60};
static const SDL_Color COLOR_SELECTED = {255, 180, 27, 120};

DesktopIcon* DesktopIcon_Create(MonitorAppId app)
{
    const MonitorApp *def;
    DesktopIcon *icon;

    def = MonitorApps_Get(app);
    if(!def){
        return NULL;
    }

    icon = calloc(1, sizeof(DesktopIcon));
    if(!icon){
        return NULL;
    }
    icon->app = app;
    icon->cell = IconGrid_CellRect(def->icon_cell.x, def->icon_cell.y);

    icon->sprite = IMG_LoadTexture(r, def->icon_path);
    if(icon->sprite){
        SDL_SetTextureBlendMode(icon->sprite, SDL_BLENDMODE_BLEND);
        SDL_QueryTexture(icon->sprite, NULL, NULL, &icon->sprite_w, &icon->sprite_h);
    }
    else {
        printf("Error while loading icon '%s': %s\n", def->icon_path, SDL_GetError());
    }

    icon->label = MonitorText_Create(
        cache_fRegular,
        TextConfig_Create(
            .color        = (SDL_Color){0, 0, 0, 255},
            .scale_width  = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .wrap         = 135,
        ),
        def->icon_label
    );
    return icon;
}

void DesktopIcon_Destroy(DesktopIcon **icon)
{
    if(!icon || !*icon){
        return;
    }
    if((*icon)->sprite){
        SDL_DestroyTexture((*icon)->sprite);
    }
    MonitorText_Destroy(&(*icon)->label);
    free(*icon);
    *icon = NULL;
}

int DesktopIcon_Contains(const DesktopIcon *icon, int ux, int uy)
{
    SDL_Point p = {ux, uy};

    return SDL_PointInRect(&p, &icon->cell);
}

void DesktopIcon_MoveTo(DesktopIcon *icon, int ux, int uy, const SDL_Rect *bounds)
{
    int max_x, max_y;

    max_x = bounds->x + bounds->w - icon->cell.w;
    max_y = bounds->y + bounds->h - icon->cell.h;

    icon->cell.x = (ux < bounds->x)? bounds->x: (ux > max_x)? max_x: ux;
    icon->cell.y = (uy < bounds->y)? bounds->y: (uy > max_y)? max_y: uy;
}

void DesktopIcon_Draw(DesktopIcon *icon)
{
    SDL_Rect cell_px, sprite_units, sprite_px;
    double scale;

    scale = MonitorSpace_TextScale();

    // Highlight
    if(icon->selected || icon->hovered){
        cell_px = MonitorSpace_RectToScreen(&icon->cell);
        MonitorDraw_Fill(&cell_px, icon->selected? COLOR_SELECTED: COLOR_HOVER);
    }

    // Sprite: centered horizontally, vertically centered in its reserved area
    if(icon->sprite){
        sprite_units = (SDL_Rect){
            icon->cell.x + (icon->cell.w - icon->sprite_w) / 2,
            icon->cell.y + (SPRITE_AREA_H - icon->sprite_h) / 2,
            icon->sprite_w,
            icon->sprite_h
        };
        sprite_px = MonitorSpace_RectToScreen(&sprite_units);
        SDL_RenderCopy(r, icon->sprite, NULL, &sprite_px);
    }

    // Label: centered below the sprite
    if(icon->label){
        MonitorText_DrawCentered(
            icon->label,
            MonitorSpace_ToScreenX(icon->cell.x + icon->cell.w / 2),
            MonitorSpace_ToScreenY(icon->cell.y + SPRITE_AREA_H + LABEL_GAP),
            scale
        );
    }
}
