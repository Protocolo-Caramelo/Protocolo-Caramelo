#ifndef GWINDOW_H
#define GWINDOW_H

#include"GCommons.h"
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_events.h>

typedef struct WindowVTable WindowVTable;
typedef struct Window Window;

/*
 * WindowVTable struct
 *
 * Callbacks opcionais: os wrappers Window_* ignoram silenciosamente
 * os ponteiros que forem NULL.
 */
struct WindowVTable{
    void (*init)(Window*);
    void (*exit)(Window*);
    void (*render)(Window*);
    void (*present)(Window*);

    void (*event)(Window*, SDL_Event);
};

/*
 * Window struct
 */
struct Window{
    WindowVTable vtable;
    SDL_Window *raw;
    SDL_Renderer *renderer;
};

static inline WindowVTable* Window_vtable(Window*w)
{
    return &w->vtable;
}

static inline SDL_Window** Window_raw(Window*w)
{
    return &w->raw;
}

static inline SDL_Renderer** Window_renderer(Window*w)
{
    return &w->renderer;
}


static inline void Window_init(Window*w)
{
    if(w->vtable.init){
        w->vtable.init(w);
    }
}

static inline void Window_exit(Window*w)
{
    if(w->vtable.exit){
        w->vtable.exit(w);
    }
}

static inline void Window_render(Window*w)
{
    if(w->vtable.render){
        w->vtable.render(w);
    }
}

static inline void Window_present(Window*w)
{
    if(w->vtable.present){
        w->vtable.present(w);
    }
}

static inline void Window_event(Window*w, SDL_Event e)
{
    if(w->vtable.event){
        w->vtable.event(w, e);
    }
}

#endif
