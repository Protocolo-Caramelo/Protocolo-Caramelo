#include"click_counter.h"

int ClickCounter_Register(ClickCounter *counter, const void *target)
{
    Uint32 now;
    int consecutive;

    now = SDL_GetTicks();
    consecutive = counter->count > 0
        && counter->target == target
        && now - counter->time <= CLICK_MULTI_MS;

    counter->count = consecutive? counter->count + 1: 1;
    counter->target = target;
    counter->time = now;
    return counter->count;
}

void ClickCounter_Reset(ClickCounter *counter)
{
    memset(counter, 0, sizeof(*counter));
}
