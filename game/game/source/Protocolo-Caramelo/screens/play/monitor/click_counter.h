#ifndef CLICK_COUNTER_H
#define CLICK_COUNTER_H

#include<graphical.h>

/*
 * ClickCounter: tells how many consecutive clicks the same target received
 * (2 = double click). Shared by everything that reacts to double clicks.
 */

// Longest time between two clicks for them to count as consecutive
#define CLICK_MULTI_MS 400

typedef struct ClickCounter ClickCounter;
struct ClickCounter {
    const void *target;
    Uint32 time;
    int count;
};

// Registers a click on target and returns its number of consecutive clicks
int ClickCounter_Register(ClickCounter *counter, const void *target);
void ClickCounter_Reset(ClickCounter *counter);

#endif
