#ifdef SERVER
modded class ExpansionActionRaidATM
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		super.OnFinishProgressServer(action_data);

		ExpansionATMBase expAtm;
		ItemBase key = ItemBase.Cast(action_data.m_MainItem);

		if ( Class.CastTo(expAtm, action_data.m_Target.GetObject()) )
			expAtm.StartRaidingEvent(action_data.m_Player, key);

		key.DeleteSafe();
	}
};
#endif