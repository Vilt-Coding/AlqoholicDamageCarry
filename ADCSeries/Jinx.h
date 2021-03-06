#pragma once
#include "BaseChampion.h"

class AlqoholicJinx
{
public:
	static void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Jinx");

		ComboMenu = MainMenu->AddMenu("Combo Settings");
		ComboQ = ComboMenu->CheckBox("Use Q", true);
		ComboW = ComboMenu->CheckBox("Use W", true);
		ComboE = ComboMenu->CheckBox("Use E [SMART]", true);
		ComboR = ComboMenu->CheckBox("Use R", true);

		HarassMenu = MainMenu->AddMenu("Harass Settings");
		HarassW = HarassMenu->CheckBox("Use W", true);
		HarassMana = HarassMenu->AddFloat("Min. Mana", 0, 100, 60);

		MiscMenu = MainMenu->AddMenu("Misc Settings");
		EGapCloser = MiscMenu->CheckBox("Auto E GapClose", true);
		EOnMe = MiscMenu->CheckBox("Cast On Me [True] Cast On End Pos [False]", false);
		KSW = MiscMenu->CheckBox("KS W", true);
		KSR = MiscMenu->CheckBox("KS R", true);
		KSRMinRange = MiscMenu->AddFloat("Min R Range", 0, 1000, 1000);
		KSRMaxRange = MiscMenu->AddFloat("Max R Range", 1000, 3000, 2000);

		DrawingsMenu = MainMenu->AddMenu("Drawing Settings");
		DrawReady = DrawingsMenu->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
		DrawW = DrawingsMenu->CheckBox("Draw W", true);
		DrawE = DrawingsMenu->CheckBox("Draw E", true);
		DrawR = DrawingsMenu->CheckBox("Draw R", true);
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, kCollidesWithNothing);
		W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, false, static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall));
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, false, kCollidesWithYasuoWall);
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, true, true, static_cast<eCollisionFlags>(kCollidesWithHeroes | kCollidesWithYasuoWall));
	}

	static void Automatic()
	{
		R->SetOverrideRange(KSRMaxRange->GetFloat());

		if (KSR->Enabled() && R->IsReady())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GEntityList->Player()->IsValidTarget(enemy, R->Range()))
				{
					if (enemy->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotR)
						&& (GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() > KSRMinRange->GetFloat()
						&& (GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() < KSRMaxRange->GetFloat())
					{
						R->CastOnTarget(enemy, kHitChanceHigh);
					}
				}
			}
		}
		if (KSW->Enabled() && W->IsReady())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GEntityList->Player()->IsValidTarget(enemy, W->Range()))
				{
					if (enemy->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotW))
					{
						W->CastOnTarget(enemy, kHitChanceHigh);
					}
				}
			}
		}
	}

	static void CheckPassive()
	{
		FishBonesActive = GEntityList->Player()->HasBuff("JinxQ") ? true : false;
	}

	static float GetAttackRange(bool FishBones = false)
	{
		return FishBones == true ? 525.f + (GEntityList->Player()->GetSpellLevel(kSlotQ) * 25 + 50) : 525.f;
	}

	static void Combo()
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

		if (target == nullptr) return;

		if (ComboQ->Enabled())
		{
			if (FishBonesActive == false 
				&& GetAttackRange(true) > (GEntityList->Player()->GetPosition() - target->GetPosition()).Length() 
				&& GetAttackRange(false) < (GEntityList->Player()->GetPosition() - target->GetPosition()).Length())
			{
				Q->CastOnPlayer();
			}
			else if ((GEntityList->Player()->GetPosition() - target->GetPosition()).Length() < GetAttackRange(false) && FishBonesActive == true)
			{
				Q->CastOnPlayer();
			}
		}

		if (ComboW->Enabled())
		{
			W->CastOnTarget(target, kHitChanceHigh);
		}

		if (ComboE->Enabled())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if ((GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() < enemy->AttackRange())
					E->CastOnTarget(enemy, kHitChanceHigh);
			}
		}
	}

	static void Harass()
	{
		if (GEntityList->Player()->ManaPercent() < HarassMana->GetFloat()) return;
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

		if (target == nullptr) return;
		if (HarassW->Enabled())
		{
			W->CastOnTarget(target, kHitChanceHigh);
		}
	}

	static void Farm()
	{

	}

	static void AntiGapE(GapCloserSpell Args)
	{
		if (Args.Sender != GEntityList->Player()
			&& Args.Sender->IsEnemy(GEntityList->Player())
			&& GEntityList->Player()->IsValidTarget(Args.Sender, E->Range() + Args.Sender->BoundingRadius())
			&& EGapCloser->Enabled() && E->IsReady()
			&& (GEntityList->Player()->GetPosition() - Args.EndPosition).Length() < 300)
		{
			EOnMe->Enabled() ? E->CastOnPlayer() : E->CastOnPosition(Args.EndPosition);
		}
	}

	static void Draw()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (R->IsReady() && DrawR->Enabled())
			{
				GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), KSRMinRange->GetFloat());
				GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), KSRMaxRange->GetFloat());
			}
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (DrawR->Enabled())
			{
				GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), KSRMinRange->GetFloat());
				GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), KSRMaxRange->GetFloat());
			}
		}
	}
};
