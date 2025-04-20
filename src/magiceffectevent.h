#pragma once

#include "utility.h"
#include "settings.h"
#include "ondeatheffects.h"


namespace Effect
{
    void InstallEvents();

    class ApplyEffectEvent final : public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
    {
        ApplyEffectEvent() = default;
        ApplyEffectEvent(const ApplyEffectEvent&) = delete;
        ApplyEffectEvent(ApplyEffectEvent&&) = delete;
        ApplyEffectEvent& operator=(const ApplyEffectEvent&) = delete;
        ApplyEffectEvent& operator=(ApplyEffectEvent&&) = delete;

    public:

        static ApplyEffectEvent* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) noexcept override;
        static void RegisterApplyEffect();
    };
}
