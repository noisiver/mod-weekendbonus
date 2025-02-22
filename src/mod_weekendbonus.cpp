#include "mod_weekendbonus.h"

WeekendBonus::WeekendBonus() : PlayerScript("WeekendBonusPlayerScript"), WorldScript("WeekendBonusWorldScript") {}

void Addmod_weekendbonusScripts()
{
    new WeekendBonus();
}
