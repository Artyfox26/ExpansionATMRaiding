class ATM_SpawnConfig
{
    private static string ATM_SpawnConfigPATH = "$profile:\\ExpansionMod\\ATM_Raiding\\SpawnSettings.json";
    bool Enable = true;
    ref array<ref ATMData> ATMSpawns = new array<ref ATMData>;

    void Load() {
        if (GetGame().IsServer()) {
            if (FileExist(ATM_SpawnConfigPATH)) {
                JsonFileLoader<ATM_SpawnConfig>.JsonLoadFile(ATM_SpawnConfigPATH, this);
            } else {
                ATMSpawns.Insert(new ATMData("ExpansionATM_1", "Location Name Here", "10 20 30", "10 20 30"));
                ATMSpawns.Insert(new ATMData("ExpansionATM_2", "Location Name Here", "10 20 30", "10 20 30"));
                ATMSpawns.Insert(new ATMData("ExpansionATM_3", "Location Name Here", "10 20 30", "10 20 30"));
                Save();
            }
        }
    }

    void Save() {
        JsonFileLoader<ATM_SpawnConfig>.JsonSaveFile(ATM_SpawnConfigPATH, this);
    }
};

ref ATM_SpawnConfig m_ATM_SpawnConfig;

static ATM_SpawnConfig GetATM_SpawnConfig() {
    if (!m_ATM_SpawnConfig) {
        m_ATM_SpawnConfig = new ATM_SpawnConfig;
        m_ATM_SpawnConfig.Load();
    }
    return m_ATM_SpawnConfig;
}
