#pragma once

#include "settings.h"

namespace DeathEffects
{
    class ResEffects
    {
    public:
        static void SetEthereal(RE::Actor *a_actor);
        static void ApplyRandomInjury(RE::Actor *a_actor, RE::SpellItem *a_spell);
        static bool CheckLadyStone(RE::PlayerCharacter *player);
        static void RemoveAllInjuries(RE::Actor *a_actor);

        // Credits: https://github.com/fenix31415/UselessFenixUtils
        static uint32_t get_count_(RE::InventoryChanges *changes, RE::TESBoundObject *item)
        {
            using func_t = decltype(&get_count_);
            REL::Relocation<func_t> func{REL::RelocationID(15868, 16108)};
            return func(changes, item);
        }

        static uint32_t get_count(RE::TESObjectREFR *refr, RE::TESBoundObject *item)
        {
            auto changes = refr->GetInventoryChanges();
            return get_count_(changes, item);
        }
    };
}