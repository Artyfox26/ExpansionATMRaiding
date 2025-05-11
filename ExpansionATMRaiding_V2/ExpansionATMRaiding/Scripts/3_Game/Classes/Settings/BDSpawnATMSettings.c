class SpawnATMSettings
{
    bool Enable;
    ref array<ref ATMData> ATM;

    void SpawnATMSettings()
    {
        ATM = new ref array<ref ATMData>;
    }

    static ref SpawnATMSettings Load()
    {
        // Use new config path from ConfigATMSpawn.c
        string configPath = "$profile:\ExpansionMod\\ATM_Raiding\\SpawnSettings.json";
        SpawnATMSettings m_Settings = new SpawnATMSettings();
        if (FileExist(configPath))
        {
            JsonFileLoader<SpawnATMSettings>.JsonLoadFile(configPath, m_Settings);
        }
        else
        {
            m_Settings.Defaults();
            JsonFileLoader<SpawnATMSettings>.JsonSaveFile(configPath, m_Settings);
        }
        return m_Settings;
    }

    void Defaults()
    {
        Enable = true;
        ATM.Insert(new ATMData("ExpansionATM_1", "Location Name Here", "10 20 30", "10 20 30"));
        ATM.Insert(new ATMData("ExpansionATM_2", "Location Name Here", "10 20 30", "10 20 30"));
        ATM.Insert(new ATMData("ExpansionATM_3", "Location Name Here", "10 20 30", "10 20 30"));
    }
};
