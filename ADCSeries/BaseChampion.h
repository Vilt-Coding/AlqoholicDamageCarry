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
IMenu* Drawings;

IMenuOption* ComboQ;
IMenuOption* ComboE;

IMenuOption* HarassQ;
IMenuOption* HarassE;
IMenuOption* HarassEStacks;
IMenuOption* HarassMana;

IMenuOption* FarmQ;
IMenuOption* FarmE;
IMenuOption* FarmEMinions;
IMenuOption* FarmEMonsters; //Baron Dragon
IMenuOption* FarmMana;

IMenuOption* AutoEKillable;
IMenuOption* MiscEDamage; //E Damage Reduction
IMenuOption* MiscR;
IMenuOption* MiscRSave; //Ally Saving
IMenuOption* MiscWDragon;
IMenuOption* MiscWBaron;

IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawEDamage;
IMenuOption* DrawR;
IMenuOption* FocusW;
IMenuOption* AutoE;
IMenuOption* EGapCloser;
IMenuOption* AutoR;
IMenuOption* AutoREnemies;