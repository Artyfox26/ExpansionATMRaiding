class PaycheckSystemConfig
{
    private static string PaycheckSystemConfigPATH = "$profile:\\ExpansionMod\\Paycheck\\Paychecksystem.json";
    int Version = 5;
    int EnablePaycheckSystem = 1;
    int PaycheckAmount = 1250;
    int PaycheckPayoutTime = 45;
    int ConfigUpdateRate = 1;
    ref array<ref PlayerBonus> BonusPlayers = new array<ref PlayerBonus>;

    void Load()
    {
        if (GetGame().IsServer())
        {
            if (FileExist(PaycheckSystemConfigPATH))
            {
                JsonFileLoader<PaycheckSystemConfig>.JsonLoadFile(PaycheckSystemConfigPATH, this);
            }
            else
            {
                Version = 5;
                EnablePaycheckSystem = 1;
                PaycheckAmount = 1250;
                PaycheckPayoutTime = 45;
                ConfigUpdateRate = 1;
                BonusPlayers = new array<ref PlayerBonus>;
                Save();
            }
        }
    }

    void Save()
    {
        JsonFileLoader<PaycheckSystemConfig>.JsonSaveFile(PaycheckSystemConfigPATH, this);
    }

    void SetPaycheckInterval(int newInterval)
    {
        int payoutInterval = newInterval * 60 * 1000;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AddMoneyToActivePlayers);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AddMoneyToActivePlayers, payoutInterval, true);
    }

    void CheckForConfigUpdates()
    {
        int oldPayoutTime = PaycheckPayoutTime;
        int oldConfigUpdateRate = ConfigUpdateRate;
        Load();
        bool changesDetected = false;
        string changeLog = "[Paycheck] Changes Detected: ";
        if (PaycheckPayoutTime != oldPayoutTime)
        {
            SetPaycheckInterval(PaycheckPayoutTime);
            changeLog += "PaycheckPayoutTime changed from " + oldPayoutTime + " to " + PaycheckPayoutTime + ", ";
            changesDetected = true;
        }
        if (ConfigUpdateRate != oldConfigUpdateRate)
        {
            changeLog += "ConfigUpdateRate changed from " + oldConfigUpdateRate + " to " + ConfigUpdateRate + ", ";
            changesDetected = true;
        }
        foreach (PlayerBonus bonusData : BonusPlayers)
        {
            if (bonusData.BonusTimerRemaining > 0)
            {
                int oldBonusTimer = bonusData.BonusTimerRemaining;
                bonusData.BonusTimerRemaining -= ConfigUpdateRate;
                if (bonusData.BonusTimerRemaining <= 0)
                {
                    bonusData.BonusTimerRemaining = 0;
                    bonusData.Bonus = 0;
                    changeLog += "Bonus expired for " + bonusData.NameForRef + ", ";
                }
                else
                {
                    changeLog += "BonusTimerRemaining decreased for " + bonusData.NameForRef + " from " + oldBonusTimer + " to " + bonusData.BonusTimerRemaining + ", ";
                }
                changesDetected = true;
            }
            if (bonusData.MultiplierTimerRemaining > 0)
            {
                int oldMultiplierTimer = bonusData.MultiplierTimerRemaining;
                bonusData.MultiplierTimerRemaining -= ConfigUpdateRate;
                if (bonusData.MultiplierTimerRemaining <= 0)
                {
                    bonusData.MultiplierTimerRemaining = 0;
                    bonusData.Multiplier = 1;
                    changeLog += "Multiplier expired for " + bonusData.NameForRef + ", ";
                }
                else
                {
                    changeLog += "MultiplierTimerRemaining decreased for " + bonusData.NameForRef + " from " + oldMultiplierTimer + " to " + bonusData.MultiplierTimerRemaining + ", ";
                }
                changesDetected = true;
            }
        }
        if (changesDetected)
        {
            Save();
        }
    }

    void AddMoneyToActivePlayers()
    {
        Load();
        if (!EnablePaycheckSystem)
            return;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        ExpansionMarketModule marketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
        if (!marketModule)
            return;
        foreach (Man player : players)
        {
            if (!player.GetIdentity())
                continue;
            string expansionID = player.GetIdentity().GetId();
            string playerName = player.GetIdentity().GetName();
            PlayerBonus existingBonus = PlayerBonus.FindBonusForPlayer(expansionID, BonusPlayers);
            if (!existingBonus)
            {
                existingBonus = new PlayerBonus(expansionID, playerName, "New Player", 1, 0, 0, 0, GetGame().GetTime() / 1000);
                BonusPlayers.Insert(existingBonus);
            }
            int basePaycheck = PaycheckAmount;
            int finalAmount = (basePaycheck * existingBonus.Multiplier) + existingBonus.Bonus;
            ExpansionMarketATM_Data playerATMData = marketModule.GetPlayerATMData(expansionID);
            if (playerATMData)
            {
                playerATMData.AddMoney(finalAmount);
            }
        }
        Save();
    }
};

ref PaycheckSystemConfig m_PaycheckSystemConfig;

static PaycheckSystemConfig GetPaycheckSystemConfig()
{
    if (!m_PaycheckSystemConfig)
    {
        m_PaycheckSystemConfig = new PaycheckSystemConfig;
        m_PaycheckSystemConfig.Load();
    }
    return m_PaycheckSystemConfig;
}
