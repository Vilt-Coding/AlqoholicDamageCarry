#include "PluginSDK.h"
#include "Vayne.h"
#include "Kalista.h"
#include "BaseChampion.h"
#include "Corki.h"
#include "Lucian.h"

PluginSetup("ADCSeries");

#pragma region Events
PLUGIN_EVENT(void) OnOrbwalkBeforeAttack(IUnit* Target)
{

}

PLUGIN_EVENT(void) OnOrbwalkAttack(IUnit* Source, IUnit* Target)
{

}
#pragma endregion

class IChampion
{
public:
	virtual void OnGameUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void AfterAttack(IUnit * Source, IUnit * Target) = 0;
	virtual void OnGapCloser(GapCloserSpell const& Args) = 0;
	virtual void OnLoad() = 0;
};

class Vayne : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicVayne().DrawMenu();
		AlqoholicVayne().LoadSpells();
	}

	void OnRender() override
	{
		AlqoholicVayne().Draw();
	}

	void OnGameUpdate() override
	{
		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicVayne().Combo();
		}
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{
		AlqoholicVayne().AntiGapclose(Args);
	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		AlqoholicVayne().QAfterAttack(Source, Target);
	}
};

class Kalista : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicKalista().DrawMenu();
		AlqoholicKalista().LoadSpells();
	}
	void OnRender() override
	{
		AlqoholicKalista().Draw();
	}

	void OnGameUpdate() override
	{
		AlqoholicKalista().Automatic();

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicKalista().Combo();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			AlqoholicKalista().Farm();
		}
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		AlqoholicKalista().QAfterAttack(Source, Target);
	}
};

class Corki : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicCorki().DrawMenu();
		AlqoholicCorki().LoadSpells();
	}
	void OnRender() override
	{
		AlqoholicCorki().Draw();
	}

	void OnGameUpdate() override
	{
		AlqoholicCorki().Automatic();

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicCorki().Combo();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			AlqoholicCorki().Harass();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			AlqoholicCorki().Farm();
		}
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{

	}
};

class Lucian : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicLucian().DrawMenu();
		AlqoholicLucian().LoadSpells();
	}
	void OnRender() override
	{
		AlqoholicLucian().Draw();
	}

	void OnGameUpdate() override
	{
		//ERRROR

		AlqoholicLucian().Automatic();
		AlqoholicLucian().CheckPassive();

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicLucian().Combo();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			AlqoholicLucian().Harass();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			AlqoholicLucian().Farm();
		}
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		AlqoholicLucian().EReset();
		AlqoholicLucian().QReset();
		AlqoholicLucian().WReset();
	}
};

IChampion* pChampion = nullptr;

PLUGIN_EVENT(void) OnRender()
{
	pChampion->OnRender();
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	pChampion->OnGameUpdate();
}

PLUGIN_EVENT(void) AfterAttack(IUnit* Source, IUnit* Target)
{
	pChampion->AfterAttack(Source, Target);
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Args)
{
	pChampion->OnGapCloser(Args);
}

void LoadChampion()
{
	std::string szChampion = GEntityList->Player()->ChampionName();

	if (szChampion == "Vayne")
		pChampion = new Vayne;
	else if (szChampion == "Kalista")
		pChampion = new Kalista;
	else if (szChampion == "Corki")
		pChampion = new Corki;
	else if (szChampion == "Lucian")
		pChampion = new Lucian;
	else
	{
		GGame->PrintChat("Champion not supported!");
	}
}

// Called when plugin is first loaded
PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	// Initializes global interfaces for core access
	PluginSDKSetup(PluginSDK);

	LoadChampion();
	pChampion->OnLoad();
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
}

// Called when plugin is unloaded
PLUGIN_API void OnUnload()
{
	MainMenu->Remove();
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
}