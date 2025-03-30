#pragma once

#include "settings.h"
#include "cache.h"
#include "utility.h"
#include "ondeatheffects.h"

namespace SleepEvents
{

    static inline float hours;
    void InstallEvents();
    void ProcessSleepStart();
    void ProcessSleepStop();

    class SleepStartHandler : public REX::Singleton<SleepStartHandler>,
                              public RE::BSTEventSink<RE::TESSleepStartEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStartEvent *a_event, RE::BSTEventSource<RE::TESSleepStartEvent> *a_eventSource) noexcept override;
        static void RegisterSleepStart();
    };

    class SleepStopHandler : public REX::Singleton<SleepStopHandler>,
                             public RE::BSTEventSink<RE::TESSleepStopEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESSleepStopEvent *a_event, RE::BSTEventSource<RE::TESSleepStopEvent> *a_eventSource) noexcept override;
        static void RegisterSleepStop();
        inline static bool is_sleeping;
    };
}