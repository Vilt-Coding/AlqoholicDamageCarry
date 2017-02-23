#pragma once
#include "PluginSDK.h"

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

IMenu* MainMenu;
IMenu* ComboMenu;
IMenu* HarassMenu;
IMenu* FarmMenu;
IMenu* MiscMenu;
IMenu* DrawingsMenu;
IMenu* QMenu;
IMenu* WMenu;
IMenu* EMenu;
IMenu* RMenu;
IMenu* Misc;
IMenu* ManaMenu;
IMenu* Drawings;

IMenuOption* ComboQ;
IMenuOption* ComboW;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* RSafeRange;

IMenuOption* HarassQ;
IMenuOption* QMinionHarass;
IMenuOption* HarassW;
IMenuOption* HarassE;
IMenuOption* HarassR;
IMenuOption* HarassExQ;
IMenuOption* HarassEStacks;
IMenuOption* HarassMana;

IMenuOption* FarmQ;
IMenuOption* FarmW;
IMenuOption* FarmE;
IMenuOption* FarmR;
IMenuOption* FarmQMinions;
IMenuOption* FarmWMinions;
IMenuOption* FarmEMinions;
IMenuOption* FarmEMonsters; //Baron Dragon
IMenuOption* FarmMana;

IMenuOption* AutoEKillable;
IMenuOption* PushDistance;
IMenuOption* MiscSheen;
IMenuOption* MiscPassive;
IMenuOption* KSQ;
IMenuOption* KSW;
IMenuOption* KSE;
IMenuOption* KSR;
IMenuOption* KSRMinRange;
IMenuOption* KSRMaxRange;
IMenuOption* MiscQ;
IMenuOption* MiscQHealth;
IMenuOption* MiscW;
IMenuOption* MiscE;
IMenuOption* MiscEDamage; //E Damage Reduction
IMenuOption* MiscR;
IMenuOption* MiscRSave; //Ally Saving
IMenuOption* MiscWDragon;
IMenuOption* MiscWBaron;
IMenuOption* EGapCloser;
IMenuOption* EOnMe;
IMenuOption* EOnEndPos;
IMenuOption* FocusW;
IMenuOption* AutoE;
IMenuOption* SemiE;
IMenuOption* SemiR;
IMenuOption* AutoR;
IMenuOption* AutoREnemies;
IMenuOption* StackTear;
IMenuOption* TearMana;
IMenuOption* HitChance;

IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawEDamage;
IMenuOption* DrawR;

bool HasPassive = false;

bool FishBonesActive = false;

Vec3 rPosCast;
IUnit* rTargetLast;
Vec3 rPosLast;