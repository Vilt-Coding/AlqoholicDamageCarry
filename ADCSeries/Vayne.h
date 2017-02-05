#pragma once
#include "BaseChampion.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

class AlqoholicVayne
{
public:

	void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Vayne");

		QMenu = MainMenu->AddMenu("Q Settings");
		WMenu = MainMenu->AddMenu("W Settings");
		EMenu = MainMenu->AddMenu("E Settings");
		RMenu = MainMenu->AddMenu("R Settings");
		DrawingsMenu = MainMenu->AddMenu("Drawings");
		GGame->PrintChat("Drawing Menu");

		ComboQ = QMenu->CheckBox("Use Q", true);

		FocusW = WMenu->CheckBox("Focus W Stacks", true);

		ComboE = EMenu->CheckBox("Use E", true);
		AutoE = EMenu->CheckBox("Auto Condemn", true);
		EGapCloser = EMenu->CheckBox("Auto Anti-GapCloser", true);

		AutoR = RMenu->CheckBox("Auto R when enemies >= x", true);
		AutoREnemies = RMenu->AddInteger("Enemies in range", 1, 5, 2);

		DrawReady = DrawingsMenu->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
		DrawE = DrawingsMenu->CheckBox("Draw E", true);
	}

	void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, false, kCollidesWithNothing);
		W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, false, kCollidesWithNothing);
		E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, true, false, kCollidesWithNothing);
		R = GPluginSDK->CreateSpell2(kSlotR, kCircleCast, false, false, kCollidesWithNothing);
		Q->SetOverrideRange(300);
		E->SetOverrideRange(750);
	}

	void Draw()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }
		}
	}

	float DegreeToRadian(double angle)
	{
		return float(M_PI * angle / 180.0);
	}

	void AutoCondemn()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy != nullptr && enemy->GetTeam() != GEntityList->Player()->GetTeam() && GEntityList->Player()->IsValidTarget(enemy, E->Range()))
			{
				auto flDistance = (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length();

				if (flDistance > E->Range()) { return; }

				float pushDistance = 415;
				float forty = 40;
				auto targetPosition = enemy->ServerPosition();
				auto checkDistance = pushDistance / forty;
				auto pushDirection = (targetPosition - GEntityList->Player()->ServerPosition()).VectorNormalize();
				for (auto i = 0; i < 40; i++)
				{
					auto finalPos = targetPosition + (pushDirection * checkDistance * i);
					if (GPrediction->IsPointWall(finalPos))
					{
						E->CastOnUnit(enemy);
					}
				}
			}
		}
	}

	IUnit* GetTargetWithW()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy->GetBuffDataByName("VayneSilveredDebuff")
				&& enemy->GetTeam() != GEntityList->Player()->GetTeam()
				&& (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length() < GEntityList->Player()->AttackRange())
			{
				return enemy;
			}
		}
		return GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, GEntityList->Player()->AttackRange());
	}

	int GetEnemiesInRange(float range)
	{
		auto enemies = GEntityList->GetAllHeros(false, true);
		auto enemiesInRange = 0;

		for (auto enemy : enemies)
		{
			if (enemy != nullptr && enemy->GetTeam() != GEntityList->Player()->GetTeam())
			{
				auto flDistance = (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length();
				if (flDistance < range)
				{
					enemiesInRange++;
				}
			}
		}
		return enemiesInRange;
	}

	void AntiGapclose(GapCloserSpell const& Args)
	{
		if (Args.Sender != GEntityList->Player()
			&& Args.Sender->IsEnemy(GEntityList->Player())
			&& GEntityList->Player()->IsValidTarget(Args.Sender, E->Range() + Args.Sender->BoundingRadius())
			&& EGapCloser->Enabled() && E->IsReady()
			&& (GEntityList->Player()->GetPosition() - Args.EndPosition).Length() < 300)
		{
			E->CastOnUnit(Args.Sender);
		}
	}

	void QAfterAttack(IUnit* Source, IUnit* Target)
	{
		if (Source == GEntityList->Player())
		{
			if (ComboQ->Enabled() && Q->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}
	}

	void Combo()
	{
		if (FocusW->Enabled())
		{
			GOrbwalking->SetOverrideTarget(GetTargetWithW());
		}

		if (AutoE->Enabled() && E->IsReady())
		{
			AutoCondemn();
		}

		if (AutoR->Enabled() && R->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			if (GetEnemiesInRange(600) >= AutoREnemies->GetInteger())
			{
				R->CastOnPlayer();
			}
		}
	}

};