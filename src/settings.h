#pragma once
#include "pch.h"
#include "helper.h"

namespace Settings
{
    namespace Values
    {
        // bool
        static REX::INI::Bool bEnableDebugLog{"Logging", "bEnableDebugLog", true};
        static REX::INI::Bool show_stress_message{"Stress", "bShowStressMessage", true};
        // int
        static REX::INI::I32 sleep_location_difficulty{"Settings", "iSleepLocationDifficulty", 1};
        // float
        static REX::INI::F32 stress_increase_value{"Stress", "fStressIncreaseValue", 0.5f};
        static REX::INI::F32 min_sleep_duration{"Settings", "fMinSleepDuration", 7.9f};

        // string
        static REX::INI::Str stress_message{"Stress", "sStressMessage", (std::string) "You are stressed!"};

        static void Update()
        {
            logs::info("Loading settings...");
            const auto ini = REX::INI::SettingStore::GetSingleton();
            ini->Init(R"(.\Data\SKSE\Plugins\death-injuries.ini)", R"(.\Data\SKSE\Plugins\death-injuries.ini)");
            ini->Load();

            if (bEnableDebugLog.GetValue())
            {
                spdlog::set_level(spdlog::level::debug);
                logs::info("Debug logging enabled");
            }

            logs::info("...Settings loaded");
        }
    }

    namespace Constants
    {
        constexpr const char *esp_name = "death-injuries.esp";
        const int injury_chest_tier0 = 0x817;
        const int injury_head_tier0 = 0x81c;
        const int injury_organs_tier0 = 0x820;
        const int injury_legs_tier0 = 0x81E;
        const int injury_chest_tier1 = 0x822;
        const int injury_head_tier1 = 0x823;
        const int injury_organs_tier1 = 0x825;
        const int injury_legs_tier1 = 0x824;
        const int injury_chest_tier2 = 0x82a;
        const int injury_head_tier2 = 0x82b;
        const int injury_organs_tier2 = 0x82d;
        const int injury_legs_tier2 = 0x82c;
        // resurrection spells
        const int ethereal_spell_formid = 0x801;
        const int death_heal_formid = 0x803;
        const int calm_spell_npcs_formid = 0x810;
        const int ethereal_effect_formid = 0x800;
        const int gold_tax_global_formid = 0x815;
        const int lady_stone_perk_formid = 0x812;
        // stress and fear integration
        constexpr const char *stress_mod_name = "Stress and Fear.esp";
        const int stress_enabled_formid = 0x8a5;
        const int stress_total_value_formid = 0x801;
    }

    struct Forms
    {
        static inline std::vector<RE::SpellItem *> minor_injuries;
        static inline std::vector<RE::SpellItem *> medium_injuries;
        static inline std::vector<RE::SpellItem *> major_injuries;

        // Mapping from lower-tier to higher-tier spells
        static inline std::unordered_map<RE::SpellItem *, RE::SpellItem *> spell_upgrades;

         // Mapping from higher-tier to lower-tier spells
        static inline std::unordered_map<RE::SpellItem *, RE::SpellItem *> spell_downgrades;
       

        // Active injuries (tracks applied injuries)
        static inline std::vector<RE::SpellItem *> active_injuries;

        // injury spells
        static inline RE::SpellItem *injury_chest_tier0{nullptr};
        static inline RE::SpellItem *injury_head_tier0{nullptr};
        static inline RE::SpellItem *injury_organs_tier0{nullptr};
        static inline RE::SpellItem *injury_legs_tier0{nullptr};
        static inline RE::SpellItem *injury_chest_tier1{nullptr};
        static inline RE::SpellItem *injury_head_tier1{nullptr};
        static inline RE::SpellItem *injury_organs_tier1{nullptr};
        static inline RE::SpellItem *injury_legs_tier1{nullptr};
        static inline RE::SpellItem *injury_chest_tier2{nullptr};
        static inline RE::SpellItem *injury_head_tier2{nullptr};
        static inline RE::SpellItem *injury_organs_tier2{nullptr};
        static inline RE::SpellItem *injury_legs_tier2{nullptr};
        // resurrection spells
        static inline RE::SpellItem *ethereal_spell{nullptr}; // ethereal spell for resurrection
        static inline RE::SpellItem *death_heal{nullptr};     // heal for resurrection and surrounding npcs
        static inline RE::SpellItem *calm_spell_npcs{nullptr};
        static inline RE::EffectSetting *ethereal_effect{nullptr};
        // lady stone integration
        static inline RE::TESGlobal *gold_tax_global{nullptr};
        static inline RE::BGSPerk *lady_stone_perk{nullptr};
        // stress and fear integration
        static inline RE::TESGlobal *stress_enabled{nullptr};
        static inline RE::TESGlobal *stress_total_value{nullptr};

        static void LogSpells()
        {
            if (Settings::Values::bEnableDebugLog.GetValue())
            {
                logs::info("injury_chest_tier0: {}", injury_chest_tier0->GetName());
                logs::info("injury_head_tier0: {}", injury_head_tier0->GetName());
                logs::info("injury_organs_tier0: {}", injury_organs_tier0->GetName());
                logs::info("injury_legs_tier0: {}", injury_legs_tier0->GetName());
                logs::info("injury_chest_tier1: {}", injury_chest_tier1->GetName());
                logs::info("injury_head_tier1: {}", injury_head_tier1->GetName());
                logs::info("injury_organs_tier1: {}", injury_organs_tier1->GetName());
                logs::info("injury_legs_tier1: {}", injury_legs_tier1->GetName());
                logs::info("injury_chest_tier2: {}", injury_chest_tier2->GetName());
                logs::info("injury_head_tier2: {}", injury_head_tier2->GetName());
                logs::info("injury_organs_tier2: {}", injury_organs_tier2->GetName());
                logs::info("injury_legs_tier2: {}", injury_legs_tier2->GetName());
            }
        }

        static void AddSpellsToInjuryList(RE::SpellItem *spell, std::vector<RE::SpellItem *> &injury_list)
        {
            if (spell)
            {
                injury_list.push_back(spell);
            }
        }

        static void PopulateInjuryLists()
        {
            AddSpellsToInjuryList(injury_chest_tier0, minor_injuries);
            AddSpellsToInjuryList(injury_head_tier0, minor_injuries);
            AddSpellsToInjuryList(injury_organs_tier0, minor_injuries);
            AddSpellsToInjuryList(injury_legs_tier0, minor_injuries);

            AddSpellsToInjuryList(injury_chest_tier1, medium_injuries);
            AddSpellsToInjuryList(injury_head_tier1, medium_injuries);
            AddSpellsToInjuryList(injury_organs_tier1, medium_injuries);
            AddSpellsToInjuryList(injury_legs_tier1, medium_injuries);

            AddSpellsToInjuryList(injury_chest_tier2, major_injuries);
            AddSpellsToInjuryList(injury_head_tier2, major_injuries);
            AddSpellsToInjuryList(injury_organs_tier2, major_injuries);
            AddSpellsToInjuryList(injury_legs_tier2, major_injuries);
        }

        static void LoadForms()
        {
            auto dh = RE::TESDataHandler::GetSingleton();

            injury_chest_tier0 = dh->LookupForm<RE::SpellItem>(Constants::injury_chest_tier0, Constants::esp_name);
            injury_head_tier0 = dh->LookupForm<RE::SpellItem>(Constants::injury_head_tier0, Constants::esp_name);
            injury_organs_tier0 = dh->LookupForm<RE::SpellItem>(Constants::injury_organs_tier0, Constants::esp_name);
            injury_legs_tier0 = dh->LookupForm<RE::SpellItem>(Constants::injury_legs_tier0, Constants::esp_name);
            injury_chest_tier1 = dh->LookupForm<RE::SpellItem>(Constants::injury_chest_tier1, Constants::esp_name);
            injury_head_tier1 = dh->LookupForm<RE::SpellItem>(Constants::injury_head_tier1, Constants::esp_name);
            injury_organs_tier1 = dh->LookupForm<RE::SpellItem>(Constants::injury_organs_tier1, Constants::esp_name);
            injury_legs_tier1 = dh->LookupForm<RE::SpellItem>(Constants::injury_legs_tier1, Constants::esp_name);
            injury_chest_tier2 = dh->LookupForm<RE::SpellItem>(Constants::injury_chest_tier2, Constants::esp_name);
            injury_head_tier2 = dh->LookupForm<RE::SpellItem>(Constants::injury_head_tier2, Constants::esp_name);
            injury_organs_tier2 = dh->LookupForm<RE::SpellItem>(Constants::injury_organs_tier2, Constants::esp_name);
            injury_legs_tier2 = dh->LookupForm<RE::SpellItem>(Constants::injury_legs_tier2, Constants::esp_name);

            // resurrection spells
            death_heal = dh->LookupForm<RE::SpellItem>(Constants::death_heal_formid, Constants::esp_name);
            ethereal_spell = dh->LookupForm<RE::SpellItem>(Constants::ethereal_spell_formid, Constants::esp_name);
            calm_spell_npcs = dh->LookupForm<RE::SpellItem>(Constants::calm_spell_npcs_formid, Constants::esp_name);
            ethereal_effect = dh->LookupForm<RE::EffectSetting>(Constants::ethereal_effect_formid, Constants::esp_name);

            // lady stone integration
            gold_tax_global = dh->LookupForm<RE::TESGlobal>(Constants::gold_tax_global_formid, Constants::esp_name);
            lady_stone_perk = dh->LookupForm<RE::BGSPerk>(Constants::lady_stone_perk_formid, Constants::esp_name);

            // stress and fear integration
            if (auto file = dh->LookupModByName(Constants::stress_mod_name); file && file->compileIndex != 0xFF)
            {
                logs::info("Stress and Fear is active");
                stress_enabled = dh->LookupForm<RE::TESGlobal>(Constants::stress_enabled_formid, Constants::stress_mod_name);
                stress_total_value = dh->LookupForm<RE::TESGlobal>(Constants::stress_total_value_formid, Constants::stress_mod_name);
                if (Settings::Values::bEnableDebugLog.GetValue())
                {
                    logs::info("stress_enabled: {}", stress_enabled->GetFormEditorID());
                    logs::info("stress_total_value: {}", stress_total_value->GetFormEditorID());
                }
            }

            PopulateInjuryLists();
            LogSpells();
            
            if (Settings::Values::bEnableDebugLog.GetValue())
            {
                for (auto &spell : minor_injuries)
                {
                    if (spell)
                        logs::info("Minor Injury Spell: {}", EDID::GetEditorID(spell));
                }
                for (auto &spell : medium_injuries)
                {
                    if (spell)
                        logs::info("Medium Injury Spell: {}", EDID::GetEditorID(spell));
                }
                for (auto &spell : major_injuries)
                {
                    if (spell)
                        logs::info("Major Injury Spell: {}", EDID::GetEditorID(spell));
                }
            }
        }
    };
}