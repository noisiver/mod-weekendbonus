#include "mod_weekendbonus.h"

WeekendBonus::WeekendBonus() : WorldScript("WeekendBonusWorldScript") {}

void Addmod_weekendbonusScripts()
{
    new WeekendBonus();
}
