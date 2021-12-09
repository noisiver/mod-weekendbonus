#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

uint32 multiplierExperience;
uint32 multiplierReputation;

class WeekendBonus : public PlayerScript
{
    public:
        WeekendBonus() : PlayerScript("WeekendBonus") {}

        void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == 5 /*Friday*/ || localtime(&t)->tm_wday == 6 /*Saturday*/ || localtime(&t)->tm_wday == 0 /*Sunday*/)
                if (multiplierExperience > 1)
                    amount *= multiplierExperience;
        }

        void OnReputationChange(Player* player, uint32 /*factionId*/, int32& standing, bool /*incremental*/) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == 5 /*Friday*/ || localtime(&t)->tm_wday == 6 /*Saturday*/ || localtime(&t)->tm_wday == 0 /*Sunday*/)
                if (multiplierReputation > 1)
                    standing *= multiplierReputation;
        }

        void OnLogin(Player* player) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == 5 /*Friday*/ || localtime(&t)->tm_wday == 6 /*Saturday*/ || localtime(&t)->tm_wday == 0 /*Sunday*/)
            {
                if (multiplierExperience > 1 && multiplierReputation > 1)
                {
                    ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the experience and reputation received!");
                }
                else if (multiplierExperience > 1)
                {
                    ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the experience received!");
                }
                else if (multiplierReputation > 1)
                {
                    ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the reputation received!");
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
            multiplierExperience = sConfigMgr->GetIntDefault("Multiplier.Experience", 1);
            multiplierReputation = sConfigMgr->GetIntDefault("Multiplier.Reputation", 1);
        }
};

void AddWeekendBonusScripts()
{
    new WeekendBonus();
    new WeekendBonusConfig();
}
