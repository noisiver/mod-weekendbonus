#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

class WeekendBonus : public PlayerScript
{
    public:
        WeekendBonus() : PlayerScript("WeekendBonus") {}

        void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == 5 /*Friday*/ || localtime(&t)->tm_wday == 6 /*Saturday*/ || localtime(&t)->tm_wday == 0 /*Sunday*/)
                amount *= sConfigMgr->GetIntDefault("Multiplier.Experience", 1);
        }

        void OnReputationChange(Player* player, uint32 /*factionId*/, int32& standing, bool /*incremental*/) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == 5 /*Friday*/ || localtime(&t)->tm_wday == 6 /*Saturday*/ || localtime(&t)->tm_wday == 0 /*Sunday*/)
                standing *= sConfigMgr->GetIntDefault("Multiplier.Reputation", 1);
        }

        void OnLogin(Player* player) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == 5 /*Friday*/ || localtime(&t)->tm_wday == 6 /*Saturday*/ || localtime(&t)->tm_wday == 0 /*Sunday*/)
                ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, increasing the experience and reputation received!");
        }
};

void AddWeekendBonusScripts()
{
    new WeekendBonus();
}
