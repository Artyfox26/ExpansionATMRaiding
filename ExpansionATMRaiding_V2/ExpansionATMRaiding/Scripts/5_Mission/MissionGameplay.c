modded class MissionGameplay extends MissionBase
{

	protected ref array<ref ExpansionATMRaidingNotificationWidget> HackingNotifications = new array<ref ExpansionATMRaidingNotificationWidget>;
	
	void MissionGameplay(){
		GetRPCManager().AddRPC( "HACK", "RPCExpansionATMRaidingSettings", this, SingeplayerExecutionType.Both );
		GetRPCManager().AddRPC( "HACK", "RPCExpansionATMRaidingNotifcation", this, SingeplayerExecutionType.Both );
	}
	
	override void OnMissionStart(){
		super.OnMissionStart();
		Print("[ExpansionATMRaiding][Client] Requesting Settings From Server");
		GetRPCManager().SendRPC("HACK", "RPCExpansionATMRaidingSettings", new Param1< ExpansionATMRaidingConfig >( NULL ), true, NULL);
	}
	
	
	void RPCExpansionATMRaidingSettings( CallType type,  ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< ExpansionATMRaidingConfig > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		m_ExpansionATMRaidingConfig = data.param1;
		Print("[ExpansionATMRaiding][Client] Settings Received");
		//m_ExpansionATMRaidingConfig.HackableItems.Debug();
	}
	
	void RPCExpansionATMRaidingNotifcation( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param3< string, string, float > data  //Player ID, Icon
		if ( !ctx.Read( data ) ) return;
		string Heading = data.param1;
		string Message = data.param2;
		float MessageTime = data.param3;
		if (GetExpansionATMRaidingConfig().Notification == 1) {
			GetGame().Chat( Message, "colorAction");
		} else if (GetExpansionATMRaidingConfig().Notification == 2) {
			int LastIndex = HackingNotifications.Count() - 1;
			if (HackingNotifications.Count() > 1) {
				HackingNotifications.Get(LastIndex).EarlyProccess();
			}
			int nextIndex = HackingNotifications.Count();
			HackingNotifications.Insert(new ExpansionATMRaidingNotificationWidget());
			HackingNotifications.Get(nextIndex).Init(Heading, Message, "ExpansionATMRaiding/GUI/Images/hacking.paa", MessageTime);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.DeleteNotification, MessageTime * 1500, false);
		}
	}
		
	void DeleteNotification(){
		if ( HackingNotifications.Count() > 0 ){
			HackingNotifications.RemoveOrdered(0);
		}
	}
}