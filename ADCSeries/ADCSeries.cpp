#include "PluginSDK.h"
#include "Vayne.h"
#include "Kalista.h"
#include "BaseChampion.h"
#include "Corki.h"
#include "Lucian.h"
#include "Jinx.h"
#include "Jhin.h"
#include "Varus.h"

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
	virtual void BeforeAttack(IUnit* Source, IUnit* Target) = 0;
	virtual void AfterAttack(IUnit * Source, IUnit * Target) = 0;
	virtual void OnGapCloser(GapCloserSpell const& Args) = 0;
	virtual void OnProcessSpell(CastedSpell const& Args) = 0;
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

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		AlqoholicVayne().QAfterAttack(Source, Target);
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

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

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		AlqoholicKalista().QAfterAttack(Source, Target);
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

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

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void OnProcessSpell(CastedSpell const& Args) override
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
		AlqoholicLucian().Automatic();
		AlqoholicLucian().CheckPassive();
		AlqoholicLucian().SemiR();

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

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		if (Source != GEntityList->Player()) return;
		AlqoholicLucian().EReset();
		AlqoholicLucian().WReset();
		AlqoholicLucian().QReset();
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}
};

class Jinx : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicJinx().DrawMenu();
		AlqoholicJinx().LoadSpells();
	}
	void OnRender() override
	{
		AlqoholicJinx().Draw();
	}

	void OnGameUpdate() override
	{
		AlqoholicJinx().Automatic();
		AlqoholicJinx().CheckPassive();

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicJinx().Combo();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			AlqoholicJinx().Harass();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			AlqoholicJinx().Farm();
		}
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{
		AlqoholicJinx().AntiGapE(Args);
	}

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

	}
};

class Jhin : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicJhin().DrawMenu();
		AlqoholicJhin().LoadSpells();
	}
	void OnRender() override
	{
		AlqoholicJhin().Draw();
	}

	void OnGameUpdate() override
	{
		AlqoholicJhin().KS();

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicJhin().Combo();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{

		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			//AlqoholicJhin().Farm();
		}

		if (AlqoholicJhin().IsCastingR())
		{
			GOrbwalking->SetMovementAllowed(false);
			GOrbwalking->SetAttacksAllowed(false);
			AlqoholicJhin().CastR();
		}
		else
		{
			GOrbwalking->SetMovementAllowed(true);
			GOrbwalking->SetAttacksAllowed(true);
		}

		if (GetAsyncKeyState(SemiR->GetInteger())
			&& R->IsReady()
			&& AlqoholicJhin().GetEnemiesInRange(RSafeRange->GetFloat()) == 0
			&& AlqoholicJhin().GetEnemiesInRange(KSRMaxRange->GetFloat()) > 0)
		{
			AlqoholicJhin().CastR();
		}

	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{
		AlqoholicJhin().AntiGapclose(Args);
	}

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{
		AlqoholicJhin().WAfterAttack(Source, Target);
		AlqoholicJhin().QAfterAttack(Source, Target);
	}

	void OnProcessSpell(CastedSpell const& Args) override
	{
		AlqoholicJhin().OnProcessSpell(Args);
	}
};

class Varus : public IChampion
{
public:
	void OnLoad() override
	{
		AlqoholicVarus().DrawMenu();
		AlqoholicVarus().LoadSpells();
	}
	void OnRender() override
	{
		AlqoholicVarus().Draw();
	}

	void OnGameUpdate() override
	{
		AlqoholicVarus().KS();

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			AlqoholicVarus().Combo();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
		{
			AlqoholicVarus().Harass();
		}
		else if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			AlqoholicVarus().Farm();
		}

		if (GetAsyncKeyState(SemiR->GetInteger())
			&& R->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());
			if (target != nullptr && GEntityList->Player()->IsValidTarget(target, R->Range()))
				R->CastOnTarget(target, HitChance->GetInteger());
		}
	}

	void OnGapCloser(GapCloserSpell const& Args) override
	{

	}

	void BeforeAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void AfterAttack(IUnit* Source, IUnit* Target) override
	{

	}

	void OnProcessSpell(CastedSpell const& Args) override
	{

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

PLUGIN_EVENT(void) BeforeAttack(IUnit* Source, IUnit* Target)
{
	pChampion->BeforeAttack(Source, Target);
}

PLUGIN_EVENT(void) AfterAttack(IUnit* Source, IUnit* Target)
{
	pChampion->AfterAttack(Source, Target);
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Args)
{
	pChampion->OnGapCloser(Args);
}

PLUGIN_EVENT(void) OnProcessSpell(CastedSpell const& Args)
{
	pChampion->OnProcessSpell(Args);
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
	else if (szChampion == "Jinx")
		pChampion = new Jinx;
	else if (szChampion == "Jhin")
		pChampion = new Jhin;
	else if (szChampion == "Varus")
		pChampion = new Varus;
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
	GEventManager->AddEventHandler(kEventOnSpellCast, OnProcessSpell);
	GEventManager->AddEventHandler(kEventOrbwalkBeforeAttack, BeforeAttack);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
}

// Called when plugin is unloaded
PLUGIN_API void OnUnload()
{
	MainMenu->SaveSettings();
	MainMenu->Remove();
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnProcessSpell);
	GEventManager->RemoveEventHandler(kEventOrbwalkBeforeAttack, BeforeAttack);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
}
