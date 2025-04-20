#include "hooks.h"

void Hooks::Character__invalidate_cached(RE::Actor *a, RE::ActorValue av)
{
    using func_t = decltype(Character__invalidate_cached);
    REL::Relocation<func_t> func{REL::RelocationID(37534, 38483)};
    return func(a, av);
}

void Hooks::InstallHooks()
{
    logs::info("*****************HOOKS*****************");
    PlayerPotionUsed::Install();
}

void Hooks::PlayerPotionUsed::Install()
{
    REL::Relocation<uintptr_t> target{REL::VariantID(39604, 40690, 0x6d7b00), REL::VariantOffset(0x15, 0x15, 0x15)};
    auto &trampoline = SKSE::GetTrampoline();
    _PlayerUsePotion = trampoline.write_call<5>(target.address(), PlayerUsePotion);
    logs::info("Installed <{}>", typeid(PlayerPotionUsed).name());
}

void Hooks::PlayerPotionUsed::PlayerUsePotion(uint64_t self, RE::AlchemyItem *alch, uint64_t extralist)
{
    if (alch->HasKeywordString("CureInjury"))
    {
        RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
        Utility::Injuries::DowngradeInjuries(player);
    }
    return _PlayerUsePotion(self, alch, extralist);
}
