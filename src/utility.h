#pragma once

#include "settings.h"
#include "cache.h"
#include "ondeatheffects.h"
#include <format> // Include for std::format

namespace Utility
{
    struct Actors
    {
        // Credits: https://github.com/powerof3/PapyrusExtenderSSE/
        inline static std::vector<RE::Actor *> GetNearbyActors(RE::TESObjectREFR *a_ref, float a_radius, bool a_ignorePlayer)
        {
            {
                std::vector<RE::Actor *> result;
                if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists)
                {
                    if (a_ignorePlayer && processLists->numberHighActors == 0)
                    {
                        return result;
                    }

                    const auto squaredRadius = a_radius * a_radius;
                    const auto originPos = a_ref->GetPosition();

                    result.reserve(processLists->numberHighActors);

                    const auto get_actor_within_radius = [&](RE::Actor *a_actor)
                    {
                        if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius)
                        {
                            result.emplace_back(a_actor);
                        }
                    };
                    for (auto &actorHandle : processLists->highActorHandles)
                    {
                        const auto actor = actorHandle.get();
                        get_actor_within_radius(actor.get());
                    }

                    if (!a_ignorePlayer)
                    {
                        get_actor_within_radius(RE::PlayerCharacter::GetSingleton());
                    }

                    if (!result.empty())
                    {
                        return result;
                    }
                }
                return result;
            }
        }
    };

    // Credits: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Conditions.h
    struct Spells
    {

        // https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Conditions.h
        static bool HasSpell(RE::Actor *actor, RE::SpellItem *spell)
        {
            using func_t = decltype(&Utility::Spells::HasSpell);

            REL::Relocation<func_t> func{Cache::HasSpellAddress};

            return func(actor, spell);
        }
        inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

        inline static bool ActorHasActiveMagicEffect(RE::Actor *a_actor, RE::EffectSetting *a_effect)
        {

            auto activeEffects = a_actor->AsMagicTarget()->GetActiveEffectList();
            RE::EffectSetting *setting = nullptr;
            for (auto &effect : *activeEffects)
            {
                setting = effect ? effect->GetBaseObject() : nullptr;
                if (setting && !effect->flags.any(RE::ActiveEffect::Flag::kInactive))
                {
                    if (setting == a_effect)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        static bool IsPermanent(RE::MagicItem *item)
        {
            switch (item->GetSpellType())
            {
            case RE::MagicSystem::SpellType::kDisease:
            case RE::MagicSystem::SpellType::kAbility:
            case RE::MagicSystem::SpellType::kAddiction:
            {
                return true;
            }
            default:
            {
                return false;
            }
            }
        }

        inline static void ApplySpell(RE::Actor *caster, RE::Actor *target, RE::SpellItem *spell)
        {
            if (IsPermanent(spell))
            {
                target->AddSpell(spell);
            }
            else
            {
                caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
            }
        }
    };
    struct Locations
    {
        inline static const std::vector<std::string> safe_areas_diff2{
            "LocTypeGuild",
            "LocTypeInn",
            "LocTypePlayerHouse"};

        inline static const std::vector<std::string> unsafe_areas_diff1{
            "LocTypeAnimalDen",
            "LocTypeBanditCamp",
            "LocTypeDraugrCrypt",
            "LocTypeDungeon",
            "LocTypeDwarvenAutomatons",
            "LocTypeFalmerHive",
            "LocTypeForswornCamp",
            "LocTypeGiantCamp",
            "LocTypeShipwreck",
            "LocTypeShip",
            "LocTypeSprigganGrove",
            "LocTypeVampireLair",
            "LocTypeWarlockLair",
            "LocTypeWerewolfLair"};

        inline static bool IsSafePlace(RE::TESObjectCELL *cell)
        {
            if (cell)
            {
                auto location = cell->GetLocation();
                if (location)
                {
                    switch (Settings::Values::sleep_location_difficulty.GetValue())
                    {
                    case 0:
                        return true;
                    case 1:
                        return !location->HasAnyKeywordByEditorID(unsafe_areas_diff1);
                    case 2:
                        return location->HasAnyKeywordByEditorID(safe_areas_diff2);
                    default:
                        return false;
                    }
                }
            }
            return true;
        }
    };

    struct Injuries
    {
        // Credits: https://github.com/powerof3/PapyrusExtenderSSE/blob/9b14951520c9f57d29fc6ff578f5496dd668188a/include/Papyrus/Functions/Actor.h#L308
        inline static void PopulateInjuryVecAfterLoad(RE::Actor *a_actor)
        {
            class Visitor : public RE::Actor::ForEachSpellVisitor
            {
            public:
                RE::BSContainer::ForEachResult Visit(RE::SpellItem *a_spell) override
                {

                    for (auto &spell : Settings::Forms::minor_injuries)
                    {
                        if (a_spell == spell)
                        {
                            spells.push_back(a_spell);
                        }
                    }
                    for (auto &spell : Settings::Forms::medium_injuries)
                    {
                        if (a_spell == spell)
                        {
                            spells.push_back(a_spell);
                        }
                    }
                    for (auto &spell : Settings::Forms::major_injuries)
                    {
                        if (a_spell == spell)
                        {
                            spells.push_back(a_spell);
                        }
                    }
                    return RE::BSContainer::ForEachResult::kContinue;
                }
                std::vector<RE::SpellItem *> spells;
            } visitor;

            a_actor->VisitSpells(visitor);
            Settings::Forms::active_injuries = visitor.spells;
            DeathEffects::ResEffects::ReapplyInnPriceMultiplier();
            return;
        }

        // Function to initialize spell upgrades
        static void InitializeUpgrades()
        {
            for (size_t i = 0; i < Settings::Forms::minor_injuries.size(); ++i)
            {
                Settings::Forms::spell_upgrades[Settings::Forms::minor_injuries[i]] = Settings::Forms::medium_injuries[i];
                Settings::Forms::spell_upgrades[Settings::Forms::medium_injuries[i]] = Settings::Forms::major_injuries[i];
            }
        }

        static void InitializeDowngrades()
        {
            for (size_t i = 0; i < Settings::Forms::minor_injuries.size(); ++i)
            {
                Settings::Forms::spell_downgrades[Settings::Forms::medium_injuries[i]] = Settings::Forms::minor_injuries[i];
                Settings::Forms::spell_downgrades[Settings::Forms::major_injuries[i]] = Settings::Forms::medium_injuries[i];
            }
        }

        static void DowngradeInjuries(RE::Actor *a_actor)
        {
            auto &injuries = Settings::Forms::active_injuries;

            // We'll rebuild the list as we go
            std::vector<RE::SpellItem *> updatedInjuries;

            for (auto *spell : injuries)
            {
                if (Settings::Forms::spell_downgrades.contains(spell))
                {
                    // Downgrade
                    RE::SpellItem *downgraded = Settings::Forms::spell_downgrades[spell];
                    a_actor->RemoveSpell(spell);
                    Spells::ApplySpell(a_actor, a_actor, downgraded);
                    updatedInjuries.push_back(downgraded);
                }
                else if (!Settings::Forms::minor_injuries.empty() &&
                         std::count(Settings::Forms::minor_injuries.begin(), Settings::Forms::minor_injuries.end(), spell))
                {
                    // It's a minor injury: remove it from actor and don't re-add
                    a_actor->RemoveSpell(spell);
                }
                else
                {
                    // Keep the spell as is
                    updatedInjuries.push_back(spell);
                }
            }

            injuries = std::move(updatedInjuries);
            DeathEffects::ResEffects::ReapplyInnPriceMultiplier();
        }

        // Function to get a random spell from a given tier
        static RE::SpellItem *GetRandomSpell(const std::vector<RE::SpellItem *> &spells)
        {
            if (spells.empty())
            {
                logs::error("No spells available in the vector.");
                return nullptr;
            }

            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<size_t> dist(0, spells.size() - 1);
            RE::SpellItem *selected_spell = spells[dist(gen)];

            if (!selected_spell)
            {
                logs::error("Random spell selection failed.");
            }
            else
            {
                logs::debug("Selected random spell: {}", EDID::GetEditorID(selected_spell));
            }
            return selected_spell;
        }

        static void ApplyInjury(RE::Actor *a_actor)
        {
            // Upgrade existing injuries if possible
            for (auto &spell : Settings::Forms::active_injuries)
            {
                if (Settings::Forms::spell_upgrades.count(spell))
                {
                    RE::SpellItem *upgraded_spell = Settings::Forms::spell_upgrades[spell];
                    // Remove the old spell
                    a_actor->RemoveSpell(spell);
                    // Upgrade to the new spell
                    Spells::ApplySpell(a_actor, a_actor, upgraded_spell);
                    logs::info("Upgraded injury: {} to {}", EDID::GetEditorID(spell), EDID::GetEditorID(upgraded_spell));
                    spell = upgraded_spell; // Update injury list
                    
                }
            }
            // Apply a new minor injury
            RE::SpellItem *new_injury = GetRandomSpell(Settings::Forms::minor_injuries);
            if (new_injury)
            {
                Settings::Forms::active_injuries.push_back(new_injury);
                Spells::ApplySpell(a_actor, a_actor, new_injury);

                logs::info("Applied new minor injury: {}", EDID::GetEditorID(new_injury));
            }
        }
    };
}