#ifndef SWSB_TIMER_H
#define SWSB_TIMER_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_FPS              120
#define SCREEN_TICKS_PER_FRAME  (1000 / SCREEN_FPS)

typedef struct
{
    Uint32 start_ticks;
    Uint32 paused_ticks;
    Uint32 iteration_ticks;
    bool is_paused;
    bool is_started;
} Timer;

Timer InitTimer(void);
void SetIterationTicks(Timer *t);
void StartStopWatch(Timer *t);
void PauseStopWatch(Timer *t);
void ResumeStopWatch(Timer *t);
Uint32 GetStopWatchTicks(Timer *t);
void SetFrameRate(Timer *t);

#endif /* SWSB_TIMER_H */
