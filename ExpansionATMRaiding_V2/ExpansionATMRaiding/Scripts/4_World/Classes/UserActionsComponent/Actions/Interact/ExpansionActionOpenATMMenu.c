modded class ExpansionActionOpenATMMenu
{
	private int m_IsRaided;

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;
		
		ExpansionATMBase expAtm;
		if ( Class.CastTo(expAtm, target.GetObject()) )
		{
			m_IsRaided = expAtm.m_IsRaided;
			
			switch(m_IsRaided)
			{
				case -1:
					m_Text = "Hacking in progress..";
				break;
				case 1:
					m_Text = "ATM is Broken";
				break;
			}
		}

		return true;
	}
};
