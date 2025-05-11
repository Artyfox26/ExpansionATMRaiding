modded class MissionServer
{
    static ref SpawnATMSettings m_ATMSpawnSettings;
    protected int currentPayoutInterval = 0;  // Stores the currently active payout interval

    override void OnInit()
    {
        super.OnInit();
        Print("[ExpansionATMRaiding] OnInit");
        GetExpansionATMRaidingConfig();
        GetRPCManager().AddRPC( "HACK", "RPCExpansionATMRaidingSettings", this, SingeplayerExecutionType.Both );
    }
    
    void RPCExpansionATMRaidingSettings( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        PlayerIdentity RequestedBy = PlayerIdentity.Cast(sender);
        if (RequestedBy){
            GetRPCManager().SendRPC("HACK", "RPCExpansionATMRaidingSettings", new Param1< ExpansionATMRaidingConfig >( GetExpansionATMRaidingConfig() ), true, RequestedBy);
        }
    }

    void MissionServer()
    {
        m_ATMSpawnSettings = SpawnATMSettings.Load();

        Print("[Paycheck] Initializing paycheck system...");

        GetPaycheckSystemConfig();

        if (!GetPaycheckSystemConfig() || !GetPaycheckSystemConfig().EnablePaycheckSystem)
        {
            Print("[Paycheck] ERROR: Failed to load config! Disabling paycheck system.");
            return;
        }

        GetPaycheckLogger(); // Initialize logger

        if (GetPaycheckSystemConfig().EnablePaycheckSystem == 1)
        {
            GetPaycheckSystemConfig().SetPaycheckInterval(GetPaycheckSystemConfig().PaycheckPayoutTime);
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(
            GetPaycheckSystemConfig().CheckForConfigUpdates,
            GetPaycheckSystemConfig().ConfigUpdateRate * 60 * 1000, true);
    }

    override void OnMissionLoaded()
    {
        super.OnMissionLoaded();
        
        if ( m_ATMSpawnSettings )
            SpawnRaidableATMs();
    }

    void SpawnRaidableATMs()
    {
        if ( !m_ATMSpawnSettings.Enable )
            return;
        
        for(int i=0; i < m_ATMSpawnSettings.ATM.Count(); i++)
        {
            ExpansionATMBase objectATM = ExpansionATMBase.Cast( GetGame().CreateObject( m_ATMSpawnSettings.ATM[i].Classname, m_ATMSpawnSettings.ATM[i].Position, false ) );
            objectATM.SetOrientation(m_ATMSpawnSettings.ATM[i].Rotation);
            objectATM.m_LocationName = m_ATMSpawnSettings.ATM[i].LocationName;
        }
    }

    void SetPaycheckInterval(int newInterval)
    {
        // Deprecated: Use GetPaycheckSystemConfig().SetPaycheckInterval instead
    }

    void CheckForConfigUpdates()
    {
        // Deprecated: Use GetPaycheckSystemConfig().CheckForConfigUpdates instead
    }

    void CreateLogFile()
    {
        // Deprecated: Use GetPaycheckLogger().CreateLogFile instead
    }

    string GetTimestamp()
    {
        // Deprecated: Use GetPaycheckLogger().GetTimestamp instead
        return GetPaycheckLogger().GetTimestamp();
    }

    void LogPaycheck(string logMessage)
    {
        // Deprecated: Use GetPaycheckLogger().LogPaycheck instead
    }

    string GetTimeString()
    {
        // Deprecated: Use GetPaycheckLogger().GetTimeString instead
        return GetPaycheckLogger().GetTimeString();
    }

    void AddMoneyToActivePlayers()
    {
        // Deprecated: Use GetPaycheckSystemConfig().AddMoneyToActivePlayers instead
    }
}

