#pragma once

#include "utility.h"
#include "cache.h"
#include "stressapplicationhandler.h"

namespace Hooks
{
    void InstallHooks();
    // Credits: https://github.com/muenchk/NPCsUsePotions/blob/3256f95831a84acccccf53802364e1edb9953e80/include/Hooks.h#L7
    struct PlayerPotionUsed
    {

        static void Install();

    private:
        static void PlayerUsePotion(uint64_t self, RE::AlchemyItem *alch, uint64_t extralist);
        static inline REL::Relocation<decltype(&PlayerUsePotion)> _PlayerUsePotion;
    };
}