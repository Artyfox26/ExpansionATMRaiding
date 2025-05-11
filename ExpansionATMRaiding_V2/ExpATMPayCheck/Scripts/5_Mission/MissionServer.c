modded class MissionServer
{
    protected ref PaycheckConfig m_PaycheckConfig;
    protected string logFilePath;  // Store the log file path
    protected int currentPayoutInterval = 0;  // Stores the currently active payout interval

    void MissionServer()
    {
        Print("[Paycheck] Initializing paycheck system...");

        LoadPaycheckConfig();  // ✅ Ensure config is loaded before use

        if (!m_PaycheckConfig)
        {
            Print("[Paycheck] ERROR: Failed to load config! Disabling paycheck system.");
            return;
        }

        CreateLogFile();

        if (m_PaycheckConfig.EnablePaycheckSystem == 1)
        {
            SetPaycheckInterval(m_PaycheckConfig.PaycheckPayoutTime);
        }

        // ✅ Set config update check interval
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckForConfigUpdates, m_PaycheckConfig.ConfigUpdateRate * 60 * 1000, true);
    }

void LoadPaycheckConfig()
{
    string configPath = "$profile:zombicide_mods\\PayCheckSystem\\Paychecksystem.json";

    // ✅ Ensure the config directory exists
    if (!FileExist("$profile:zombicide_mods\\PayCheckSystem"))
    {
        MakeDirectory("$profile:zombicide_mods\\PayCheckSystem");
    }

    bool configExists = FileExist(configPath);

    // ✅ Load the existing config if it exists, otherwise create a new one
    if (configExists)
    {
        JsonFileLoader<PaycheckConfig>.JsonLoadFile(configPath, m_PaycheckConfig);
        Print("[Paycheck] Loaded existing config.");
    }
    else
    {
        Print("[Paycheck] Config file not found! Creating a default config...");

        // ✅ Create a new config with default values
        m_PaycheckConfig = new PaycheckConfig();
        m_PaycheckConfig.Version = 5;  // ✅ Ensure the correct version
        m_PaycheckConfig.EnablePaycheckSystem = 1;
        m_PaycheckConfig.PaycheckAmount = 1250;
        m_PaycheckConfig.PaycheckPayoutTime = 45;
        m_PaycheckConfig.ConfigUpdateRate = 1;
        m_PaycheckConfig.BonusPlayers = new array<ref PlayerBonus>;

        // ✅ Save the new default config to the file
        JsonFileLoader<PaycheckConfig>.JsonSaveFile(configPath, m_PaycheckConfig);
        Print("[Paycheck] Default config created at " + configPath);
    }

    // ✅ Ensure all players in the config have the required fields
    if (!m_PaycheckConfig.BonusPlayers)
    {
        m_PaycheckConfig.BonusPlayers = new array<ref PlayerBonus>;
    }
    foreach (PlayerBonus player : m_PaycheckConfig.BonusPlayers)
    {
        if (player.MultiplierTimerRemaining < 0)
        {
            player.MultiplierTimerRemaining = 0; // ✅ Ensure this exists
        }
        if (player.BonusTimerRemaining < 0)
        {
            player.BonusTimerRemaining = 0; // ✅ Ensure this exists
        }
        if (player.Multiplier < 1)  
        {
            player.Multiplier = 1; // ✅ Ensure multiplier is at least 1
        }
    }

    // ✅ Save updated values back to the config file
    JsonFileLoader<PaycheckConfig>.JsonSaveFile(configPath, m_PaycheckConfig);
    Print("[Paycheck] Config file updated and saved.");
}

void SetPaycheckInterval(int newInterval)
{
    int payoutInterval = newInterval * 60 * 1000; // Convert minutes to milliseconds

    // ✅ Remove any existing payout interval to ensure no duplicates
    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(AddMoneyToActivePlayers);

    // ✅ Apply the new payout interval immediately
    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AddMoneyToActivePlayers, payoutInterval, true);
    currentPayoutInterval = payoutInterval;

    Print("[Paycheck] Updated payout interval to: " + newInterval + " minutes.");
    LogPaycheck("[Paycheck] Updated payout interval to " + newInterval + " minutes.");
}

void CheckForConfigUpdates()
{
    Print("[Paycheck] Checking for config updates...");
    
    int oldPayoutTime = m_PaycheckConfig.PaycheckPayoutTime;
    int oldConfigUpdateRate = m_PaycheckConfig.ConfigUpdateRate;

    LoadPaycheckConfig();  // ✅ Reload the config file

    bool changesDetected = false;
    string changeLog = "[Paycheck] Changes Detected: ";

    // ✅ Check for PaycheckPayoutTime changes
    if (m_PaycheckConfig.PaycheckPayoutTime != oldPayoutTime)
    {
        SetPaycheckInterval(m_PaycheckConfig.PaycheckPayoutTime);  // ✅ Ensure it updates every time
        changeLog += "PaycheckPayoutTime changed from " + oldPayoutTime + " to " + m_PaycheckConfig.PaycheckPayoutTime + ", ";
        changesDetected = true;
    }

    // ✅ Check for ConfigUpdateRate changes
    if (m_PaycheckConfig.ConfigUpdateRate != oldConfigUpdateRate)
    {
        changeLog += "ConfigUpdateRate changed from " + oldConfigUpdateRate + " to " + m_PaycheckConfig.ConfigUpdateRate + ", ";
        changesDetected = true;
    }

    // ✅ Decrease `BonusTimerRemaining` and `MultiplierTimerRemaining` for all players
	foreach (PlayerBonus bonusData : m_PaycheckConfig.BonusPlayers)
	{
		// ✅ Update Bonus Timer
		if (bonusData.BonusTimerRemaining > 0)
		{
			int oldBonusTimer = bonusData.BonusTimerRemaining;
			bonusData.BonusTimerRemaining -= m_PaycheckConfig.ConfigUpdateRate;

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

		// ✅ Update Multiplier Timer
		if (bonusData.MultiplierTimerRemaining > 0)
		{
			int oldMultiplierTimer = bonusData.MultiplierTimerRemaining;
			bonusData.MultiplierTimerRemaining -= m_PaycheckConfig.ConfigUpdateRate;

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

	// ✅ Save updated config if changes were made
	if (changesDetected)
	{
		string configPath = "$profile:zombicide_mods\\PayCheckSystem\\Paychecksystem.json";
		JsonFileLoader<PaycheckConfig>.JsonSaveFile(configPath, m_PaycheckConfig);

		array<string> logEntries = new array<string>();  
		changeLog.Split(", ", logEntries);  // ✅ Corrected split usage

		foreach (string entry : logEntries)
		{
			if (entry != "")
			{
				string formattedLog = "[Paycheck] Changes Detected: " + entry;
				LogPaycheck(formattedLog);
				Print(formattedLog);
			}
		}

		LogPaycheck("[Paycheck] Changes Detected: Config Saved and Updated.");
		Print("[Paycheck] Changes Detected: Config Saved and Updated.");
	}
	else
	{
		string timeString = GetTimeString();
		LogPaycheck("[Paycheck] Time: " + timeString + " Config Up to date.");
	}
}

void CreateLogFile()
{
    string logFolder = "$profile:zombicide_mods\\PayCheckSystem\\Logs";
    if (!FileExist(logFolder))
    {
        MakeDirectory(logFolder);
    }

    logFilePath = logFolder + "\\PayCheckLog_" + GetTimestamp() + ".txt";
    
    FileHandle file = OpenFile(logFilePath, FileMode.WRITE);
    if (file != 0)
    {
        FPrintln(file, "[Paycheck] Log file created: " + GetTimestamp());
        CloseFile(file);
    }
}  // ✅ Ensure this properly closes

string GetTimestamp()
{
    int year, month, day, hour, minute, second;
    GetYearMonthDay(year, month, day);
    GetHourMinuteSecond(hour, minute, second);
    return year.ToString() + month.ToStringLen(2) + day.ToStringLen(2) + "_" + hour.ToStringLen(2) + minute.ToStringLen(2) + second.ToStringLen(2);
}

void LogPaycheck(string logMessage)
{
    FileHandle file = OpenFile(logFilePath, FileMode.APPEND);
    if (file != 0)
    {
        FPrintln(file, "[" + GetTimeString() + "] " + logMessage);
        CloseFile(file);
    }
}

void AddMoneyToActivePlayers()
{
    Print("[Paycheck] Checking config before payout...");
    LoadPaycheckConfig(); // Ensure we have the latest config

    if (!m_PaycheckConfig || !m_PaycheckConfig.EnablePaycheckSystem)
    {
        Print("[Paycheck] Paycheck system is disabled or config is missing. Skipping payout.");
        return;
    }

    array<Man> players = new array<Man>;
    GetGame().GetPlayers(players);

    ExpansionMarketModule marketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
    if (!marketModule)
    {
        Print("[Paycheck] Market module not found!");
        return;
    }

    string timeString = GetTimeString();
    bool configUpdated = false; // Track if new players were added
    string configPath = "$profile:zombicide_mods\\PayCheckSystem\\Paychecksystem.json";

    foreach (Man player : players)
    {
        if (!player.GetIdentity())
        {
            Print("[Paycheck] ERROR: Player identity not found for player object: " + player.ToString());
            continue;
        }

        string expansionID = player.GetIdentity().GetId();
        string playerName = player.GetIdentity().GetName();

        Print("[Paycheck] Processing player: Name=" + playerName + ", ID=" + expansionID);

        // Check if player already exists in BonusPlayers
        PlayerBonus existingBonus = PlayerBonus.FindBonusForPlayer(expansionID, m_PaycheckConfig.BonusPlayers);
        if (!existingBonus)
        {
            Print("[Paycheck] Adding new player to BonusPlayers: " + playerName + " (" + expansionID + ")");
            existingBonus = new PlayerBonus(expansionID, playerName, "New Player", 1, 0, 0, 0, GetGame().GetTime() / 1000);
            m_PaycheckConfig.BonusPlayers.Insert(existingBonus);
            configUpdated = true;
        }

        // Apply Bonus & Multiplier
        int basePaycheck = m_PaycheckConfig.PaycheckAmount;
        int finalAmount = (basePaycheck * existingBonus.Multiplier) + existingBonus.Bonus;

        ExpansionMarketATM_Data playerATMData = marketModule.GetPlayerATMData(expansionID);
        if (playerATMData)
        {
            playerATMData.AddMoney(finalAmount);
            ExpansionNotification(new StringLocaliser("Payment"), new StringLocaliser("You have received a payout of " + finalAmount.ToString() + " to your Bank account."), ExpansionIcons.GetPath("Exclamationmark_64")).Create();

            string logEntry = "[Paycheck] Time:" + timeString + " IngameName: " + playerName;
            logEntry += " | ExpansionID: " + expansionID;
            logEntry += " | Final Amount: " + finalAmount.ToString();
            logEntry += " | BonusTimerRemaining: " + existingBonus.BonusTimerRemaining + " mins";

            LogPaycheck(logEntry);
        }
    }

    // Save updated config if needed
    if (configUpdated)
    {
        Print("[Paycheck] Saving updated config with new players...");
        JsonFileLoader<PaycheckConfig>.JsonSaveFile(configPath, m_PaycheckConfig);
        Print("[Paycheck] Config successfully saved.");
    }
} // ✅ Closes function properly

string GetTimeString()
{
    int hour, minute, second;
    GetHourMinuteSecond(hour, minute, second);
    return hour.ToStringLen(2) + ":" + minute.ToStringLen(2);
}
};

class PaycheckConfig
{
    int Version = 5;
    int EnablePaycheckSystem = 1;
    int PaycheckAmount = 1250;
    int PaycheckPayoutTime = 45;
    int ConfigUpdateRate = 1;
    ref array<ref PlayerBonus> BonusPlayers;

    void PaycheckConfig()
    {
        if (!BonusPlayers)
        {
            BonusPlayers = new array<ref PlayerBonus>;
        }
    }
}

class PlayerBonus
{
    string SteamID;
    string NameForRef;
    string ReasonRef;
    int Multiplier;
    int MultiplierTimerRemaining;  // ✅ New Timer for Multiplier
    int Bonus;
    int BonusTimerRemaining;
    int LastBonusUpdate;

    void PlayerBonus(string steamID, string name, string reason, int multiplier, int multiplierTimer, int bonus, int bonusTimer, int lastUpdate)
    {
        SteamID = steamID;
        NameForRef = name;
        ReasonRef = reason;
        Multiplier = multiplier;
        MultiplierTimerRemaining = multiplierTimer;
        Bonus = bonus;
        BonusTimerRemaining = bonusTimer;
        LastBonusUpdate = lastUpdate;
    }
	
	// ✅ Add this function inside `PlayerBonus`
	static PlayerBonus FindBonusForPlayer(string steamID, array<ref PlayerBonus> bonusList)
	{
		foreach (PlayerBonus bonusData : bonusList)
		{
			if (bonusData.SteamID == steamID)
			{
				return bonusData;
			}
		}
		return null;
	}
}

// ✅ Update a player's multiplier, bonus, and both timers
static void UpdateBonusForPlayer(PlayerBonus existingBonus, int newMultiplier, int multiplierTimer, int newBonus, int bonusTimer)
{
    existingBonus.Multiplier = newMultiplier;
    existingBonus.MultiplierTimerRemaining = multiplierTimer;  // ✅ Now updates Multiplier Timer
    existingBonus.Bonus = newBonus;
    existingBonus.BonusTimerRemaining = bonusTimer;
    existingBonus.LastBonusUpdate = GetGame().GetTime() / 1000;  // ✅ Store last update in seconds
}

