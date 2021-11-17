#include "../include/SWSB_timer.h"

Timer InitTimer(void)
{
    Timer t = {0, 0, 0, false, false};
    return t;
}

void SetIterationTicks(Timer *t)
{
    t->iteration_ticks = SDL_GetTicks();
}

void StartStopWatch(Timer *t)
{
    t->is_started = true;
    t->start_ticks = SDL_GetTicks();
}

void PauseStopWatch(Timer *t)
{
    if (t->is_started && !t->is_paused)
    {
        t->is_paused = true;
        t->paused_ticks = SDL_GetTicks() - t->start_ticks;
    }
}

void ResumeStopWatch(Timer *t)
{
    if (t->is_started && t->is_paused)
    {
        t->is_paused = false;
        t->start_ticks = SDL_GetTicks() - t->paused_ticks;
    }
}

Uint32 GetStopWatchTicks(Timer *t)
{
    Uint32 time = 0;

    if (t->is_started)
    {
        if (t->is_paused)
        {
            time = t->paused_ticks;
        }
        else
        {
            time = SDL_GetTicks() - t->start_ticks;
        }
    }

    return time;
}

void SetFrameRate(Timer *t)
{
    // We don't want to use GetStopWatchTicks here. If we didn't have a "pause game" feature,
    // then we could, but let me explain why we can't. If you call PauseStopWatch
    // then later on call ResumeStopWatch and then get the value from GetStopWatchTicks
    // that value will return relativity the same amount of time before PauseStopWatch was called.
    // This is a problem. Even if we placed ResumeStopWatch at the last line of the while loop,
    // we are still going to be behind the actual time it took to complete the loop.
    // Even though your stopwatch was paused, time kept going. So using your stopwatch
    // to set the frame rate is bad idea, since it's not a good representaion of how much time
    // actually passed in the while loop.
    // For setting the frame rate, we want to know the amount of time
    // it took to complete one iteration in the while loop.
    if (SCREEN_TICKS_PER_FRAME > (SDL_GetTicks() - t->iteration_ticks))
    {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - (SDL_GetTicks() - t->iteration_ticks));
    }
}
