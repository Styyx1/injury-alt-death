#include "sleephandler.h"

namespace SleepEvents
{
    using Result = RE::BSEventNotifyControl;

    void InstallEvents()

    {
        SleepStopHandler::RegisterSleepStop();
        SleepStartHandler::RegisterSleepStart();
    }

    void ProcessSleepStart()

    {
        hours = RE::Calendar::GetSingleton()->GetHoursPassed();
        SleepStopHandler::is_sleeping = true;
    }
    void ProcessSleepStop()
    {
        hours = RE::Calendar::GetSingleton()->GetHoursPassed() - hours;
        RE::PlayerCharacter *player = Cache::GetPlayerSingleton();

        if (hours >= Settings::Values::min_sleep_duration.GetValue() / 2 && hours < Settings::Values::min_sleep_duration.GetValue())
        {
            if (Utility::Locations::IsSafePlace(player->GetParentCell())){
                Utility::Injuries::DowngradeInjuries(player);
            }
        }

        if (hours >= Settings::Values::min_sleep_duration.GetValue())
        {
            
            if (Utility::Locations::IsSafePlace(player->GetParentCell()))
            {
                DeathEffects::ResEffects::RemoveAllInjuries(player);
#ifdef DEBUGGING
                logs::info("player slept for more than {} hours", Settings::Values::min_sleep_duration.GetValue());
#endif
            }
            else
            {
#ifdef DEBUGGING
                logs::info("not a safe area, can't heal injury here");
#endif
            }
        }
    }
    RE::BSEventNotifyControl SleepStartHandler::ProcessEvent(const RE::TESSleepStartEvent *a_event, RE::BSTEventSource<RE::TESSleepStartEvent> *a_eventSource) noexcept
    {
        if (!a_event)
        {
            return Result::kContinue;
        }
        ProcessSleepStart();

        return Result::kContinue;
    }
    void SleepStartHandler::RegisterSleepStart()
    {
        auto *eventSink = SleepStartHandler::GetSingleton();

        auto *eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESSleepStartEvent>(eventSink);
        logs::info("Registered <{}>", typeid(SleepStartHandler).name());
    }
    RE::BSEventNotifyControl SleepStopHandler::ProcessEvent(const RE::TESSleepStopEvent *a_event, RE::BSTEventSource<RE::TESSleepStopEvent> *a_eventSource) noexcept
    {
        if (!a_event)
        {
            return Result::kContinue;
        }
        if (a_event->interrupted)
        {
            return Result::kContinue;
        }
        ProcessSleepStop();
        return Result::kContinue;
    }
    void SleepStopHandler::RegisterSleepStop()
    {
        auto *eventSink = SleepStopHandler::GetSingleton();

        auto *eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESSleepStopEvent>(eventSink);

        logs::info("Registered <{}>", typeid(SleepStopHandler).name());
    }
}