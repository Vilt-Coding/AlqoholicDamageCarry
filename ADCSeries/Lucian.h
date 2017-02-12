#pragma once
#include "BaseChampion.h"

class AlqoholicLucian
{
public:
	void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Lucian");

		ComboMenu = MainMenu->AddMenu("Combo Settings");
		ComboQ = ComboMenu->CheckBox("Use Q", true);
		ComboW = ComboMenu->CheckBox("Use W", true);
		ComboE = ComboMenu->CheckBox("Use E", true);
		ComboR = ComboMenu->AddKey("Semi-Manual R", 84);

		HarassMenu = MainMenu->AddMenu("Harass Settings");
		HarassQ = HarassMenu->CheckBox("Use Q", true);
		HarassExQ = HarassMenu->CheckBox("Use Extended Q [WIP]", true);
		HarassW = HarassMenu->CheckBox("Use W", false);
		HarassMana = HarassMenu->AddFloat("Min. Mana", 0, 100, 60);

		FarmMenu = MainMenu->AddMenu("Farm Settings");
		FarmQ = FarmMenu->CheckBox("Use Q", true);
		FarmW = FarmMenu->CheckBox("Use W", false);
		FarmWMinions = FarmMenu->AddInteger("Min. W Minions", 1, 6, 2);
		FarmMana = FarmMenu->AddFloat("Min. Mana", 0, 100, 40);

		MiscMenu = MainMenu->AddMenu("Misc Settings");
		MiscPassive = MiscMenu->CheckBox("Smart Passive Logic", true);
		KSQ = MiscMenu->CheckBox("KS Q", true);
		KSR = MiscMenu->CheckBox("KS R", true);

		DrawingsMenu = MainMenu->AddMenu("Drawing Settings");
		DrawReady = DrawingsMenu->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
		DrawW = DrawingsMenu->CheckBox("Draw W", true);
		DrawE = DrawingsMenu->CheckBox("Draw E", true);
		DrawR = DrawingsMenu->CheckBox("Draw R", true);
	}

	void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithNothing);
		Q->SetSkillshot(0.2f, 50.f, 1600.f, 675.f);
		W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, kCollidesWithMinions);
		W->SetSkillshot(0.3f, 80.f, 1600.f, 1000.f);
		E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing);
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithMinions);
		R->SetSkillshot(0.2f, 110.f, 2500.f, 1400.f);
	}

	void Draw()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(0, 255, 0, 255), W->Range()); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(0, 0, 255, 255), E->Range()); }
			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 255, 255), R->Range()); }
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 0, 0, 255), Q->Range()); }
			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(0, 255, 0, 255), W->Range()); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(0, 0, 255, 255), E->Range()); }
			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 255, 255), R->Range()); }
		}
	}

	void CheckPassive()
	{
		if (GEntityList->Player()->HasBuff("LucianPassiveBuff"))
		{
			HasPassive = true;
		}
		else
		{
			HasPassive = false;
		}
	}

	void Automatic()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (KSQ->Enabled() && Q->IsReady())
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GEntityList->Player()->IsValidTarget(enemy, Q->Range()))
				{
					if (enemy->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotQ))
					{
						Q->CastOnTarget(enemy);
					}
				}
			}
			if (KSR->Enabled() && R->IsReady())
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GEntityList->Player()->IsValidTarget(enemy, R->Range()))
				{
					if (enemy->GetHealth() < (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotR) / 2))
					{
						R->CastOnTarget(enemy, kHitChanceHigh);
					}
				}
			}
		}
	}

	void QReset()
	{
			if (ComboQ->Enabled() && Q->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
				Q->CastOnTarget(target, kHitChanceHigh);
			}
	}
	
	void WReset()
	{
			if (ComboW->Enabled() && W->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());
				W->CastOnTarget(target, kHitChanceHigh);
			}
	}

	void EReset()
	{
			if (ComboE->Enabled() && E->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				E->CastOnPosition(GGame->CursorPosition());
			}
	}

	void Combo()
	{
		if (!MiscPassive->Enabled() || HasPassive == false)
		{
			if (ComboR->Enabled() && R->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());
				if (target->GetHealth() < (GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotR) / 2))
				{
					R->CastOnTarget(target, kHitChanceHigh);
				}
			}
		}
	}

	void Harass()
	{
		if (!MiscPassive->Enabled() || HasPassive == false)
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());


			if (HarassQ->Enabled() && Q->IsReady())
			{
				Q->CastOnTarget(target);
			}

			if (ComboW->Enabled() && W->IsReady())
			{
				W->CastOnTarget(target, kHitChanceHigh);
			}
		}
	}

	void Farm()
	{
		if (!MiscPassive->Enabled() || HasPassive == false)
		{
			if (GEntityList->Player()->ManaPercent() >= FarmMana->GetFloat())
			{
				for (auto minion : GEntityList->GetAllMinions(false, true, false))
				{
					if (minion->IsEnemy(GEntityList->Player()) && !minion->IsDead() && GEntityList->Player()->IsValidTarget(minion, Q->Range()))
					{
						if (FarmW->Enabled() && W->IsReady())
						{
							W->CastOnTarget(minion, kHitChanceHigh);
						}
						if (FarmQ->Enabled() && Q->IsReady())
						{
							Q->CastOnTarget(minion);
						}
					}
				}
			}
		}
	}
};
