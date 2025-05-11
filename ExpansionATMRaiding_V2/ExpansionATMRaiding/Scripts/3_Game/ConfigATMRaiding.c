class ATM_RaidingConfig
{
    private static string ATM_RaidingConfigPATH = "$profile:\\ExpansionMod\\ATM_Raiding\\ATM_RaidingConfig.json";
    // Default Values
    bool EnableATMSystem = true;
    int ATMZoneRadius = 50;
    int ATMHoldTime = 900;
    int ATMTimeDisabled = 5400000;
    int PlayerPopulationLow = 10;
    string MoneyClassname = "ExpansionBanknoteHryvnia";
    int MoneyAmountLowMin = 64564;
    int MoneyAmountLowMax = 158551;
    int MoneyAmountHighMin = 169765;
    int MoneyAmountHighMax = 814193;
    ref map<string, int> ToolsRaidTime = new map<string, int>;
    ref array<ref ATMData> ATMSpawns = new array<ref ATMData>;

    void Load() {
        if (GetGame().IsServer()) {
            if (FileExist(ATM_RaidingConfigPATH)) {
                JsonFileLoader<ATM_RaidingConfig>.JsonLoadFile(ATM_RaidingConfigPATH, this);
            } else {
                ToolsRaidTime.Insert("Crowbar", 10 * 60);
                ToolsRaidTime.Insert("Screwdriver", 15 * 60);
                ATMSpawns.Insert(new ATMData("ExpansionATM_1", "Location Name Here", "10 20 30", "10 20 30"));
                ATMSpawns.Insert(new ATMData("ExpansionATM_2", "Location Name Here", "10 20 30", "10 20 30"));
                ATMSpawns.Insert(new ATMData("ExpansionATM_3", "Location Name Here", "10 20 30", "10 20 30"));
                Save();
            }
        }
    }

    void Save() {
        JsonFileLoader<ATM_RaidingConfig>.JsonSaveFile(ATM_RaidingConfigPATH, this);
    }
};

ref ATM_RaidingConfig m_ATM_RaidingConfig;

static ATM_RaidingConfig GetATM_RaidingConfig() {
    if (!m_ATM_RaidingConfig) {
        m_ATM_RaidingConfig = new ATM_RaidingConfig;
        m_ATM_RaidingConfig.Load();
    }
    return m_ATM_RaidingConfig;
}
