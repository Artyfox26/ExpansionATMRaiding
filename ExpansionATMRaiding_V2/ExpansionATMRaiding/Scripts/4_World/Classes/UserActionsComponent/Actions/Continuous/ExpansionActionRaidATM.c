class ExpansionActionRaidATMCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(10);
	}
};

class ExpansionActionRaidATM: ActionContinuousBase
{
	void ExpansionActionRaidATM()
	{
		m_CallbackClass 	= ExpansionActionRaidATMCB;
		m_SpecialtyWeight 	= UASoftSkillsWeight.PRECISE_HIGH;
        m_CommandUID 		= DayZPlayerConstants.CMD_ACTIONMOD_GIVER;
	}
	
	override void CreateConditionComponents()
	{
        m_ConditionItem = new CCINonRuined();
        m_ConditionTarget = new CCTNone();
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if ( !super.ActionCondition(player, target, item) )
            return false;

		ExpansionATMBase expAtm;
		if ( Class.CastTo(expAtm, target.GetObject()) )
		{
			if ( player.Expansion_IsInSafeZone() )
				return false;

			if (!GetExpansionSettings().GetMarket(false).IsLoaded())
				return false;

			if (!GetExpansionSettings().GetMarket().ATMSystemEnabled)
				return false;
			
			switch(expAtm.m_IsRaided)
			{
				case -1:
					return false;
				break;
				case 0:
					m_Text = "Hack ATM";
				break;
				case 1:
					m_Text = "Electronics seems to be already fried";
					#ifdef SERVER
						return false;
					#endif
				break;
			}
			return true;
		}
		
		return false;
	}
	
	override void OnFinishProgressClient(ActionData action_data)
	{
		Object target_object = action_data.m_Target.GetObject();
		if(target_object)
		{
			EffectSound sound = SEffectManager.PlaySound( "spotlight_turn_on_SoundSet", target_object.GetPosition() );
			sound.SetSoundAutodestroy( true );
		}
	}
};

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