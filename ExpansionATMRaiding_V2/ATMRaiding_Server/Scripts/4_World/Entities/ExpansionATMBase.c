#ifdef SERVER
modded class ExpansionATMBase
{
	static ref RaidATMSettings Settings;

#ifdef EXPANSIONMODNAVIGATION
	private ExpansionMarkerModule m_MarkerModule;
	private ExpansionMarkerData m_Marker;
#endif

#ifdef PVEZ
	protected autoptr PVEZ_Zone pvez_Zone;
#endif

	ref Timer m_Timer;
	private int m_TimeSpent = 0;

	ref Timer m_AITimer;
	ref array<Object> m_Entities;
	
	private PlayerBase m_Player;

	private string m_HackingDevice;
	private float m_HackingDeviceHealth;
	private int m_HoldTime;

	string m_LocationName;

	void ExpansionATMBase()
	{
		m_Timer 	= new Timer();
		m_AITimer 	= new Timer();
		m_Entities 	= new array<Object>;
	}
	
	void ~ExpansionATMBase()
	{
		StopRaidingEvent(false);
		DeleteAI();
	}

	void StartRaidingEvent(PlayerBase player, ItemBase hackingDevice)
	{
		if (!Settings)
			Settings = RaidATMSettings.Load();

		m_Player = player;
		m_HackingDevice = hackingDevice.GetType();
		m_HackingDeviceHealth = hackingDevice.GetHealth();
		m_TimeSpent = 0;
		CreateMarker();
#ifdef PVEZ
		vector position = GetPosition();
		pvez_Zone = g_Game.pvez_Zones.AddZone(
			PVEZ_ZONE_TYPE_STATIC,
			position[0],
			position[2],
			RaidATMSettings.PVEZ.Radius,
			RaidATMSettings.PVEZ.DisplayName,
			RaidATMSettings.PVEZ.ShowBorderOnMap,
			RaidATMSettings.PVEZ.ShowNameOnMap);
#endif
		NotificationSystem.Create( new StringLocaliser( "ATM System" ), new StringLocaliser( "An ATM at "+ m_LocationName+" is getting hacked!" ), ExpansionIcons.GetPath("Coins"), COLOR_EXPANSION_NOTIFICATION_MISSION, 4.0, NULL );

        if ( m_Timer )
			m_Timer.Stop();

        if ( m_AITimer )
			m_AITimer.Stop();

		if ( Settings.ToolsRaidTime.Contains(m_HackingDevice) )
			m_HoldTime = Settings.ToolsRaidTime.Get(m_HackingDevice);
		else
			m_HoldTime = Settings.ATM_HOLD_TIME;

		m_Timer.Run(1, this, "OnRaidingEvent", NULL, true);
		int minTime = m_HoldTime - 60;
		if ( minTime < 0 )
			minTime = 0;
		
		m_AITimer.Run(Math.RandomIntInclusive(minTime, m_HoldTime), this, "SpawnAI", NULL, false);

		ToggleRaidState(-1);
	}

	void OnRaidingEvent()
	{
		if ( !IsPlayerInZone() )
		{
			StopRaidingEvent(false);

			if ( m_Player )
			{
				ItemBase hackItem = ItemBase.Cast(m_Player.GetInventory().CreateInInventory(m_HackingDevice));
				hackItem.SetHealth("", "", m_HackingDeviceHealth);
			}

			ToggleRaidState(0);
			return;
		}

		m_TimeSpent++;
		if ( m_TimeSpent > m_HoldTime )
			StopRaidingEvent(true);
		else if ( m_TimeSpent == 1 )
		{
			NotificationSystem.Create( new StringLocaliser( "ATM System" ), new StringLocaliser( "Stay Within 50m of The ATM"), ExpansionIcons.GetPath("Coins"), COLOR_EXPANSION_NOTIFICATION_MISSION, 4.0, m_Player.GetIdentity() );
		}
		else if (m_TimeSpent % 90 == 0)
		{	
			int timeLeft = m_HoldTime - m_TimeSpent;
			int minutes = timeLeft / 60; // Calculate minutes
			int seconds = timeLeft % 60; // Calculate remaining seconds

			string timeMessage;

			if (minutes > 0)
			{
				timeMessage = minutes.ToString() + " minute";
				if (minutes > 1)
					timeMessage += "s";

				timeMessage += " and ";
			}

			timeMessage += seconds.ToString() + " second";
			if (seconds != 1)
				timeMessage += "s";

			NotificationSystem.Create(new StringLocaliser("ATM System"), new StringLocaliser("Stay Within 50m for " + timeMessage), ExpansionIcons.GetPath("Coins"), COLOR_EXPANSION_NOTIFICATION_MISSION, 4.0, m_Player.GetIdentity());
		}
	}

 void StopRaidingEvent(bool isCompleted)
    {
        DeleteMarker();
        
        if ( m_Timer )
            m_Timer.Stop();
        
        if ( m_AITimer )
            m_AITimer.Stop();

        if ( isCompleted )
            SpawnMoney();

        GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( DeleteAI, 600000, false );
    }

    void DeleteAI()
    {
#ifdef PVEZ
        if (g_Game && g_Game.pvez_Zones)
            g_Game.pvez_Zones.RemoveZone(pvez_Zone);
#endif

        if ( m_Entities )
        {
            foreach (Object selectedObject: m_Entities)
            {
                if (selectedObject)
                    GetGame().ObjectDelete(selectedObject);
            }
        }
    }

	bool IsPlayerInZone()
	{
		if ( !m_Player )
			return false;

		if ( vector.Distance(m_Player.GetPosition(), GetPosition()) < Settings.ATM_ZONE_RADIUS )
			return true;
		
		return false;
	}

	void CreateMarker()
	{
#ifdef EXPANSIONMODNAVIGATION
		if (CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule))
		{
			m_Marker = m_MarkerModule.CreateServerMarker("ATM Raid in Progress", ExpansionIcons.GetPath("Coins"), GetPosition(), ARGB(255, 0, 170, 255), true);
			m_Marker.SetVisibility(EXPANSION_MARKER_VIS_WORLD);
		}
#endif
	}

	void DeleteMarker()
	{
#ifdef EXPANSIONMODNAVIGATION
		if (m_Marker && m_MarkerModule)
			m_MarkerModule.RemoveServerMarker(m_Marker.GetUID());
#endif
	}

	void SpawnAI()
	{
		vector pos 				= GetPosition();
		int radius 				= Settings.Enemies.SpawnRadius;
		TStringArray classnames = Settings.Enemies.Entities;
		int spawncount 			= Settings.Enemies.Amount;
		if ( spawncount < 0 )
			spawncount = Math.RandomIntInclusive(1, Math.AbsInt(spawncount));

		string ent;
		vector spawnPos;
		array<Object> entities = new array<Object>;
		for (int i=0; i < spawncount; i++)
		{
			spawnPos 	= pos + Vector(Math.RandomFloatInclusive(radius,-radius), 0, Math.RandomFloatInclusive(radius,-radius));
			spawnPos[1] = GetGame().SurfaceY(spawnPos[0], spawnPos[2]);
			ent = classnames.GetRandomElement();

#ifdef EXPANSIONMODAI
			bool isAI = ent.IndexOf("eAI_Survivor") == 0;
			TStringArray parts();
			if (isAI)
			{
				ent.Split("|", parts);
				ent = parts[0];
				parts.Remove(0);
			}
#endif

			Object obj = GetGame().CreateObject( ent, spawnPos, false, GetGame().IsKindOf(ent, "DZ_LightAI") );

			if ( obj )
			{
#ifdef EXPANSIONMODAI
				DayZPlayerImplement ai;
				if (isAI && Class.CastTo(ai, obj) && parts.Count())
				{
					ExpansionObjectSpawnTools.ProcessGear(ai, parts[0]);
				}
#endif
				ZombieBase zombie;
				AnimalBase animal;
				if (Class.CastTo(zombie, obj))
					zombie.Expansion_OverrideAlertLevel(21.0);
				else if (Class.CastTo(animal, obj))
					animal.GetInputController().OverrideAlertLevel(true, true, 1, 31.0);  //! Should work for wolf/bear

				entities.Insert(obj);
			}
		}

		if ( m_Entities )
			DeleteAI();

		m_Entities = entities;
	}

	void SpawnMoney()
	{
		array<Man> m_Players = new array<Man>;
		GetGame().GetPlayers(m_Players);
		int playercount = m_Players.Count();
		int quant;

		if ( playercount > Settings.PLAYER_POPULATION_LOW )
			quant = Math.RandomIntInclusive(Settings.MONEY_AMOUNT_HIGH_MIN, Settings.MONEY_AMOUNT_HIGH_MAX);
		else
			quant = Math.RandomIntInclusive(Settings.MONEY_AMOUNT_LOW_MIN, Settings.MONEY_AMOUNT_LOW_MAX);

		while (quant > 0)
		{
			vector moneyPos = GetPosition() + -GetDirection() + Vector(Math.RandomFloat(-0.1, 0.1), 0, Math.RandomFloat(-0.1, 0.1)) * 1.125;
			ItemBase money = ItemBase.Cast(GetGame().CreateObjectEx(Settings.MONEY_CLASSNAME, moneyPos, ECE_PLACE_ON_SURFACE));
			if ( !money.HasQuantity() )
				break;

			if ( money.GetQuantityMax() < quant )
			{
				quant -= money.GetQuantityMax();
				money.SetQuantity(money.GetQuantityMax());
			}
			else
			{
				money.SetQuantity(quant);
				quant = 0;
			}
		}

		NotificationSystem.Create( new StringLocaliser( "ATM System" ), new StringLocaliser( "The ATM at "+ m_LocationName+" has been hacked!" ), ExpansionIcons.GetPath("Coins"), COLOR_EXPANSION_NOTIFICATION_MISSION, 4.0, NULL );
		
		ToggleRaidState(1);
        
        if ( Settings.ATM_TIME_DISABLED > 0 )
		    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ToggleRaidState, Settings.ATM_TIME_DISABLED, false, 0);
	}

	void ToggleRaidState(int state)
	{
		m_IsRaided = state;
        SetSynchDirty();
	}
};
#endif