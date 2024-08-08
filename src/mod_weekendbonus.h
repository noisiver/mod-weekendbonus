#ifndef MOD_WEEKENDBONUS_H
#define MOD_WEEKENDBONUS_H

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

class WeekendBonus : public WorldScript
{
public:
    WeekendBonus();

    // WorldScript
    void OnAfterConfigLoad(bool /*reload*/) override;
    void OnStartup() override;
    void OnUpdate(uint32 /*diff*/) override;

private:
    void LoadDefaultValues();
    void SetRates(bool /*active*/);

    bool Triggered;
    time_t LocalTime;
    Milliseconds CheckFrequency;
    Milliseconds CheckTime;
    Milliseconds AnnouncementFrequency;
    Milliseconds AnnouncementTime;

    float ExperienceMultiplier;
    float MoneyMultiplier;
    uint32 ProfessionsMultiplier;
    float ReputationMultiplier;
    uint32 ProficienciesMultiplier;
    float HonorMultiplier;

    float DefaultExperienceMultiplier[6];
    float DefaultBattlegroundExperienceMultiplier[6];
    uint32 DefaultProfessionsMultiplier[2];
    uint32 DefaultProficienciesMultiplier[2];
    float DefaultMoneyMultiplier;
    float DefaultReputationMultiplier;
    float DefaultHonorMultiplier;
};

#endif
