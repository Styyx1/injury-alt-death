#include "ResurrectionAPI.h"
#include "settings.h"
#include "cache.h"
#include "ondeatheffects.h"
#include "utility.h"
#include "sleephandler.h"
#include "hooks.h"
#include "magiceffectevent.h"

class ResurrectionManager : public ResurrectionAPI
{
	bool should_resurrect(RE::Actor *a) const override
	{
		RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
		if (a == player)
		{
			return true;			
		}
		else
		{
			return false;
		}
	}

	bool busy = false;
	int counter = 0;
	int debugCounter = 0;

	void resurrect(RE::Actor *a) override
	{
		RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
		if (a == player)
		{
			// do resurrection related stuff
			DeathEffects::ResEffects::SetEthereal(player);
		}
		return;
	}
};

void addSubscriber()
{
	if (auto pluginHandle = REX::W32::GetModuleHandleA("ResurrectionAPI.dll"))
	{
		if (auto AddSubscriber = (AddSubscriber_t)GetProcAddress(pluginHandle, "ResurrectionAPI_AddSubscriber"))
		{
			AddSubscriber(std::make_unique<ResurrectionManager>());
		}
	}
}

void InitListener(SKSE::MessagingInterface::Message *a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded)
	{
		Settings::Forms::LoadForms();
		Utility::Injuries::InitializeUpgrades();
		Utility::Injuries::InitializeDowngrades();
		addSubscriber();
		SleepEvents::InstallEvents();
		Hooks::InstallHooks();
		Effect::InstallEvents();
		Settings::JSONSettings::JSONValues::LoadAllInjuryConfigs("Data/SKSE/Plugins/SufferingInjuries/");
	}
	if (a_msg->type == SKSE::MessagingInterface::kPostLoadGame)
	{
		Utility::Injuries::PopulateInjuryVecAfterLoad(Cache::GetPlayerSingleton());
	}
}

SKSEPluginLoad(const SKSE::LoadInterface *a_skse)
{
	SKSE::Init(a_skse);
	logs::init();
	SKSE::AllocTrampoline(14 * 2);
	Settings::Values::Update();
	Cache::CacheAddLibAddresses();
	SKSE::GetMessagingInterface()->RegisterListener(InitListener);

	return true;
}
