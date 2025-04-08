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
    SetAv::Install();
    // ModAVHook::Install();
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

void Hooks::SetAv::Install()
{
    REL::Relocation<uintptr_t> target{RE::PlayerCharacter::VTABLE[5]};
    _SetAV = target.write_vfunc(0x4, SetAV);
    logs::info("Installed <{}>", typeid(SetAv).name());
}

void Hooks::SetAv::SetAV(RE::ActorValueOwner *a_this, RE::ActorValue av, float value)
{

    if (av == RE::ActorValue::kHealth || av == RE::ActorValue::kMagicka || av == RE::ActorValue::kStamina)
    {
        float currentValue = a_this->GetActorValue(av);
        float currentBase = a_this->GetBaseActorValue(av);

        logs::info("value is {}", value);

        auto actor = skyrim_cast<RE::Actor *>(a_this);
        if (actor)
        {
            auto dmg_modi = actor->GetActorRuntimeData().healthModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage];
            logs::info("damage modifier is {}", dmg_modi);
            if (dmg_modi < 0.0f)
            {
                Character__invalidate_cached(actor, av);
                actor->GetActorRuntimeData().healthModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage] = 0;
                Character__invalidate_cached(actor, av);
            }
        }

        logs::info("SetAV for {}: current = {}, base = {}, new base = {}", av, currentValue, currentBase, value);
    }
    _SetAV(a_this, av, value);
}

void Hooks::ModAVHook::Install()
{
    REL::Relocation<uintptr_t> target{RE::PlayerCharacter::VTABLE[5]};
    _ModAV = target.write_vfunc(0x5, ModAV);
    logs::info("Installed <{}>", typeid(ModAVHook).name());
}

void Hooks::ModAVHook::ModAV(RE::ActorValueOwner *a_this, RE::ActorValue actorValue, float value)
{

    logs::info("ModAV for {}: value = {}", actorValue, value);
    auto actor = skyrim_cast<RE::Actor *>(a_this);
    if (actor)
    {
        auto dmg_modi = actor->GetActorRuntimeData().healthModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage];
        logs::info("damage modifier is {}", dmg_modi);
        if (dmg_modi < 0.0f)
        {
            Character__invalidate_cached(actor, actorValue);
            actor->GetActorRuntimeData().healthModifiers.modifiers[RE::ACTOR_VALUE_MODIFIER::kDamage] = 0;
            Character__invalidate_cached(actor, actorValue);
        }
    }
    _ModAV(a_this, actorValue, value);
}
