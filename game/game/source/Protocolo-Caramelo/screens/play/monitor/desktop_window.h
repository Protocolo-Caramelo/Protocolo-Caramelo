#ifndef DESKTOP_WINDOW_H
#define DESKTOP_WINDOW_H

#include"monitor_apps.h"
#include"monitor_text.h"

/*
 * DesktopWindow: a window that lives inside the monitor (not an OS window).
 * Geometry, state and hit-testing are pure math in monitor units; only
 * DesktopWindow_Draw touches the renderer. The window manager decides *when*
 * to move/resize/maximize/close, this module knows *how*.
 */

// Layout, in monitor units
#define DWIN_BORDER         4   // outline thickness
#define DWIN_TITLE_H        64
#define DWIN_BUTTON_SIZE    40  // minimize / maximize / close
#define DWIN_BUTTON_MARGIN  12  // between the buttons and the window edge
#define DWIN_BUTTON_GAP     8   // between two buttons
#define DWIN_RESIZE_GRAB    16  // grabbable strip along the edges (inside the window)
#define DWIN_RESIZE_OUTSIDE 4   // extra grabbable strip just outside the window
#define DWIN_SPAWN_MARGIN   60  // where the first window appears, from the desktop corner

// Which edges a resize drag is holding (can be combined for corners)
enum {
    DWIN_EDGE_LEFT   = 1,
    DWIN_EDGE_RIGHT  = 2,
    DWIN_EDGE_TOP    = 4,
    DWIN_EDGE_BOTTOM = 8
};

typedef enum DesktopWindowPart DesktopWindowPart;
enum DesktopWindowPart {
    DWIN_PART_NONE,     // outside the window (or window minimized)
    DWIN_PART_BODY,
    DWIN_PART_TITLE,
    DWIN_PART_MINIMIZE, // title bar buttons
    DWIN_PART_MAXIMIZE,
    DWIN_PART_CLOSE,
    DWIN_PART_RESIZE    // see DesktopWindowHit.edges
};

typedef struct DesktopWindowHit DesktopWindowHit;
struct DesktopWindowHit {
    DesktopWindowPart part;
    int edges;
};

typedef enum DesktopWindowDrag DesktopWindowDrag;
enum DesktopWindowDrag {
    DWIN_DRAG_NONE,
    DWIN_DRAG_MOVE,
    DWIN_DRAG_RESIZE
};

typedef struct DesktopWindow DesktopWindow;
struct DesktopWindow {
    MonitorAppId app;
    const MonitorApp *def;

    SDL_Rect rect;          // outer rect, in monitor units
    int min_w, min_h;

    // State. A minimized window keeps its maximized flag (it comes back as it was)
    int maximized;
    int minimized;
    SDL_Rect restore_rect;  // where to go back to when un-maximizing

    MonitorText *title;
    MonitorText *placeholder;

    // Drag in progress
    DesktopWindowDrag drag;
    int resize_edges;
    int grab_dx, grab_dy;       // MOVE: cursor offset inside the window
    SDL_Rect start_rect;        // window rect when the drag began
    int start_x, start_y;       // RESIZE: cursor position when the drag began
};

// Creates the window near the top-left corner of bounds, shifted by (cascade, cascade)
DesktopWindow* DesktopWindow_Create(MonitorAppId app, const SDL_Rect *bounds, int cascade);
void DesktopWindow_Destroy(DesktopWindow **win);

// Areas of the window, in monitor units
SDL_Rect DesktopWindow_TitleRect(const DesktopWindow *win);
SDL_Rect DesktopWindow_BodyRect(const DesktopWindow *win);
// part must be DWIN_PART_MINIMIZE, DWIN_PART_MAXIMIZE or DWIN_PART_CLOSE
SDL_Rect DesktopWindow_ButtonRect(const DesktopWindow *win, DesktopWindowPart part);

DesktopWindowHit DesktopWindow_HitTest(const DesktopWindow *win, int ux, int uy);

// Keeps the whole window inside bounds
void DesktopWindow_Clamp(DesktopWindow *win, const SDL_Rect *bounds);

// State changes
void DesktopWindow_Maximize(DesktopWindow *win, const SDL_Rect *bounds);
void DesktopWindow_Restore(DesktopWindow *win);
void DesktopWindow_SetMinimized(DesktopWindow *win, int minimized);

// Dragging. The Begin functions return 0 when the window cannot be dragged now
// (maximized windows are fixed in place).
int DesktopWindow_BeginMove(DesktopWindow *win, int ux, int uy);
int DesktopWindow_BeginResize(DesktopWindow *win, int edges, int ux, int uy);
// Applies the current drag for a cursor at (ux, uy)
void DesktopWindow_DragTo(DesktopWindow *win, int ux, int uy, const SDL_Rect *bounds);
void DesktopWindow_EndDrag(DesktopWindow *win);
// 1 if a move drag changed the window position
int DesktopWindow_DragMoved(const DesktopWindow *win);

void DesktopWindow_Draw(DesktopWindow *win, int active);

#endif
