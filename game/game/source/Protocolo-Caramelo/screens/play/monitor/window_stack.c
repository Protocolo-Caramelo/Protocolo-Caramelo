#include"window_stack.h"

static DesktopWindow *z_order[MONITOR_APP_COUNT];
static DesktopWindow *open_order[MONITOR_APP_COUNT];
static int count;

static int indexIn(DesktopWindow *const *list, const DesktopWindow *win)
{
    int i;

    for(i = 0; i < count; ++i){
        if(list[i] == win){
            return i;
        }
    }
    return -1;
}

// Removes list[index], shifting the following entries down
static void removeAt(DesktopWindow **list, int index)
{
    int i;

    for(i = index; i < count - 1; ++i){
        list[i] = list[i + 1];
    }
    list[count - 1] = NULL;
}

int WindowStack_Count(void)
{
    return count;
}

DesktopWindow* WindowStack_AtZ(int i)
{
    return (i >= 0 && i < count)? z_order[i]: NULL;
}

DesktopWindow* WindowStack_AtOpenOrder(int i)
{
    return (i >= 0 && i < count)? open_order[i]: NULL;
}

DesktopWindow* WindowStack_Find(int app)
{
    int i;

    for(i = 0; i < count; ++i){
        if((int)z_order[i]->app == app){
            return z_order[i];
        }
    }
    return NULL;
}

int WindowStack_Contains(const DesktopWindow *win)
{
    return indexIn(z_order, win) >= 0;
}

int WindowStack_Add(DesktopWindow *win)
{
    if(count >= MONITOR_APP_COUNT){
        return 0;
    }
    z_order[count] = win;
    open_order[count] = win;
    ++count;
    return 1;
}

void WindowStack_Remove(const DesktopWindow *win)
{
    int z, opened;

    z = indexIn(z_order, win);
    if(z < 0){
        return;
    }
    opened = indexIn(open_order, win);

    removeAt(z_order, z);
    removeAt(open_order, opened);
    --count;
}

void WindowStack_BringToFront(const DesktopWindow *win)
{
    DesktopWindow *moved;
    int index;

    index = indexIn(z_order, win);
    if(index < 0){
        return;
    }
    moved = z_order[index];
    removeAt(z_order, index);
    z_order[count - 1] = moved;
}

DesktopWindow* WindowStack_Active(void)
{
    int i;

    for(i = count - 1; i >= 0; --i){
        if(!z_order[i]->minimized){
            return z_order[i];
        }
    }
    return NULL;
}

DesktopWindow* WindowStack_TopmostAt(int ux, int uy, DesktopWindowHit *hit)
{
    int i;

    for(i = count - 1; i >= 0; --i){
        *hit = DesktopWindow_HitTest(z_order[i], ux, uy);
        if(hit->part != DWIN_PART_NONE){
            return z_order[i];
        }
    }
    return NULL;
}

int WindowStack_Copy(DesktopWindow **out)
{
    memcpy(out, z_order, count * sizeof(z_order[0]));
    return count;
}
