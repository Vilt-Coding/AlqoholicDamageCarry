#pragma once
#include "BaseChampion.h"

class AlqoholicCorki
{
public:

	void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Corki");

		ComboMenu = MainMenu->AddMenu("Combo Settings");
		ComboQ = ComboMenu->CheckBox("Use Q", true);
		ComboE = ComboMenu->CheckBox("Use E", true);
		ComboR = ComboMenu->CheckBox("Use R", true);

		HarassMenu = MainMenu->AddMenu("Harass Settings");
		HarassQ = HarassMenu->CheckBox("Use Q", true);
		HarassR = HarassMenu->CheckBox("Use R", true);
		HarassMana = HarassMenu->AddFloat("Min. Mana", 0, 100, 60);

		FarmMenu = MainMenu->AddMenu("Farm Settings");
		FarmQ = FarmMenu->CheckBox("Use Q", true);
		FarmE = FarmMenu->CheckBox("Use E", false);
		FarmQMinions = FarmMenu->AddInteger("Min. Q Minions", 1, 6, 2);
		FarmMana = FarmMenu->AddFloat("Min. Mana", 0, 100, 40);

		MiscMenu = MainMenu->AddMenu("Misc Settings");
		MiscSheen = MiscMenu->CheckBox("Sheen Logic", true);
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
		Q = GPluginSDK->CreateSpell2(kSlotQ, kCircleCast, true, true, kCollidesWithYasuoWall);
		Q->SetSkillshot(0.5f, 200.f, 1000.f, 825.f);
		W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, false, false, kCollidesWithNothing);
		E = GPluginSDK->CreateSpell2(kSlotE, kConeCast, false, true, kCollidesWithYasuoWall);
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, true, kCollidesWithMinions);
		R->SetSkillshot(0.2f, 40.f, 2000.f, 1230.f);
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
						Q->CastOnTarget(enemy, kHitChanceHigh);
					}
				}
			}
			if (KSR->Enabled() && R->IsReady())
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GEntityList->Player()->IsValidTarget(enemy, R->Range()))
				{
					if (enemy->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotR))
					{
						if (GPrediction->GetCollisionFlagsForPoint(enemy->GetPosition()) == 0)
						{
							R->CastOnTarget(enemy, kHitChanceHigh);
						}
					}
				}
			}
		}
	}

	void Combo()
	{
		if (!MiscSheen->Enabled() || !GEntityList->Player()->HasBuff("sheen"))
		{
			if (ComboQ->Enabled() && Q->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
				if (target != nullptr)
					Q->CastOnTarget(target, kHitChanceHigh);
			}
			if (ComboE->Enabled() && E->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
				if (target != nullptr 
					&& GEntityList->Player()->IsValidTarget(target, 500) 
					&& (GEntityList->Player()->GetPosition() - target->GetPosition()).Length() < 500)
					E->CastOnPlayer();
			}
			if (ComboR->Enabled() && R->IsReady())
			{
				auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
				if (target != nullptr)
					R->CastOnTarget(target, kHitChanceHigh);
			}
		}
	}

	void Harass()
	{

		if (HarassQ->Enabled() && Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			if (target != nullptr)
				Q->CastOnTarget(target, kHitChanceHigh);
		}
		if (HarassR->Enabled() && R->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
			if (target != nullptr)
			R->CastOnTarget(target, kHitChanceHigh);
		}
	}

	void Farm()
	{
		if ((FarmQ->Enabled() || FarmE->Enabled()) && GEntityList->Player()->ManaPercent() >= FarmMana->GetFloat())
		{
			for (auto minion : GEntityList->GetAllMinions(false, true, false))
			{
				if (minion->IsEnemy(GEntityList->Player()) && !minion->IsDead() && GEntityList->Player()->IsValidTarget(minion, Q->Range()))
				{
					if (FarmQ->Enabled() && Q->IsReady())
					{
						Vec3 pos;
						int hit;
						GPrediction->FindBestCastPosition(Q->Range(), Q->Radius(), true, true, false, pos, hit);
						if (hit >= FarmQMinions->GetInteger())
						{
							Q->CastOnPosition(pos);
						}
					}
					if (FarmE->Enabled() && E->IsReady())
					{
						E->CastOnPlayer();
					}
				}
			}
		}
	}

};
