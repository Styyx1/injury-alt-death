#include "ondeatheffects.h"
#include "utility.h"
#include "settings.h"
#include "cache.h"
#include "stressapplicationhandler.h"

void DeathEffects::ResEffects::SetEthereal(RE::Actor *a_actor)
{
    if (!Utility::Spells::ActorHasActiveMagicEffect(a_actor, Settings::Forms::ethereal_effect))
    {
        Utility::Spells::ApplySpell(a_actor, a_actor, Settings::Forms::death_heal);
        Utility::Spells::ApplySpell(a_actor, a_actor, Settings::Forms::ethereal_spell);

        for (auto enemy : Utility::Actors::GetNearbyActors(a_actor, 500.0f, false))
        {
            if (enemy && enemy->IsHostileToActor(a_actor))
            {
                Utility::Spells::ApplySpell(enemy, enemy, Settings::Forms::death_heal);
                if (enemy != a_actor)
                {
                    Utility::Spells::ApplySpell(enemy, enemy, Settings::Forms::calm_spell_npcs);
                }
            }
        }
        auto player = Cache::GetPlayerSingleton();

        if (CheckLadyStone(player))
        {
            Utility::Injuries::ApplyInjury(player);
            auto stressManager = Stress::StressHandler::GetSingleton();
            stressManager->ApplyStress();
        }
    }
}

void DeathEffects::ResEffects::ApplyRandomInjury(RE::Actor *a_actor, RE::SpellItem *a_spell)
{
}
#undef GetObject
bool DeathEffects::ResEffects::CheckLadyStone(RE::PlayerCharacter *player)
{
    if (player->HasPerk(Settings::Forms::lady_stone_perk))
    {
        RE::TESForm *const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
        if (player->GetItemCount(gold->As<RE::TESBoundObject>()) >= Settings::Forms::gold_tax_global->value)
        {
            player->RemoveItem(gold->As<RE::TESBoundObject>(), Settings::Forms::gold_tax_global->value, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr);
            if (Settings::Values::show_lady_stone_message.GetValue())
            {
                RE::DebugNotification(std::format("You offered {} {} to the lady stone", Settings::Forms::gold_tax_global->value, gold->GetName()).c_str(), nullptr, true);
            }
            return false;
        }
    }
    return true;
}

void DeathEffects::ResEffects::RemoveAllInjuries(RE::Actor *a_actor)
{
    for (auto &spell : Settings::Forms::active_injuries)
    {
        if (Utility::Spells::HasSpell(a_actor, spell))
        {
            a_actor->RemoveSpell(spell);
            logs::debug("Removed injury: {}", EDID::GetEditorID(spell));
        }
    }
    // Decrease stress for curing all injuries
    auto stressManager = Stress::StressHandler::GetSingleton();
    stressManager->ReduceStress();
    Settings::Forms::active_injuries.clear();
    logs::debug("All injuries removed.");
}
