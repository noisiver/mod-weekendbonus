#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

uint32 multiplierExperience;
uint32 multiplierReputation;

enum Day
{
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

class WeekendBonus : public PlayerScript
{
    public:
        WeekendBonus() : PlayerScript("WeekendBonus") {}

        void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == Day::FRIDAY || localtime(&t)->tm_wday == Day::SATURDAY || localtime(&t)->tm_wday == Day::SUNDAY)
                if (multiplierExperience > 1)
                    amount *= multiplierExperience;
        }

        void OnReputationChange(Player* player, uint32 /*factionId*/, int32& standing, bool /*incremental*/) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == Day::FRIDAY || localtime(&t)->tm_wday == Day::SATURDAY || localtime(&t)->tm_wday == Day::SUNDAY)
                if (multiplierReputation > 1)
                    standing *= multiplierReputation;
        }

        void OnLogin(Player* player) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == Day::FRIDAY || localtime(&t)->tm_wday == Day::SATURDAY || localtime(&t)->tm_wday == Day::SUNDAY)
            {
                if (multiplierExperience > 1 && multiplierReputation > 1)
                {
                    ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the experience and reputation gained!");
                }
                else if (multiplierExperience > 1)
                {
                    ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the experience gained!");
                }
                else if (multiplierReputation > 1)
                {
                    ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the reputation gained!");
                }
            }
        }
};

class WeekendBonusConfig : WorldScript
{
    public:
        WeekendBonusConfig() : WorldScript("WeekendBonusConfig") {}

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            multiplierExperience = sConfigMgr->GetOption<int32>("WeekendBonus.Multiplier.Experience", 1);
            multiplierReputation = sConfigMgr->GetOption<int32>("WeekendBonus.Multiplier.Reputation", 1);
        }
};

class WeekendBonusAnnouncement : WorldScript
{
    public:
        WeekendBonusAnnouncement() : WorldScript("WeekendBonusAnnouncement") {}

        void OnStartup() override
        {
            triggered = false;
        }

        void OnUpdate(uint32 diff) override
        {
            DoAnnouncements();
        }

        private:
            bool triggered;
            time_t localTime;

            void DoAnnouncements()
            {
                localTime = time(NULL);

                if (localtime(&localTime)->tm_wday == Day::FRIDAY && localtime(&localTime)->tm_hour == 0 && localtime(&localTime)->tm_min == 0)
                {
                    if (!triggered)
                    {
                        if (multiplierExperience > 1 && multiplierReputation > 1)
                        {
                            sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, increasing the experience and reputation gained!");
                        }
                        else if (multiplierExperience > 1)
                        {
                            sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, increasing the experience gained!");
                        }
                        else if (multiplierReputation > 1)
                        {
                            sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, increasing the reputation gained!");
                        }

                        triggered = true;
                    }
                }
                else if (localtime(&localTime)->tm_wday == Day::MONDAY && localtime(&localTime)->tm_hour == 0 && localtime(&localTime)->tm_min == 0)
                {
                    if (!triggered)
                    {
                        sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is no longer active.");
                        triggered = true;
                    }
                }
                else
                {
                    if (triggered)
                        triggered = false;
                }
            }
};

void AddWeekendBonusScripts()
{
    new WeekendBonus();
    new WeekendBonusConfig();
    new WeekendBonusAnnouncement();
}
