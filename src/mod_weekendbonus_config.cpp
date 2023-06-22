#include "mod_weekendbonus.h"

void WeekendBonus::OnAfterConfigLoad(bool reload)
{
    if (!reload)
        LoadDefaultValues();

    float ExperienceMultiplier = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Experience", 2.0f);
    float MoneyMultiplier = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Money", 2.0f);
    uint32 ProfessionsMultiplier = sConfigMgr->GetOption<uint32>("WeekendBonus.Multiplier.Professions", 2);
    float ReputationMultiplier = sConfigMgr->GetOption<float>("WeekendBonus.Multiplier.Reputation", 2.0f);
    uint32 ProficienciesMultiplier = sConfigMgr->GetOption<uint32>("WeekendBonus.Multiplier.Proficiencies", 2);

    if (reload)
    {
        if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
            SetRates(true);
        else
            SetRates(false);
    }
}

void WeekendBonus::OnStartup()
{
    Triggered = false;
    LocalTime = time(NULL);

    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) || localtime(&LocalTime)->tm_wday == Day::SATURDAY || localtime(&LocalTime)->tm_wday == Day::SUNDAY)
        SetRates(true);
}

void WeekendBonus::OnUpdate(uint32 /*diff*/)
{
    LocalTime = time(NULL);

    if ((localtime(&LocalTime)->tm_wday == Day::FRIDAY && localtime(&LocalTime)->tm_hour >= 18) && !Triggered)
    {
        sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is now active, granting you bonuses!");
        SetRates(true);
        Triggered = true;
    }
    else if (localtime(&LocalTime)->tm_wday == Day::MONDAY && Triggered)
    {
        sWorld->SendServerMessage(SERVER_MSG_STRING, "The weekend bonus is no longer active.");
        SetRates(false);
        Triggered = false;
    }
}

void WeekendBonus::LoadDefaultValues()
{
    DefaultExperienceMultiplier[0] = sWorld->getRate(RATE_XP_KILL);
    DefaultExperienceMultiplier[1] = sWorld->getRate(RATE_XP_QUEST);
    DefaultExperienceMultiplier[2] = sWorld->getRate(RATE_XP_QUEST_DF);
    DefaultExperienceMultiplier[3] = sWorld->getRate(RATE_XP_EXPLORE);
    DefaultExperienceMultiplier[4] = sWorld->getRate(RATE_XP_PET);
    DefaultBattlegroundExperienceMultiplier[0] = sWorld->getRate(RATE_XP_BG_KILL_AV);
    DefaultBattlegroundExperienceMultiplier[1] = sWorld->getRate(RATE_XP_BG_KILL_WSG);
    DefaultBattlegroundExperienceMultiplier[2] = sWorld->getRate(RATE_XP_BG_KILL_AB);
    DefaultBattlegroundExperienceMultiplier[3] = sWorld->getRate(RATE_XP_BG_KILL_EOTS);
    DefaultBattlegroundExperienceMultiplier[4] = sWorld->getRate(RATE_XP_BG_KILL_SOTA);
    DefaultBattlegroundExperienceMultiplier[5] = sWorld->getRate(RATE_XP_BG_KILL_IC);
    DefaultMoneyMultiplier = sWorld->getRate(RATE_DROP_MONEY);
    DefaultProfessionsMultiplier[0] = sWorld->getIntConfig(CONFIG_SKILL_GAIN_CRAFTING);
    DefaultProfessionsMultiplier[1] = sWorld->getIntConfig(CONFIG_SKILL_GAIN_GATHERING);
    DefaultReputationMultiplier = sWorld->getRate(RATE_REPUTATION_GAIN);
    DefaultProficienciesMultiplier[0] = sWorld->getIntConfig(CONFIG_SKILL_GAIN_DEFENSE);
    DefaultProficienciesMultiplier[1] = sWorld->getIntConfig(CONFIG_SKILL_GAIN_WEAPON);
}

void WeekendBonus::SetRates(bool active)
{
    if (active)
    {
        sWorld->setRate(RATE_XP_KILL, DefaultExperienceMultiplier[0] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_QUEST, DefaultExperienceMultiplier[1] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_QUEST_DF, DefaultExperienceMultiplier[2] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_EXPLORE, DefaultExperienceMultiplier[3] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_PET, DefaultExperienceMultiplier[4] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_BG_KILL_AV, DefaultBattlegroundExperienceMultiplier[0] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_BG_KILL_WSG, DefaultBattlegroundExperienceMultiplier[1] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_BG_KILL_AB, DefaultBattlegroundExperienceMultiplier[2] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_BG_KILL_EOTS, DefaultBattlegroundExperienceMultiplier[3] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_BG_KILL_SOTA, DefaultBattlegroundExperienceMultiplier[4] * ExperienceMultiplier);
        sWorld->setRate(RATE_XP_BG_KILL_IC, DefaultBattlegroundExperienceMultiplier[5] * ExperienceMultiplier);
        sWorld->setRate(RATE_DROP_MONEY, DefaultMoneyMultiplier * MoneyMultiplier);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_CRAFTING, DefaultProfessionsMultiplier[0] * ProfessionsMultiplier);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_GATHERING, DefaultProfessionsMultiplier[1] * ProfessionsMultiplier);
        sWorld->setRate(RATE_REPUTATION_GAIN, DefaultReputationMultiplier * ReputationMultiplier);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_DEFENSE, DefaultProficienciesMultiplier[0] * ProficienciesMultiplier);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_WEAPON, DefaultProficienciesMultiplier[1] * ProficienciesMultiplier);
    }
    else
    {
        sWorld->setRate(RATE_XP_KILL, DefaultExperienceMultiplier[0]);
        sWorld->setRate(RATE_XP_QUEST, DefaultExperienceMultiplier[1]);
        sWorld->setRate(RATE_XP_QUEST_DF, DefaultExperienceMultiplier[2]);
        sWorld->setRate(RATE_XP_EXPLORE, DefaultExperienceMultiplier[3]);
        sWorld->setRate(RATE_XP_PET, DefaultExperienceMultiplier[4]);
        sWorld->setRate(RATE_XP_BG_KILL_AV, DefaultBattlegroundExperienceMultiplier[0]);
        sWorld->setRate(RATE_XP_BG_KILL_WSG, DefaultBattlegroundExperienceMultiplier[1]);
        sWorld->setRate(RATE_XP_BG_KILL_AB, DefaultBattlegroundExperienceMultiplier[2]);
        sWorld->setRate(RATE_XP_BG_KILL_EOTS, DefaultBattlegroundExperienceMultiplier[3]);
        sWorld->setRate(RATE_XP_BG_KILL_SOTA, DefaultBattlegroundExperienceMultiplier[4]);
        sWorld->setRate(RATE_XP_BG_KILL_IC, DefaultBattlegroundExperienceMultiplier[5]);
        sWorld->setRate(RATE_DROP_MONEY, DefaultMoneyMultiplier);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_CRAFTING, DefaultProfessionsMultiplier[0]);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_GATHERING, DefaultProfessionsMultiplier[1]);
        sWorld->setRate(RATE_REPUTATION_GAIN, DefaultReputationMultiplier);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_DEFENSE, DefaultProficienciesMultiplier[0]);
        sWorld->setIntConfig(CONFIG_SKILL_GAIN_WEAPON, DefaultProficienciesMultiplier[1]);
    }
}
