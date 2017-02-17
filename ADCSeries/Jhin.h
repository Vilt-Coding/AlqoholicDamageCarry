#pragma once
#include "BaseChampion.h"
#include "PluginData.h"

class AlqoholicJhin
{
public:

	static void DrawMenu()
	{
		MainMenu = GPluginSDK->AddMenu("AlqoholicDamageCarry - Jhin");

		QMenu = MainMenu->AddMenu("Q Settings");
		WMenu = MainMenu->AddMenu("W Settings");
		EMenu = MainMenu->AddMenu("E Settings");
		RMenu = MainMenu->AddMenu("R Settings");
		DrawingsMenu = MainMenu->AddMenu("Drawings");

		ComboQ = QMenu->CheckBox("[Combo] Use Q", true);
		//HarassQ = QMenu->CheckBox("[Harass] Use Q", true);
		//FarmQ = QMenu->CheckBox("[Farm] Use Q", true);
		KSQ = QMenu->CheckBox("[KS] Use Q", true);

		ComboW = WMenu->CheckBox("[Combo] Use W", true);
		FocusW = WMenu->CheckBox("[Misc] Only W Marked", true);
		//FarmW = WMenu->CheckBox("[Farm] Use W", true);
		KSW = WMenu->CheckBox("[KS] Use W", true);

		EGapCloser = EMenu->CheckBox("[Misc] Auto Anti-GapCloser", true);

		SemiR = RMenu->AddKey("[Misc] Semi-Auto R [TAP TO SHOOT]", 84);
		KSRMinRange = RMenu->AddFloat("[Misc] Min R Range", 0, 1000, 1000);
		KSRMaxRange = RMenu->AddFloat("[Misc] Max R Range", 1000, 3000, 2800);
		RSafeRange = RMenu->AddFloat("[Misc] R Safe Range", 1000, 2000, 1400);

		DrawReady = DrawingsMenu->CheckBox("Draw Only Ready Spells", true);
		DrawQ = DrawingsMenu->CheckBox("Draw Q", true);
		DrawW = DrawingsMenu->CheckBox("Draw W", true);
		DrawE = DrawingsMenu->CheckBox("Draw E", true);
		DrawR = DrawingsMenu->CheckBox("Draw R", true);
	}

	static void LoadSpells()
	{
		Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithYasuoWall);
		W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, false, kCollidesWithYasuoWall);
		E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, true, kCollidesWithNothing);
		R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithYasuoWall);

		W->SetOverrideRange(2500.f);
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

	static void KS()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy != nullptr)
			{
				if ((GDamage->GetAutoAttackDamage(GEntityList->Player(), enemy, true) > enemy->GetHealth())
					&& (GEntityList->Player()->GetPosition() - enemy->GetPosition()).Length() < GEntityList->Player()->GetRealAutoAttackRange(enemy))
					return;

				if (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotQ) > enemy->GetHealth() 
					&& GEntityList->Player()->IsValidTarget(enemy, Q->Range())
					&& Q->IsReady()
					&& KSQ->Enabled())
				{
					Q->CastOnTarget(enemy);
				}

				if (GDamage->GetSpellDamage(GEntityList->Player(), enemy, kSlotW) > enemy->GetHealth() 
					&& GEntityList->Player()->IsValidTarget(enemy, W->Range())
					&& W->IsReady()
					&& KSW->Enabled())
				{
					W->CastOnTarget(enemy, kHitChanceHigh);
				}
			}
		}
	}

	static IUnit* GetTargetWithW()
	{
		for (auto enemy : GEntityList->GetAllHeros(false, true))
		{
			if (enemy->HasBuff("jhinespotteddebuff")
				&& enemy->GetTeam() != GEntityList->Player()->GetTeam()
				&& (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length() < W->Range())
			{
				return enemy;
			}
		}
		return nullptr;
	}

	static int GetEnemiesInRange(float range)
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

	static void AntiGapclose(GapCloserSpell const& Args)
	{
		if (Args.Sender != GEntityList->Player()
			&& Args.Sender->IsEnemy(GEntityList->Player())
			&& GEntityList->Player()->IsValidTarget(Args.Sender, E->Range() + Args.Sender->BoundingRadius())
			&& EGapCloser->Enabled() && E->IsReady()
			&& (GEntityList->Player()->GetPosition() - Args.EndPosition).Length() < 300)
		{
			E->CastOnPosition(Args.EndPosition);
		}
	}

	static void QAfterAttack(IUnit* Source, IUnit* Target)
	{
		if (Source == GEntityList->Player())
		{
			if (ComboQ->Enabled() 
				&& Q->IsReady() 
				&& GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				if (GDamage->GetAutoAttackDamage(GEntityList->Player(), Target, true) > Target->GetHealth())
					return;

				Q->CastOnTarget(Target);
			}
		}
	}

	static void WAfterAttack(IUnit* Source, IUnit* Target)
	{
		if (Source == GEntityList->Player())
		{
			if (ComboW->Enabled()
				&& W->IsReady()
				&& GOrbwalking->GetOrbwalkingMode() == kModeCombo
			   	&& (GEntityList->Player()->GetPosition() - Target->GetPosition()).Length() > GEntityList->Player()->AttackRange())
			{
				if (GDamage->GetAutoAttackDamage(GEntityList->Player(), Target, true) > Target->GetHealth())
					return;

				if (FocusW->Enabled())
				{
					auto target = GetTargetWithW();
					if (target == nullptr) return;
					if ((GEntityList->Player()->GetPosition() - target->GetPosition()).Length() > GEntityList->Player()->AttackRange())
					W->CastOnTarget(target);
				}
				else
				{
					W->CastOnTarget(Target);
				}
			}
		}
	}

	static Vec2 Rotate(Vec2 vec2, float angle)
	{
		auto c = cos(angle);
		auto s = sin(angle);

		return Vec2(vec2.x * c - vec2.y * s, vec2.y * c + vec2.x * s);
	}

	static float CrossProduct(Vec2 self, Vec2 other)
	{
		return other.y * self.x - other.x * self.y;
	}

	static bool IsCastingR()
	{
		return GEntityList->Player()->GetSpellName(kSlotR) == "JhinRShot";
	}

	static void OnProcessSpell(CastedSpell const& Args)
	{
		if (Args.Caster_ == GEntityList->Player() && Args.Name_ == "JhinR")
		{
			rPosCast = Args.EndPosition_;
		}
	}

	static bool InCone(Vec3 pos) //TY SEBBY <3
	{
		auto range = KSRMaxRange->GetFloat();
		auto angle = 70.f * (3.14159265359 / 180.f);
		auto end2 = Vec2(rPosCast.x, rPosCast.z) - Vec2(GEntityList->Player()->GetPosition().x, GEntityList->Player()->GetPosition().z);
		auto edge1 = Rotate(end2, -angle / 2);
		auto edge2 = Rotate(edge1, angle);

		auto point = Vec2(pos.x, pos.z) - Vec2(GEntityList->Player()->GetPosition().x, GEntityList->Player()->GetPosition().z);
		if ((point - Vec2()).Length() < range * range && CrossProduct(edge1, point) > 0 && CrossProduct(point, edge2) > 0)
		{
			return true;
		}
		return false;
	}

	static double GetRdmg(IUnit* target)
	{
		auto damage = (-25 + 75 * GEntityList->Player()->GetSpellLevel(kSlotR) + 0.2 * GEntityList->Player()->PhysicalDamageMod()) * (1 + (100 - target->HealthPercent()) * 0.02);

		return GDamage->CalcPhysicalDamage(GEntityList->Player(), target, damage);
	}

	static void CastR()
	{
		if (!IsCastingR())
		{
			R->SetOverrideRange(KSRMaxRange->GetFloat());
		}
		else
		{
			R->SetOverrideRange(3500);
		}

		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, R->Range());
		if (target != nullptr && !target->IsDead() && GEntityList->Player()->IsValidTarget(target, R->Range()))
		{
			R->CastOnTarget(target, kHitChanceHigh);
			rTargetLast = target;
		}

		if (!IsCastingR()
			&& target != nullptr
			&& !target->IsDead()
			&& GetEnemiesInRange(RSafeRange->GetFloat()) == 0
			&& (GEntityList->Player()->GetPosition() - target->GetPosition()).Length() > KSRMinRange->GetFloat())
		{
			R->CastOnTarget(target);
			rTargetLast = target;
		}

		if (IsCastingR())
		{
			if (target != nullptr && !target->IsDead() && AlqoholicJhin().InCone(target->ServerPosition()))
			{
				R->CastOnTarget(target);
			}
			else
			{
				for (auto enemy : GEntityList->GetAllHeros(false, true))
				{
					if (enemy != nullptr && GEntityList->Player()->IsValidTarget(enemy, R->Range()) && AlqoholicJhin().InCone(target->ServerPosition()))
					{
						R->CastOnTarget(target);
						rPosLast = target->ServerPosition();
						rTargetLast = enemy;
					}
				}
			}
		}
		else if (IsCastingR() && rTargetLast != nullptr && !rTargetLast->IsDead())
		{
			if (AlqoholicJhin().InCone(rTargetLast->GetPosition()) && AlqoholicJhin().InCone(rPosLast))
			{
				R->CastOnPosition(rPosLast);
				GEntityList->Player()->HealthPercent();
			}
		}
	}

	static void Combo()
	{
		if (ComboW->Enabled()
			&& FocusW->Enabled()
			&& W->IsReady())
		{
			auto target = GetTargetWithW();
			if (target != nullptr
				&& (GEntityList->Player()->GetPosition() - target->GetPosition()).Length() > GEntityList->Player()->AttackRange())
			{
				W->CastOnTarget(target, kHitChanceHigh);
			}
		}
	}
	
					    
	//static void Farm() 
	//{
	//	if (FarmW->Enabled() && W->IsReady())
	//	{
	//		Vec3 castPos;
	//		int minionsHit;
	//		W->FindBestCastPosition(true, false, castPos, minionsHit);

	//		if (minionsHit > 2)
	//		{
	//			W->CastOnPosition(castPos);
	//		}
	//	}

	//	for (auto minion : GEntityList->GetAllMinions(false, true, true))
	//	{
	//		if (minion == nullptr || minion->IsDead()) return;

	//		if (FarmQ->Enabled() && Q->IsReady())
	//		{
	//			if (GEntityList->Player()->IsValidTarget(minion, Q->Range())
	//				&& GDamage->GetSpellDamage(GEntityList->Player(), minion, kSlotQ) > minion->GetHealth())
	//			{
	//				Q->CastOnUnit(minion);
	//			}
	//		}
	//	}
	//}
};
