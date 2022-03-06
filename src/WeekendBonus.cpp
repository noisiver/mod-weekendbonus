#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

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
                ChatHandler(player->GetSession()).SendSysMessage("The weekend bonus is active, granting you bonuses!");
        }
};

class WeekendBonusWorld : WorldScript
{
    public:
        WeekendBonusWorld() : WorldScript("WeekendBonusWorld") {}

        void OnAfterConfigLoad(bool reload) override
        {
            if (!reload)
            {
                rateExperienceKill       = sWorld->getRate(RATE_XP_KILL);
                rateExperienceBgKill[0]  = sWorld->getRate(RATE_XP_BG_KILL_AV);
                rateExperienceBgKill[1]  = sWorld->getRate(RATE_XP_BG_KILL_WSG);
                rateExperienceBgKill[2]  = sWorld->getRate(RATE_XP_BG_KILL_AB);
                rateExperienceBgKill[3]  = sWorld->getRate(RATE_XP_BG_KILL_EOTS);
                rateExperienceBgKill[4]  = sWorld->getRate(RATE_XP_BG_KILL_SOTA);
                rateExperienceBgKill[5]  = sWorld->getRate(RATE_XP_BG_KILL_IC);
                rateExperienceQuest      = sWorld->getRate(RATE_XP_QUEST);
                rateExperienceQuestDf    = sWorld->getRate(RATE_XP_QUEST_DF);
                rateExperienceExplore    = sWorld->getRate(RATE_XP_EXPLORE);
                rateExperiencePet        = sWorld->getRate(RATE_XP_PET);
                rateMoney                = sWorld->getRate(RATE_DROP_MONEY);
                rateProfessionsCrafting  = sWorld->getIntConfig(CONFIG_SKILL_GAIN_CRAFTING);
                rateProfessionsGathering = sWorld->getIntConfig(CONFIG_SKILL_GAIN_GATHERING);
                rateReputation           = sWorld->getRate(RATE_REPUTATION_GAIN);
                rateProficienciesDefense = sWorld->getIntConfig(CONFIG_SKILL_GAIN_DEFENSE);
                rateProficienciesWeapons = sWorld->getIntConfig(CONFIG_SKILL_GAIN_WEAPON);
            }

            multiplierExperience    = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Experience", 2.0f);
            multiplierMoney         = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Money", 2.0f);
            multiplierProfessions   = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Professions", 2.0f);
            multiplierReputation    = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Reputation", 2.0f);
            multiplierProficiencies = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Proficiencies", 2.0f);

            if (reload)
            {
                if (localtime(&localTime)->tm_wday == Day::FRIDAY || localtime(&localTime)->tm_wday == Day::SATURDAY || localtime(&localTime)->tm_wday == Day::SUNDAY)
                    SetWorldRates(true);
                else
                    SetWorldRates(false);
            }
        }

        void OnStartup() override
        {
            triggered = false;
            localTime = time(NULL);

            if (localtime(&localTime)->tm_wday == Day::FRIDAY || localtime(&localTime)->tm_wday == Day::SATURDAY || localtime(&localTime)->tm_wday == Day::SUNDAY)
                SetWorldRates(true);
        }

        void OnUpdate(uint32 /*diff*/) override
        {
            localTime = time(NULL);

            if (localtime(&localTime)->tm_wday == Day::FRIDAY && localtime(&localTime)->tm_hour == 0 && localtime(&localTime)->tm_min == 0 && !triggered)
            {
                sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, granting you bonuses!");
                SetWorldRates(true);
                triggered = true;
            }
            else if (localtime(&localTime)->tm_wday == Day::MONDAY && localtime(&localTime)->tm_hour == 0 && localtime(&localTime)->tm_min == 0 && triggered)
            {
                sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is no longer active.");
                SetWorldRates(false);
                triggered = false;
            }
        }

    private:
        uint32 rateExperienceKill;
        uint32 rateExperienceBgKill[6];
        uint32 rateExperienceQuest;
        uint32 rateExperienceQuestDf;
        uint32 rateExperienceExplore;
        uint32 rateExperiencePet;
        uint32 rateMoney;
        uint32 rateProfessionsCrafting;
        uint32 rateProfessionsGathering;
        uint32 rateReputation;
        uint32 rateProficienciesDefense;
        uint32 rateProficienciesWeapons;

        uint32 multiplierExperience;
        uint32 multiplierMoney;
        uint32 multiplierProfessions;
        uint32 multiplierReputation;
        uint32 multiplierProficiencies;

        bool triggered;
        time_t localTime;

        void SetWorldRates(bool weekend)
        {
            if (weekend)
            {
                sWorld->setRate(RATE_XP_KILL, rateExperienceKill * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL_AV, rateExperienceBgKill[0] * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL_WSG, rateExperienceBgKill[1] * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL_AB, rateExperienceBgKill[2] * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL_EOTS, rateExperienceBgKill[3] * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL_SOTA, rateExperienceBgKill[4] * multiplierExperience);
                sWorld->setRate(RATE_XP_BG_KILL_IC, rateExperienceBgKill[5] * multiplierExperience);
                sWorld->setRate(RATE_XP_QUEST, rateExperienceQuest * multiplierExperience);
                sWorld->setRate(RATE_XP_QUEST_DF, rateExperienceQuestDf * multiplierExperience);
                sWorld->setRate(RATE_XP_EXPLORE, rateExperienceExplore * multiplierExperience);
                sWorld->setRate(RATE_XP_PET, rateExperiencePet * multiplierExperience);

                sWorld->setRate(RATE_DROP_MONEY, rateMoney * multiplierMoney);

                sWorld->setIntConfig(CONFIG_SKILL_GAIN_CRAFTING, rateProfessionsCrafting * multiplierProfessions);
                sWorld->setIntConfig(CONFIG_SKILL_GAIN_GATHERING, rateProfessionsGathering * multiplierProfessions);

                sWorld->setRate(RATE_REPUTATION_GAIN, rateReputation * multiplierReputation);

                sWorld->setIntConfig(CONFIG_SKILL_GAIN_DEFENSE, rateProficienciesDefense * multiplierProficiencies);
                sWorld->setIntConfig(CONFIG_SKILL_GAIN_WEAPON, rateProficienciesWeapons * multiplierProficiencies);
            }
            else
            {
                sWorld->setRate(RATE_XP_KILL, rateExperienceKill);
                sWorld->setRate(RATE_XP_BG_KILL_AV, rateExperienceBgKill[0]);
                sWorld->setRate(RATE_XP_BG_KILL_WSG, rateExperienceBgKill[1]);
                sWorld->setRate(RATE_XP_BG_KILL_AB, rateExperienceBgKill[2]);
                sWorld->setRate(RATE_XP_BG_KILL_EOTS, rateExperienceBgKill[3]);
                sWorld->setRate(RATE_XP_BG_KILL_SOTA, rateExperienceBgKill[4]);
                sWorld->setRate(RATE_XP_BG_KILL_IC, rateExperienceBgKill[5]);
                sWorld->setRate(RATE_XP_QUEST, rateExperienceQuest);
                sWorld->setRate(RATE_XP_QUEST_DF, rateExperienceQuestDf);
                sWorld->setRate(RATE_XP_EXPLORE, rateExperienceExplore);
                sWorld->setRate(RATE_XP_PET, rateExperiencePet);

                sWorld->setRate(RATE_DROP_MONEY, rateMoney);

                sWorld->setIntConfig(CONFIG_SKILL_GAIN_CRAFTING, rateProfessionsCrafting);
                sWorld->setIntConfig(CONFIG_SKILL_GAIN_GATHERING, rateProfessionsGathering);

                sWorld->setRate(RATE_REPUTATION_GAIN, rateReputation);

                sWorld->setIntConfig(CONFIG_SKILL_GAIN_DEFENSE, rateProficienciesDefense);
                sWorld->setIntConfig(CONFIG_SKILL_GAIN_WEAPON, rateProficienciesWeapons);
            }
        }
};

void AddWeekendBonusScripts()
{
    new WeekendBonusPlayer();
    new WeekendBonusWorld();
}
