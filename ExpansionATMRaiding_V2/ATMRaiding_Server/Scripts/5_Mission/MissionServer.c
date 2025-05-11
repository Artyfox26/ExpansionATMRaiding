#ifdef SERVER
modded class MissionServer
{
	static ref SpawnATMSettings m_ATMSpawnSettings;

	void MissionServer()
	{
		m_ATMSpawnSettings = SpawnATMSettings.Load();
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
};
#endif