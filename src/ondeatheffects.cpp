#include "ondeatheffects.h"
#include "utility.h"
#include "settings.h"
#include "cache.h"
#include "stressapplicationhandler.h"

void DeathEffects::ResEffects::CapInnPrices()
{
    {
        if (Settings::Values::bEnableInnPrice.GetValue())
        {
            if (Settings::Forms::inn_price_single_night->value < Settings::Forms::inn_prices_map[Settings::Forms::inn_price_single_night])
            {
                Settings::Forms::inn_price_single_night->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_single_night];
            }

            if (!DoesExist(Settings::Forms::inn_price_week) || !DoesExist(Settings::Forms::inn_price_month) || !DoesExist(Settings::Forms::inn_price_night_capital) || !DoesExist(Settings::Forms::inn_price_week_capital) || !DoesExist(Settings::Forms::inn_price_month_capital))
            {
                logs::error("One or more inn price globals are null. Cannot cap to default values.");
                return;
            }
            else
            {

                if (Settings::Forms::inn_price_week->value < Settings::Forms::inn_prices_map[Settings::Forms::inn_price_week])
                {
                    Settings::Forms::inn_price_week->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_week];
                }
                if (Settings::Forms::inn_price_month->value < Settings::Forms::inn_prices_map[Settings::Forms::inn_price_month])
                {
                    Settings::Forms::inn_price_month->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_month];
                }
                if (Settings::Forms::inn_price_night_capital->value < Settings::Forms::inn_prices_map[Settings::Forms::inn_price_night_capital])
                {
                    Settings::Forms::inn_price_night_capital->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_night_capital];
                }
                if (Settings::Forms::inn_price_week_capital->value < Settings::Forms::inn_prices_map[Settings::Forms::inn_price_week_capital])
                {
                    Settings::Forms::inn_price_week_capital->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_week_capital];
                }
                if (Settings::Forms::inn_price_month_capital->value < Settings::Forms::inn_prices_map[Settings::Forms::inn_price_month_capital])
                {
                    Settings::Forms::inn_price_month_capital->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_month_capital];
                }
            }
        }
    }
}

bool DeathEffects::ResEffects::DoesExist(RE::TESGlobal *a_global)
{
    return a_global != nullptr && a_global->GetFormType() == RE::FormType::Global;
}

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
            IncreaseInnPrices(Settings::Values::fInnPriceMultiplier.GetValue());
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
            ResetInnPrices();
        }
    }
    // Decrease stress for curing all injuries
    auto stressManager = Stress::StressHandler::GetSingleton();
    stressManager->ReduceStress();
    Settings::Forms::active_injuries.clear();
    logs::debug("All injuries removed.");
}

void DeathEffects::ResEffects::IncreaseInnPrices(float multiplier)
{
    if (Settings::Values::bEnableInnPrice.GetValue())
    {
        if (DoesExist(Settings::Forms::inn_price_single_night))
        {
            Settings::Forms::inn_price_single_night->value *= 1 + (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_week))
        {
            Settings::Forms::inn_price_week->value *= 1 + (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_month))
        {
            Settings::Forms::inn_price_month->value *= 1 + (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_night_capital))
        {
            Settings::Forms::inn_price_night_capital->value *= 1 + (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_week_capital))
        {
            Settings::Forms::inn_price_week_capital->value *= 1 + (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_month_capital))
        {
            Settings::Forms::inn_price_month_capital->value *= 1 + (multiplier / 100.0f);
        }
    }
}

void DeathEffects::ResEffects::DecreaseInnPrices(float multiplier)
{
    if (Settings::Values::bEnableInnPrice.GetValue())
    {
        if (DoesExist(Settings::Forms::inn_price_single_night))
        {
            Settings::Forms::inn_price_single_night->value *= 1 - (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_week))
        {
            Settings::Forms::inn_price_week->value *= 1 - (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_month))
        {
            Settings::Forms::inn_price_month->value *= 1 - (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_night_capital))
        {
            Settings::Forms::inn_price_night_capital->value *= 1 - (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_week_capital))
        {
            Settings::Forms::inn_price_week_capital->value *= 1 - (multiplier / 100.0f);
        }
        if (DoesExist(Settings::Forms::inn_price_month_capital))
        {
            Settings::Forms::inn_price_month_capital->value *= 1 - (multiplier / 100.0f);
        }
        CapInnPrices();
    }
}

void DeathEffects::ResEffects::ResetInnPrices()
{
    if (Settings::Forms::inn_prices_map.empty())
    {
        logs::error("Inn prices map is empty. Cannot reset to default values.");
        return;
    }
    if (!Settings::Values::bEnableInnPrice.GetValue())
    {
        return;
    }
    Settings::Forms::inn_price_single_night->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_single_night];

    if (Settings::Forms::inn_price_week == nullptr || Settings::Forms::inn_price_month == nullptr || Settings::Forms::inn_price_night_capital == nullptr || Settings::Forms::inn_price_week_capital == nullptr || Settings::Forms::inn_price_month_capital == nullptr)
    {
        logs::error("One or more inn price globals are null. Cannot reset to default values.");
        return;
    }

    Settings::Forms::inn_price_week->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_week];
    Settings::Forms::inn_price_month->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_month];
    Settings::Forms::inn_price_night_capital->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_night_capital];
    Settings::Forms::inn_price_week_capital->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_week_capital];
    Settings::Forms::inn_price_month_capital->value = Settings::Forms::inn_prices_map[Settings::Forms::inn_price_month_capital];
    logs::debug("Inn prices reset to default values.");
}

void DeathEffects::ResEffects::ReapplyInnPriceMultiplier()
{
    if (!Settings::Values::bEnableInnPrice.GetValue())
        return;

    int injuryCount = static_cast<int>(Settings::Forms::active_injuries.size());
    float inj_modi = 1 + Settings::Values::fInnPriceMultiplier.GetValue() / 100.0f;
    float multiplier = std::pow(inj_modi, injuryCount);

    for (auto &[global, basePrice] : Settings::Forms::inn_prices_map)
    {
        if (global->As<RE::TESGlobal>() && DoesExist(global->As<RE::TESGlobal>()))
        {
            global->As<RE::TESGlobal>()->value = basePrice * multiplier;
        }
    }
}
