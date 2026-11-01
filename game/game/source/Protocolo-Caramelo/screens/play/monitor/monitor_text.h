#ifndef MONITOR_TEXT_H
#define MONITOR_TEXT_H

#include<graphical.h>

/*
 * MonitorText: a Text plus the TextConfig it was designed with, in monitor units.
 * Every draw scales that config to the current monitor size, so callers never
 * touch scale/wrap by hand.
 */
typedef struct MonitorText MonitorText;
struct MonitorText {
    Text *text;
    TextConfig config;      // designed in monitor units
    double applied_scale;   // scale the Text was last configured with
};

MonitorText* MonitorText_Create(TextCache *cache, const TextConfig *config, const char *str);
void MonitorText_Destroy(MonitorText **mt);

// Size, in screen pixels, that the text occupies at the given scale
void MonitorText_Measure(MonitorText *mt, double scale, int *w, int *h);

// (x, y) is the top-left corner, in screen pixels
void MonitorText_Draw(MonitorText *mt, int x, int y, double scale);
// cx is the horizontal center, y the top
void MonitorText_DrawCentered(MonitorText *mt, int cx, int y, double scale);

#endif
