#pragma once
#ifndef MOD_FRAMELIMITER_H
#define MOD_FRAMELIMITER_H

#include <chrono>
#include <thread>

class FrameLimiter
{
private:
    static inline double TIME_Frequency = 0.0;
    static inline double TIME_Ticks = 0.0;
    static inline double TIME_Frametime = 0.0;

public:
    enum FPSLimitMode { FPS_NONE, FPS_REALTIME, FPS_ACCURATE };
    static void Init(FPSLimitMode mode, double fFPSLimit)
    {
        auto TICKS_PER_FRAME = 1;
        auto TICKS_PER_SECOND = (TICKS_PER_FRAME * fFPSLimit);

        if (mode == FPS_ACCURATE)
        {
            TIME_Frametime = 1000.0 / fFPSLimit;
            TIME_Frequency = 1000.0; // ticks are milliseconds
        }
        else // FPS_REALTIME
        {
            TIME_Frequency = 1.0 / fFPSLimit; // ticks are 1/n frames (n = fFPSLimit)
        }

        Ticks();
    }

    static unsigned long long Sync_RT()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - Ticks()).count();

        return elapsedMilliseconds;
    }

    static unsigned long long Sync_SLP()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto millis_delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - Ticks()).count();

        if (TIME_Frametime <= millis_delta)
        {
            Ticks() = currentTime;
            return 1;
        }
        else if (TIME_Frametime - millis_delta > 2) // > 2ms
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Sleep for ~1ms
        }
        else
        {
            std::this_thread::yield(); // Yield thread's time-slice (does not actually sleep)
        }

        return 0;
    }

private:
    static std::chrono::high_resolution_clock::time_point& Ticks()
    {
        static std::chrono::high_resolution_clock::time_point timePoint;
        return timePoint;
    }
};
#endif