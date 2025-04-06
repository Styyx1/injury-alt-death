#pragma once
#include "pch.h"
#include "helper.h"

namespace Settings
{
    namespace Values
    {
        // bool
        static REX::INI::Bool bEnableDebugLog{"Logging", "bEnableDebugLog", false};
        static REX::INI::Bool show_stress_message{"Stress", "bShowStressMessage", true};
        static REX::INI::Bool show_lady_stone_message{"Settings", "bShowLadyStoneMessage", true};
        // int
        static REX::INI::I32 sleep_location_difficulty{"Settings", "iSleepLocationDifficulty", 1};
        // float
        static REX::INI::F32 stress_increase_value{"Stress", "fStressIncreaseValue", 10.0f};
        static REX::INI::F32 min_sleep_duration{"Settings", "fMinSleepDuration", 7.9f};

        // string
        static REX::INI::Str stress_increase_message{"Stress", "sStressMessage", (std::string) "You are getting stressed!"};
        static REX::INI::Str stress_decrease_message{"Stress", "sStressDecreaseMessage", (std::string) "You are less stressed!"};

        // hidden
        static REX::INI::Bool bEnableInnPrice{"Settings", "bEnableInnPrice", false};
        static REX::INI::F32 fInnPriceMultiplier{"Settings", "fInnPriceMultiplier", 20.0f}; // percentage increase for all inn prices

        static void LogSettings()
        {
            logs::info("----------Settings Logging----------");
            logs::info("bEnableDebugLog: {}", bEnableDebugLog.GetValue() ? "true" : "false");
            logs::info("iSleepLocationDifficulty: {}", sleep_location_difficulty.GetValue());
            logs::info("fStressIncreaseValue: {}", stress_increase_value.GetValue());
            logs::info("fMinSleepDuration: {}", min_sleep_duration.GetValue());
            logs::info("sStressMessage: {}", stress_increase_message.GetValue().c_str());
            logs::info("sStressDecreaseMessage: {}", stress_decrease_message.GetValue().c_str());
            logs::info("----------Settings Logging----------");
        }

        static void Update()
        {
            logs::info("*****************SETTINGS*****************");
            logs::info("Loading settings...");
            const auto ini = REX::INI::SettingStore::GetSingleton();
            ini->Init(R"(.\Data\SKSE\Plugins\stages-of-suffering.ini)", R"(.\Data\SKSE\Plugins\stages-of-suffering.ini)");
            ini->Load();

            LogSettings();

            if (bEnableDebugLog.GetValue())
            {
                spdlog::set_level(spdlog::level::debug);
                logs::debug("Debug logging enabled");
            }
            logs::info("...Settings loaded");
        }
    }

    namespace Constants
    {
        constexpr const char *esp_name = "StagesOfSuffering.esp";
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
        // inn prices
        const int inn_price_single_night_formid = 0x9CC98;
        const int inn_price_week_formid = 0x80a;
        const int inn_price_month_formid = 0x80b;
        const int inn_price_night_capital_formid = 0x82F;
        const int inn_price_week_capital_formid = 0x82e;
        const int inn_price_month_capital_formid = 0x82d;
        constexpr const char *inn_price_mod_name = "Candlehearth.esp";
        constexpr const char *inn_price_vanilla_master = "Skyrim.esm";
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
        // inn prices
        static inline RE::TESGlobal *inn_price_single_night{nullptr};
        static inline RE::TESGlobal *inn_price_week{nullptr};
        static inline RE::TESGlobal *inn_price_month{nullptr};
        static inline RE::TESGlobal *inn_price_night_capital{nullptr};
        static inline RE::TESGlobal *inn_price_week_capital{nullptr};
        static inline RE::TESGlobal *inn_price_month_capital{nullptr};

        static inline std::unordered_map<RE::TESForm *, float> inn_prices_map;

        static void StoreInnPrices()
        {
            if (Settings::Values::bEnableInnPrice.GetValue())
            {
                inn_prices_map[inn_price_single_night] = inn_price_single_night->value;
                if (inn_price_week && inn_price_month && inn_price_night_capital && inn_price_week_capital && inn_price_month_capital)
                {
                    inn_prices_map[inn_price_week] = inn_price_week->value;
                    inn_prices_map[inn_price_month] = inn_price_month->value;
                    inn_prices_map[inn_price_night_capital] = inn_price_night_capital->value;
                    inn_prices_map[inn_price_week_capital] = inn_price_week_capital->value;
                    inn_prices_map[inn_price_month_capital] = inn_price_month_capital->value;
                }
            }
        }

        static void LogSpells()
        {
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
            logs::info("*****************FORMS*****************");
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

            if (auto mod_file = dh->LookupModByName(Constants::esp_name); !mod_file || mod_file->compileIndex == 0xFF)
            {
                SKSE::stl::report_and_fail(std::format("Failed to load {}. Please enable it, the mod can not work without it", Constants::esp_name));
            }

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
            // inn prices
            inn_price_single_night = dh->LookupForm<RE::TESGlobal>(Constants::inn_price_single_night_formid, Constants::inn_price_vanilla_master);
            if (auto candleh = dh->LookupModByName(Constants::inn_price_mod_name); candleh && candleh->compileIndex != 0xFF)
            {
                logs::info("Candlehearth is active");
                inn_price_week = dh->LookupForm<RE::TESGlobal>(Constants::inn_price_week_formid, Constants::inn_price_mod_name);
                inn_price_month = dh->LookupForm<RE::TESGlobal>(Constants::inn_price_month_formid, Constants::inn_price_mod_name);
                inn_price_night_capital = dh->LookupForm<RE::TESGlobal>(Constants::inn_price_night_capital_formid, Constants::inn_price_mod_name);
                inn_price_week_capital = dh->LookupForm<RE::TESGlobal>(Constants::inn_price_week_capital_formid, Constants::inn_price_mod_name);
                inn_price_month_capital = dh->LookupForm<RE::TESGlobal>(Constants::inn_price_month_capital_formid, Constants::inn_price_mod_name);
                if (Settings::Values::bEnableDebugLog.GetValue())
                {
                    logs::info("inn_price_week: {}", inn_price_week->GetFormEditorID());
                    logs::info("inn_price_month: {}", inn_price_month->GetFormEditorID());
                    logs::info("inn_price_night_capital: {}", inn_price_night_capital->GetFormEditorID());
                    logs::info("inn_price_week_capital: {}", inn_price_week_capital->GetFormEditorID());
                    logs::info("inn_price_month_capital: {}", inn_price_month_capital->GetFormEditorID());
                }
            }

            PopulateInjuryLists();
            LogSpells();
            StoreInnPrices();
            logs::info("inn_prices_single_night: {}", inn_prices_map[inn_price_single_night]);
        }
    };

    namespace JSONSettings
    {
        namespace JSONValues
        {

            static inline RE::TESForm *GetFormFromString(const std::string &spellName)
            {
                std::istringstream ss{spellName};
                std::string plugin, id;
                std::getline(ss, id, '|');
                std::getline(ss, plugin);

                // std::getline(ss, plugin, '|');
                // std::getline(ss, id);

                RE::FormID rawFormID;
                std::istringstream(id) >> std::hex >> rawFormID;

                auto dataHandler = RE::TESDataHandler::GetSingleton();
                return dataHandler->LookupForm(rawFormID, plugin);
            }

            static inline void LoadInjuryConfig(const std::string &configFilePath)
            {
                std::ifstream file(configFilePath);
                if (!file.is_open())
                {
                    logs::error("Failed to open the file: {}", configFilePath);
                    return;
                }

                nlohmann::json j;
                file >> j;

                // Minor injuries
                for (const auto &str : j["minor_injuries"])
                {
                    logs::info("Loading minor injury spell: {}", str.get<std::string>());
                    RE::TESForm *form = GetFormFromString(str.get<std::string>());
                    if (form && form->GetFormType() == RE::FormType::Spell)
                    {
                        Forms::minor_injuries.push_back(form->As<RE::SpellItem>());
                        logs::info("Loaded minor injury spell: {}", EDID::GetEditorID(form));
                    }
                }

                // Medium injuries
                for (const auto &str : j["medium_injuries"])
                {
                    logs::info("Loading medium injury spell: {}", str.get<std::string>());
                    RE::TESForm *form = GetFormFromString(str.get<std::string>());
                    if (form && form->GetFormType() == RE::FormType::Spell)
                    {
                        Forms::medium_injuries.push_back(form->As<RE::SpellItem>());
                        logs::info("Loaded medium injury spell: {}", EDID::GetEditorID(form));
                    }
                }

                // Major injuries
                for (const auto &str : j["major_injuries"])
                {
                    logs::info("Loading major injury spell: {}", str.get<std::string>());
                    RE::TESForm *form = GetFormFromString(str.get<std::string>());
                    if (form && form->GetFormType() == RE::FormType::Spell)
                    {
                        Forms::major_injuries.push_back(form->As<RE::SpellItem>());
                        logs::info("Loaded major injury spell: {}", EDID::GetEditorID(form));
                    }
                }

                // Spell upgrades
                for (auto &[fromStr, toStr] : j["spell_upgrades"].items())
                {
                    logs::info("Loading spell upgrade: {} -> {}", fromStr.c_str(), toStr.get<std::string>());
                    RE::TESForm *lowerForm = GetFormFromString(fromStr);
                    RE::TESForm *higherForm = GetFormFromString(toStr.get<std::string>());

                    if (lowerForm && higherForm &&
                        lowerForm->GetFormType() == RE::FormType::Spell &&
                        higherForm->GetFormType() == RE::FormType::Spell)
                    {
                        Forms::spell_upgrades[lowerForm->As<RE::SpellItem>()] = higherForm->As<RE::SpellItem>();
                        logs::info("Loaded spell upgrade: {} -> {}", EDID::GetEditorID(lowerForm), EDID::GetEditorID(higherForm));
                    }
                }

                // Spell downgrades
                for (auto &[fromStr, toStr] : j["spell_downgrades"].items())
                {
                    logs::info("Loading spell downgrade: {} -> {}", fromStr.c_str(), toStr.get<std::string>());
                    RE::TESForm *higherForm = GetFormFromString(fromStr);
                    RE::TESForm *lowerForm = GetFormFromString(toStr.get<std::string>());

                    if (higherForm && lowerForm &&
                        higherForm->GetFormType() == RE::FormType::Spell &&
                        lowerForm->GetFormType() == RE::FormType::Spell)
                    {
                        Forms::spell_downgrades[higherForm->As<RE::SpellItem>()] = lowerForm->As<RE::SpellItem>();
                        logs::info("Loaded spell downgrade: {} -> {}", EDID::GetEditorID(higherForm), EDID::GetEditorID(lowerForm));
                    }
                }

                logs::info("Loaded {} minor, {} medium, and {} major injuries from {}.",
                           Forms::minor_injuries.size(),
                           Forms::medium_injuries.size(),
                           Forms::major_injuries.size(),
                           configFilePath);
            }

            static inline void LoadAllInjuryConfigs(const std::string &folderPath)
            {
                if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath))
                {
                    logs::error("Injury config folder not found: {}", folderPath);
                    return;
                }

                for (const auto &entry : std::filesystem::directory_iterator(folderPath))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".json")
                    {
                        logs::info("Loading injury config: {}", entry.path().string());
                        LoadInjuryConfig(entry.path().string());
                    }
                }

                logs::info("Finished loading all injury configs.");
            }
        }
    }
}