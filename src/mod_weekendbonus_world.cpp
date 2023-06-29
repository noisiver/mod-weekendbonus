#include "mod_weekendbonus.h"

void WeekendBonus::OnStartup()
{
    Triggered = false;
    LocalTime = time(NULL);

    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
        SetRates(true);
}

void WeekendBonus::OnUpdate(uint32 diff)
{
    CheckTime += Milliseconds(diff);
    if (CheckTime > CheckFrequency)
    {
        LocalTime = time(NULL);

        if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) && !Triggered)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, granting you bonuses!");
            SetRates(true);
        }
        else if (localtime(&LocalTime)->tm_wday == Day::MONDAY && Triggered)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is no longer active.");
            SetRates(false);
        }

        CheckTime = 0s;
    }

    if (Triggered)
    {
        AnnouncementTime += Milliseconds(diff);
        if (AnnouncementTime > AnnouncementFrequency)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is active, granting you bonuses!");
            AnnouncementTime = 0s;
        }
    }
}
