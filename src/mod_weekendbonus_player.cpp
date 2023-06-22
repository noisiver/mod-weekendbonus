#include "mod_weekendbonus.h"

void WeekendBonus::OnLogin(Player* player)
{
    LocalTime = time(NULL);

    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
        ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, granting you bonuses!");
}
