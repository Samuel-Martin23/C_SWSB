#include "../include/SWSB_timer.h"

Timer timer = {0, 0, 0, false, false};

void SetIterationTicks(void)
{
    timer.iteration_ticks = SDL_GetTicks();
}

void StartStopWatch(void)
{
    timer.is_started = true;
    timer.start_ticks = SDL_GetTicks();
}

void PauseStopWatch(void)
{
    if (timer.is_started && !(timer.is_paused))
    {
        timer.is_paused = true;
        timer.paused_ticks = SDL_GetTicks() - timer.start_ticks;
    }
}

void ResumeStopWatch(void)
{
    if (timer.is_started && timer.is_paused)
    {
        timer.is_paused = false;
        timer.start_ticks = SDL_GetTicks() - timer.paused_ticks;
    }
}

Uint32 GetStopWatchTicks(void)
{
    Uint32 time = 0;

    if (timer.is_started)
    {
        if (timer.is_paused)
        {
            time = timer.paused_ticks;
        }
        else
        {
            time = SDL_GetTicks() - timer.start_ticks;
        }
    }

    return time;
}

void SetFrameRate(void)
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
    if (SCREEN_TICKS_PER_FRAME > (SDL_GetTicks() - timer.iteration_ticks))
    {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - (SDL_GetTicks() - timer.iteration_ticks));
    }
}
