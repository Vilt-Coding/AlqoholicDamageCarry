#pragma once
#include <array>
#include <string>
#include "BaseChampion.h"

class AlqoholicKalista
{
public:
	Vec3 baronLocation = Vec3(5064.f, 10568.f, -71.f);
	Vec3 dragonLocation = Vec3(9796.f, 4432.f, -71.f);

	void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Kalista");

		ComboMenu = MainMenu->AddMenu("Combo Settings");
		ComboQ = ComboMenu->CheckBox("Use Q", true);
		ComboE = ComboMenu->CheckBox("Use E", true);

		HarassMenu = MainMenu->AddMenu("Harass Settings [WIP/SOON]");
		HarassQ = HarassMenu->CheckBox("Use Q", true);
		HarassE = HarassMenu->CheckBox("Use E", true);
		HarassEStacks = HarassMenu->AddInteger("E at X Stacks [0 to Disable]", 0, 10, 6);
		HarassMana = HarassMenu->AddFloat("Min. Mana", 0, 100, 60);

		FarmMenu = MainMenu->AddMenu("Farm Settings");
		FarmQ = FarmMenu->CheckBox("Use Q", true);
		FarmE = FarmMenu->CheckBox("Use E", true);
		FarmEMinions = FarmMenu->AddInteger("Min. E Minions", 1, 6, 2);
		FarmEMonsters = FarmMenu->CheckBox("Auto E Dragon/Baron", true);
		FarmMana = FarmMenu->AddFloat("Min. Mana", 0, 100, 40);

		MiscMenu = MainMenu->AddMenu("Misc Settings");
		AutoEKillable = MiscMenu->CheckBox("Auto E Killable", true);
		MiscEDamage = MiscMenu->AddFloat("E Damage Reduction %", 0, 10, 3);
		MiscR = ComboMenu->CheckBox("Use R to Save Ally", true);
		MiscRSave = ComboMenu->AddFloat("Ally Health %", 0, 100, 15);
		MiscWDragon = MiscMenu->AddKey("W To Dragon", 71);
		MiscWBaron = MiscMenu->AddKey("W To Baron", 72);

		DrawingsMenu = MainMenu->AddMenu("Drawing Settings");
		DrawReady = DrawingsMenu->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
		DrawW = DrawingsMenu->CheckBox("Draw W", true);
		DrawE = DrawingsMenu->CheckBox("Draw E", true);
		DrawEDamage = DrawingsMenu->CheckBox("Draw E Damage", true);
		DrawR = DrawingsMenu->CheckBox("Draw R", true);
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, kCollidesWithMinions);
		Q->SetSkillshot(0.25, 40.f, 1200.f, 1150.f);
		W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, false, false, kCollidesWithNothing);
		W->SetOverrideRange(5000.f);
		E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, kCollidesWithNothing);
		E->SetOverrideRange(1000.f);
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, kCollidesWithNothing);
		R->SetOverrideRange(1000.f);
	}

	void Draw() const
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }
			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}

		if (DrawEDamage->Enabled() && !GEntityList->Player()->IsDead())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if ((GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() <= E->Range())
				{
					if (enemy->HasBuff("kalistaexpungemarker") && !enemy->IsDead())
					{
						std::array<Vec3, 1> w2s;
						auto eDamageText = "E Damage: " + std::to_string(GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotE));
						GGame->Projection(enemy->GetPosition(), &w2s[0]);

						if (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotE) >= enemy->GetHealth())
						{
							GRender->DrawTextW(Vec2(w2s[0].x - 80, w2s[0].y + 50), Vec4(255, 0, 0, 255), eDamageText.c_str());
						}
						else if (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotE) >= enemy->GetHealth() * 0.75)
						{
							GRender->DrawTextW(Vec2(w2s[0].x - 80, w2s[0].y + 50), Vec4(255, 255, 0, 0), eDamageText.c_str());
						}
						else
						{
							GRender->DrawTextW(Vec2(w2s[0].x - 80, w2s[0].y + 50), Vec4(255, 255, 255, 255), eDamageText.c_str());
						}
					}
				}
			}
		}

	}

	static void CastE()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy->IsEnemy(GEntityList->Player()) && (GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() <= E->Range())
			{
				auto eDamage = GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotE) - (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotE) * (MiscEDamage->GetFloat() / 10));
				if (eDamage > enemy->GetHealth() && enemy->HasBuff("kalistaexpungemarker"))
				{
					E->CastOnPlayer();
				}
			}
		}
	}

	static void StealE()
	{
		for (auto minion : GEntityList->GetAllMinions(false, false, true))
		{
			auto minionName = minion->GetObjectName();
			if (std::strstr(minionName, "baron") != nullptr || std::strstr(minionName, "dragon") != nullptr || std::strstr(minionName, "herald") != nullptr)
			{
				auto player = GEntityList->Player();
				if (player->IsValidTarget(minion, E->Range()))
				{
					auto dmg = static_cast<float>(GDamage->GetSpellDamage(player, minion, kSlotE));
					auto health = GHealthPrediction->GetPredictedHealth(minion, kLastHitPrediction, 250, 250);

					if (dmg > health)
					{
						E->CastOnPlayer();
					}
				}
			}
		}
	}

	static void SaveAlly()
	{
		for (auto ally : GEntityList->GetAllHeros(true, false))
		{
			if (!ally->IsEnemy(GEntityList->Player()) && !ally->IsDead() && ally->HasBuff("kalistacoopstrikeally"))
			{
				if ((GEntityList->Player()->GetPosition() - ally->GetPosition()).Length() < R->Range() && ally->HealthPercent() <= MiscRSave->GetFloat())
				{
					auto enemies = 0;
					for (auto enemy : GEntityList->GetAllHeros(false, true))
					{
						if (enemy->IsEnemy(ally) && !enemy->IsDead())
						{
							if ((ally->GetPosition() - enemy->GetPosition()).Length() < 550)
							{
								enemies++;
							}
						}
					}
					if (enemies >= 2)
					{
						R->CastOnPlayer();
					}
				}
			}
		}
	}

	static void Automatic()
	{
		if (AutoEKillable->Enabled() && E->IsReady())
		{
			CastE();
		}
		if (FarmEMonsters->Enabled() && E->IsReady())
		{
			StealE();
		}
		if (MiscR->Enabled() && R->IsReady())
		{
			SaveAlly();
		}

		/*if (MiscWDragon->Enabled() && W->IsReady())
		{
			if ((GEntityList->Player()->GetPosition() - dragonLocation).Length() < W->Range())
			{
				W->CastOnPosition(dragonLocation);
			}
		}
		if (MiscWBaron->Enabled() && W->IsReady())
		{
			if ((GEntityList->Player()->GetPosition() - baronLocation).Length() < W->Range())
			{
				W->CastOnPosition(baronLocation);
			}
		}*/
	}

	static void QAfterAttack(IUnit* Source, IUnit* Target)
	{
		if (Source == GEntityList->Player())
		{
			if (ComboQ->Enabled() && Q->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
				if (GPrediction->GetCollisionFlagsForPoint(qTarget->GetPosition()) == 0)
				{
					Q->CastOnTarget(qTarget, kHitChanceHigh);
				}
			}
		}
	}

	static void Combo()
	{
		auto qTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, GEntityList->Player()->AttackRange());

		GOrbwalking->SetOverrideTarget(target);

		if (Q->IsReady() && ComboQ->Enabled())
		{
			Q->CastOnTarget(qTarget, kHitChanceHigh);
		}
		if (E->IsReady() && ComboE->Enabled())
		{
			CastE();
		}
	}

	static void Harass()
	{
		//TODO: Not a priority
	}

	static void Farm()
	{
		if (GEntityList->Player()->ManaPercent() >= FarmMana->GetFloat())
		{
			auto eMinions = 0;

			for (auto minion : GEntityList->GetAllMinions(false, true, false))
			{
				if (minion->IsEnemy(GEntityList->Player()) && !minion->IsDead() && GEntityList->Player()->IsValidTarget(minion, E->Range()))
				{
					if (FarmQ->Enabled() && Q->IsReady())
					{
						if (GHealthPrediction->GetPredictedHealth(minion, kLastHitPrediction, 250, 250) < GDamage->GetSpellDamage(GEntityList->Player(), minion, kSlotQ))
						{
							auto hit = GPrediction->GetCollisionFlagsForPoint(minion->GetPosition());
							if (hit == 0)
							{
								Q->CastOnTarget(minion, kHitChanceHigh);
							}
						}
					}
					if (FarmE->Enabled() && E->IsReady())
					{
						if (minion->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), minion, kSlotE))
						{
							eMinions++;
						}
					}
				}
			}
			if (FarmE->Enabled() && E->IsReady())
			{
				if (eMinions >= FarmEMinions->GetInteger())
				{
					E->CastOnPlayer();
				}
			}
		}
	}

};
