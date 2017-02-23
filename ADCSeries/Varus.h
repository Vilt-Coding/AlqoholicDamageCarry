#pragma once
#include "BaseChampion.h"

class AlqoholicVarus
{
public:
	static void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Varus");

		QMenu = MainMenu->AddMenu("Q Settings");
		WMenu = MainMenu->AddMenu("W Settings");
		EMenu = MainMenu->AddMenu("E Settings");
		RMenu = MainMenu->AddMenu("R Settings");
		ManaMenu = MainMenu->AddMenu("Mana Settings");
		DrawingsMenu = MainMenu->AddMenu("Drawing Settings");
		MiscMenu = MainMenu->AddMenu("Misc Settings");

		ComboQ = QMenu->CheckBox("[Combo] Use Q", true);
		HarassQ = QMenu->CheckBox("[Harass] Use Q", true);
		FarmQ = QMenu->CheckBox("[Farm] Use Q", true);
		FarmQMinions = QMenu->AddInteger("[Farm] Min. Hit", 1, 6, 3);
		KSQ = QMenu->CheckBox("[KS] Use Q", true);

		//ComboW = WMenu->CheckBox("[Combo] Focus W", true);
		MiscW = WMenu->AddInteger("[Combo] Only Proc Spells on X stacks (0 Disable)", 0, 3, 0);

		ComboE = EMenu->CheckBox("[Combo] Use E", true);
		HarassE = EMenu->CheckBox("[Harass] Use E", true);
		FarmE = EMenu->CheckBox("[Farm] Use E", true);
		FarmEMinions = EMenu->AddInteger("[Farm] Min. Hit", 1, 6, 3);
		KSE = EMenu->CheckBox("[KS] Use E", true);

		ComboR = RMenu->CheckBox("[Combo] Use R", true);
		MiscR = RMenu->AddInteger("[Combo] Min. Hit", 1, 5, 2);
		SemiR = RMenu->AddKey("[Misc] Semi-Auto R", 84);

		FarmMana = ManaMenu->AddFloat("[Harass] Min. Mana", 0, 100, 50);
		HarassMana = ManaMenu->AddFloat("[Harass] Min. Mana", 0, 100, 50);

		DrawReady = DrawingsMenu->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
		DrawE = DrawingsMenu->CheckBox("Draw E", true);
		DrawR = DrawingsMenu->CheckBox("Draw R", true);

		HitChance = MiscMenu->AddInteger("[Misc] HitChance (3 Low, 4 Medium, 5 High, 6 VeryHigh",
			kHitChanceLow,
			kHitChanceVeryHigh,
			kHitChanceHigh);
		StackTear = MiscMenu->CheckBox("[Misc] Stack Tear", true);
		TearMana = MiscMenu->AddFloat("[Misc] Min. Mana", 0, 100, 90);
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, kCollidesWithYasuoWall);
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, kCollidesWithYasuoWall);
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithYasuoWall);

		Q->SetCharged(1000.f, 1550.f, 1.5f);
	}

	static void Draw()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), 1000.f); }
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), 1550.f); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), 1000.f); }
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), 1550.f); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}
	}

	static void KS()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy != nullptr)
			{
				if (KSQ->Enabled() && Q->IsReady())
				{
					if (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotQ) > enemy->GetHealth()
						&& GEntityList->Player()->IsValidTarget(enemy, 1550.f))
					{
						if (Q->IsCharging())
						{
							Q->CastOnTarget(enemy, HitChance->GetInteger());
						}
						else
						{
							Q->StartCharging();
						}
					}
				}

				if (KSE->Enabled() && E->IsReady())
				{
					if (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotE) > enemy->GetHealth()
						&& GEntityList->Player()->IsValidTarget(enemy, E->Range()))
					{
						E->CastOnTarget(enemy, HitChance->GetInteger());
					}
				}
			}
		}
	}

	static int GetWStacks(IUnit* target)
	{
		return target->GetBuffCount("varuswdebuff");
	}

	static std::vector<IUnit *> EnemiesWithW()
	{
		std::vector<IUnit *> enemiesWithW = std::vector<IUnit *>();
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy->HasBuff("varuswdebuff")
				&& enemy->GetTeam() != GEntityList->Player()->GetTeam()
				&& (GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() < E->Range())
			{
				enemiesWithW.push_back(enemy);
			}
		}
		return enemiesWithW;
	}

	static int GetEnemiesInRRange(IUnit* target)
	{
		auto enemiesInRange = 0;

		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy != nullptr && GEntityList->Player()->IsValidTarget(enemy, R->Range()))
			{
				auto distance = (target->GetPosition() - enemy->GetPosition()).Length();
				if (distance < 500)
				{
					enemiesInRange++;
				}
			}
		}
		return enemiesInRange;
	}

	static void Combo()
	{
		IUnit* target = nullptr;
		//auto enemiesWithW = EnemiesWithW();
		//
		//if (FocusW->Enabled())
		//{
		//	switch (EnemiesWithW().size())
		//	{
		//	case 0:
		//		target = nullptr;
		//		break;
		//
		//	case 1:
		//		target = enemiesWithW[0];
		//		break;
		//
		//	default:
		//		std::sort(enemiesWithW.begin(), enemiesWithW.end(), [](IUnit* first, IUnit* second)
		//		{
		//			return first->GetHealth() < second->GetHealth();
		//		});
		//		target = enemiesWithW[0];
		//		break;
		//	}
		//}

		if (ComboQ->Enabled() && Q->IsReady())
		{
			if (target == nullptr)
				target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 1550.f);


			if (target != nullptr)
			{
				if (GetWStacks(target) < MiscW->GetInteger()) return;
				if (Q->IsCharging())
				{
					Q->CastOnTarget(target, HitChance->GetInteger());
					return;
				}
				Q->StartCharging();
			}
		}
		if (ComboE->Enabled() && E->IsReady())
		{
			if (target == nullptr)
				target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());
			
			if (target != nullptr)
			{
				if (GetWStacks(target) < MiscW->GetInteger()) return;
				E->CastOnTarget(target, HitChance->GetInteger());
			}
		}
		if (ComboR->Enabled() && R->IsReady())
		{
			for (auto rTarget : GEntityList->GetAllHeros(false, true))
			{
				if (rTarget != nullptr
					&& (GEntityList->Player()->GetPosition() - rTarget->GetPosition()).Length() < R->Range()
					&& GEntityList->Player()->IsValidTarget(rTarget, R->Range()))
				{
					if (GetEnemiesInRRange(rTarget) >= MiscR->GetInteger())
					{
						R->CastOnTarget(rTarget, HitChance->GetInteger());
						break;
					}
				}
			}
		}
	}

	static void Harass()
	{
		IUnit* target = nullptr;

		if (HarassMana->GetFloat() >= GEntityList->Player()->ManaPercent()) return;

		if (HarassQ->Enabled() && Q->IsReady())
		{
			if (target == nullptr)
				target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 1550.f);


			if (target != nullptr)
			{
				if (GetWStacks(target) < MiscW->GetInteger()) return;
				if (Q->IsCharging())
				{
					Q->CastOnTarget(target, HitChance->GetInteger());
					return;
				}
				Q->StartCharging();
			}
		}
		if (HarassE->Enabled() && E->IsReady())
		{
			if (target == nullptr)
				target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());


			if (target != nullptr)
			{
				if (GetWStacks(target) < MiscW->GetInteger()) return;
				E->CastOnTarget(target, HitChance->GetInteger());
			}
		}
	}

	static void Farm()
	{
		if (GEntityList->Player()->ManaPercent() >= FarmMana->GetFloat())
		{
			if (FarmQ->Enabled() && Q->IsReady())
			{
				Vec3 castPos;
				int minionsHit;
				Q->FindBestCastPosition(true, false, castPos, minionsHit);

				if (minionsHit >= FarmQMinions->GetInteger())
				{
					if (Q->IsCharging())
					{
						Q->CastOnPosition(castPos);
						return;
					}
					Q->StartCharging();
				}
			}
			if (FarmE->Enabled() && E->IsReady())
			{
				Vec3 castPos;
				int minionsHit;
				E->FindBestCastPosition(true, false, castPos, minionsHit);

				if (minionsHit >= FarmEMinions->GetInteger())
				{
					E->CastOnPosition(castPos);
				}
			}
		}
	}
};
