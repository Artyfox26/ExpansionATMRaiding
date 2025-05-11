class RaidATMSettings
{
    string MONEY_CLASSNAME;
    int MONEY_AMOUNT_LOW_MIN;
    int MONEY_AMOUNT_LOW_MAX;
    int MONEY_AMOUNT_HIGH_MIN;
    int MONEY_AMOUNT_HIGH_MAX;
    int PLAYER_POPULATION_LOW;
    int ATM_TIME_DISABLED;
    int ATM_ZONE_RADIUS;
    int ATM_HOLD_TIME;

	ref ATMInfected Enemies;
	
	ref map<string, int> ToolsRaidTime;
    ref array<ref HackableItem> HackableItems;
#ifdef PVEZ
	ref static ATMPVEZ PVEZ;
#endif

	static ref RaidATMSettings Load()
	{
        // Use new config path from ConfigATMRaiding.c
        string configPath = "$profile:\ExpansionMod\\ATM_Raiding\\ATM_RaidingConfig.json";
        RaidATMSettings m_Settings = new RaidATMSettings();
        if (FileExist(configPath))
        {
            JsonFileLoader<RaidATMSettings>.JsonLoadFile(configPath, m_Settings);
        }
        else
        {
            m_Settings.Default();
            JsonFileLoader<RaidATMSettings>.JsonSaveFile(configPath, m_Settings);
        }
        return m_Settings;
	}

	void Default()
	{
		MONEY_CLASSNAME    	= "ExpansionBanknoteHryvnia";
		MONEY_AMOUNT_LOW_MIN    = 64564;
		MONEY_AMOUNT_LOW_MAX    = 158551;
		MONEY_AMOUNT_HIGH_MIN   = 169765;
		MONEY_AMOUNT_HIGH_MAX   = 814193;
		PLAYER_POPULATION_LOW 	= 10;
		ATM_TIME_DISABLED     	= 5400000;
		ATM_ZONE_RADIUS     	= 50;
		ATM_HOLD_TIME     		= 900;

		Enemies = new ATMInfected(-4, 60, {"eAI_SurvivorM_Mirek|faction:Mercenaries,loadout:AIGuards_T4_FOG_Loadout,canbelooted:True,UnlimitedReload:True"});
		
		ToolsRaidTime = new map<string, int>;
		ToolsRaidTime.Insert("Crowbar", 10 * 60);
		ToolsRaidTime.Insert("Screwdriver", 15 * 60);
		ToolsRaidTime.Insert("DecoderTablet", 8 * 60); // Add DecoderTablet as a valid hacking tool

        // Add default hackable items for the new hacking tablet
        HackableItems = new array<ref HackableItem>;
        HackableItems.Insert(new HackableItem("ExpansionATM_1", 60, 900, 2));
        HackableItems.Insert(new HackableItem("ExpansionATM_2", 90, 1200, 3));
        HackableItems.Insert(new HackableItem("ExpansionATM_3", 120, 1800, 5));
		
	#ifdef PVEZ
		PVEZ = new ATMPVEZ("ATM Raiding", 150, true, true);
	#endif
	}
};
