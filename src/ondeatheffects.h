#pragma once

#include "settings.h"

namespace DeathEffects
{
    class ResEffects
    {
        static bool DoesExist(RE::TESGlobal *a_global);

    public:
        static void SetEthereal(RE::Actor *a_actor);
        static bool CheckLadyStone(RE::PlayerCharacter *player);
        static void RemoveAllInjuries(RE::Actor *a_actor);
        static void ReapplyInnPriceMultiplier();
    };
}