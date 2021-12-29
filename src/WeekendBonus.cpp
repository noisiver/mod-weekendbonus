#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

float multiplierExperience;
float multiplierMoney;
float multiplierReputation;

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

class WeekendBonusPlayer : public PlayerScript
{
    public:
        WeekendBonusPlayer() : PlayerScript("WeekendBonusPlayer") {}

        void OnLogin(Player* player) override
        {
            time_t t = time(NULL);

            if (localtime(&t)->tm_wday == Day::FRIDAY || localtime(&t)->tm_wday == Day::SATURDAY || localtime(&t)->tm_wday == Day::SUNDAY)
            {
                ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, granting you bonuses to experience, reputation and money!");
            }
        }
};

class WeekendBonusWorld : WorldScript
{
    public:
        WeekendBonusWorld() : WorldScript("WeekendBonusWorld") {}

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            multiplierExperience = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Experience", 2.0f);
            multiplierMoney = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Money", 2.0f);
            multiplierReputation = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Reputation", 2.0f);
        }

        void OnStartup() override
        {
            triggered = false;
            localTime = time(NULL);

            if (localtime(&localTime)->tm_wday == Day::FRIDAY || localtime(&localTime)->tm_wday == Day::SATURDAY || localtime(&localTime)->tm_wday == Day::SUNDAY)
                SetWorldRates();
        }

        void OnUpdate(uint32 diff) override
        {
            HandleWeekendBonuses();
        }

    private:
        bool triggered;
        time_t localTime;

        void HandleWeekendBonuses()
        {
            localTime = time(NULL);

            if (localtime(&localTime)->tm_wday == Day::FRIDAY && localtime(&localTime)->tm_hour == 0 && localtime(&localTime)->tm_min == 0)
            {
                if (!triggered)
                {
                    sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, granting you bonuses to experience, reputation and money!");
                    SetWorldRates();
                }
            }
            else if (localtime(&localTime)->tm_wday == Day::MONDAY && localtime(&localTime)->tm_hour == 0 && localtime(&localTime)->tm_min == 0)
            {
                if (triggered)
                {
                    sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is no longer active.");
                    SetWorldRates();
                }
            }
        }

        void SetWorldRates()
        {
            if (!triggered)
            {
                sWorld->setRate(RATE_XP_KILL, sWorld->getRate(RATE_XP_KILL) * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL, sWorld->getRate(RATE_XP_BG_KILL) * multiplierExperience);
                sWorld->setRate(RATE_XP_QUEST, sWorld->getRate(RATE_XP_QUEST) * multiplierExperience);
                sWorld->setRate(RATE_XP_QUEST_DF, sWorld->getRate(RATE_XP_QUEST_DF) * multiplierExperience);
                sWorld->setRate(RATE_XP_EXPLORE, sWorld->getRate(RATE_XP_EXPLORE) * multiplierExperience);
                sWorld->setRate(RATE_XP_PET, sWorld->getRate(RATE_XP_PET) * multiplierExperience);
                sWorld->setRate(RATE_DROP_MONEY, sWorld->getRate(RATE_DROP_MONEY) * multiplierMoney);
                sWorld->setRate(RATE_REPUTATION_GAIN, sWorld->getRate(RATE_REPUTATION_GAIN) * multiplierReputation);
                triggered = true;
            }
            else
            {
                sWorld->setRate(RATE_XP_KILL, sWorld->getRate(RATE_XP_KILL) / multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL, sWorld->getRate(RATE_XP_BG_KILL) / multiplierExperience);
                sWorld->setRate(RATE_XP_QUEST, sWorld->getRate(RATE_XP_QUEST) / multiplierExperience);
                sWorld->setRate(RATE_XP_QUEST_DF, sWorld->getRate(RATE_XP_QUEST_DF) / multiplierExperience);
                sWorld->setRate(RATE_XP_EXPLORE, sWorld->getRate(RATE_XP_EXPLORE) / multiplierExperience);
                sWorld->setRate(RATE_XP_PET, sWorld->getRate(RATE_XP_PET) / multiplierExperience);
                sWorld->setRate(RATE_DROP_MONEY, sWorld->getRate(RATE_DROP_MONEY) / multiplierMoney);
                sWorld->setRate(RATE_REPUTATION_GAIN, sWorld->getRate(RATE_REPUTATION_GAIN) / multiplierReputation);
                triggered = false;
            }
        }
};

void AddWeekendBonusScripts()
{
    new WeekendBonusPlayer();
    new WeekendBonusWorld();
}
