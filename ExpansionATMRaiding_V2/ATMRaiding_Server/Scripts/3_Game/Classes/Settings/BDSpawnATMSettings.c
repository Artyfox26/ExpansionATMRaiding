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
		if (!FileExist(ATM_FOLDER_PROFILE))
			MakeDirectory(ATM_FOLDER_PROFILE);
		
		SpawnATMSettings m_Settings = new SpawnATMSettings();
		if (FileExist(ATM_FILE_SPAWNSETTINGS))
		{
			JsonFileLoader<SpawnATMSettings>.JsonLoadFile(ATM_FILE_SPAWNSETTINGS, m_Settings);
		}
		else
		{
			m_Settings.Defaults();
			JsonFileLoader<SpawnATMSettings>.JsonSaveFile(ATM_FILE_SPAWNSETTINGS, m_Settings);
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
