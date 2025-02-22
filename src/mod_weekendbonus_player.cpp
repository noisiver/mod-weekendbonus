#include "Chat.h"

#include "mod_weekendbonus.h"

void WeekendBonus::OnPlayerLogin(Player* player)
{
    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
    {
        ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, granting you bonuses!");
    }
}
