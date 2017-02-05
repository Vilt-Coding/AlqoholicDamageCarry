#pragma once

class AlqoholicJinx
{
public:
	void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Jinx");

		ComboMenu = MainMenu->AddMenu("Combo Settings");
		ComboQ = ComboMenu->CheckBox("Use Q", true);
		ComboW = ComboMenu->CheckBox("Use W", true);
		ComboE = ComboMenu->CheckBox("Use E", true);
		ComboR = ComboMenu->CheckBox("Use R", true);

		HarassMenu = MainMenu->AddMenu("Harass Settings [WIP/SOON]");
		HarassW = HarassMenu->CheckBox("Use W", true);
		HarassMana = HarassMenu->AddFloat("Min. Mana", 0, 100, 60);

		MiscMenu = MainMenu->AddMenu("Misc Settings");
		EGapCloser = MiscMenu->CheckBox("Auto E GapClose", true);
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

	void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, kCollidesWithNothing);
		W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, false, kCollidesWithMinions);
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, false, kCollidesWithYasuoWall);
		R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, true, true, kCollidesWithYasuoWall);
	}

	void Automatic()
	{
		R->SetOverrideRange(KSRMaxRange->GetFloat());

		if (KSR->Enabled() && R->IsReady())
		{
			for (auto enemy : GEntityList->GetAllHeros(false, true))
			{
				if (enemy->IsEnemy(GEntityList->Player()) && GEntityList->Player()->IsValidTarget(enemy, R->Range()))
				{
					if (enemy->GetHealth() < GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotR))
					{
						Vec3 pos;
						GPrediction->SimulateMissile(GEntityList->Player()->GetPosition(), enemy, R->Speed(), R->Radius(), R->Range(), R->GetDelay(), 0, pos);

						R->CastOnPosition(pos);
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

	void Combo()
	{
		//if (ComboQ->Enabled() && GEntityList->Player()->HasBuff())
	}
};