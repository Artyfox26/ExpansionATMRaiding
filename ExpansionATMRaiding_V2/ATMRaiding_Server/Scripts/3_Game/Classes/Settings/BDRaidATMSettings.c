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
#ifdef PVEZ
	ref static ATMPVEZ PVEZ;
#endif

	static ref RaidATMSettings Load()
	{
		if (!FileExist(ATM_FOLDER_PROFILE))
			MakeDirectory(ATM_FOLDER_PROFILE);
		
		RaidATMSettings m_Settings = new RaidATMSettings();
		if (FileExist(ATM_FILE_HACKSETTINGS))
		{
			JsonFileLoader<RaidATMSettings>.JsonLoadFile(ATM_FILE_HACKSETTINGS, m_Settings);
		}
		else
		{
			m_Settings.Default();
			JsonFileLoader<RaidATMSettings>.JsonSaveFile(ATM_FILE_HACKSETTINGS, m_Settings);
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

		Enemies = new ATMInfected(-6, 60, {"eAI_SurvivorM_Mirek|faction:Mercenaries,loadout:AIGuards_T4_FOG_Loadout,canbelooted:True,UnlimitedReload:True"});
		
		ToolsRaidTime = new map<string, int>;
		ToolsRaidTime.Insert("Crowbar", 10 * 60);
		ToolsRaidTime.Insert("Screwdriver", 15 * 60);
	#ifdef PVEZ
		PVEZ = new ATMPVEZ("ATM Raiding", 150, true, true);
	#endif
	}
};
