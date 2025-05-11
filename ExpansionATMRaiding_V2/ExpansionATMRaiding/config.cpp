class CfgPatches
{
	class ExpansionATMRaiding
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {};
		weapons[] = {};
	};
};

class CfgMods
{
	class ExpansionATMRaiding
	{
		dir = "ExpansionATMRaiding";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ExpansionATMRaiding";
		credits = "LieutenantMaster & Artyfox";
		author = "LieutenantMaster & Artyfox";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[]= 
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"ExpansionATMRaiding/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"ExpansionATMRaiding/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"ExpansionATMRaiding/Scripts/5_Mission"
				};
			};
		};
	};
};
class CfgSlots
{
	class Slot_Att_TabletBattery_1
	{
		name = "Att_TabletBattery_1";
		displayName = "Tablet Battery";
		selection = "att_TabletBattery";
		ghostIcon = "batteryd";
	};
	class Slot_Att_TabletBattery_2
	{
		name = "Att_TabletBattery_2";
		displayName = "Tablet Battery";
		selection = "att_TabletBattery";
		ghostIcon = "batteryd";
	};
	class Slot_Att_TabletBattery_3
	{
		name = "Att_TabletBattery_3";
		displayName = "Tablet Battery";
		selection = "att_TabletBattery";
		ghostIcon = "batteryd";
	};
	class Slot_Att_TabletBattery_4
	{
		name = "Att_TabletBattery_4";
		displayName = "Tablet Battery";
		selection = "att_TabletBattery";
		ghostIcon = "batteryd";
	};
	class Slot_Att_TabletBattery_5
	{
		name = "Att_TabletBattery_5";
		displayName = "Tablet Battery";
		selection = "att_TabletBattery";
		ghostIcon = "batteryd";
	};
};
class CfgVehicles
{
	class Container_Base;
	class CombinationLock;
	class Inventory_Base;
	class Battery9V;
	class DecoderTablet: Inventory_Base
	{
		scope = 2;
		displayName = "Decoder Tablet";
		descriptionShort = "A Tablet Used for hacking into buildings or tents protected by a code lock";
		model = "ExpansionATMRaiding\Data\DecoderTablet.p3d";
		itemSize[] = {4,3};
		weight = 800;
		rotationFlags = 1;
		attachments[] = {"Att_TabletBattery_1","Att_TabletBattery_2","Att_TabletBattery_3","Att_TabletBattery_4","Att_TabletBattery_5"};
		hiddenSelections[] = {"zbytek","tablet_on","tablef_off","screen","background","backscreen","top"};
		hiddenSelectionsTextures[] = {"ExpansionATMRaiding\Data\textures\Tablet_off_co.paa","ExpansionATMRaiding\Data\textures\Tablet_on_co.paa","ExpansionATMRaiding\Data\textures\Tablet_off_co.paa","ExpansionATMRaiding\Data\textures\Tablet_off_co.paa","ExpansionATMRaiding\Data\textures\Tablet_off_co.paa","ExpansionATMRaiding\Data\textures\Tablet_off_co.paa","ExpansionATMRaiding\Data\textures\Tablet_off_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"ExpansionATMRaiding\Data\textures\Tablet_on.rvmat"}},{0.7,{"ExpansionATMRaiding\Data\textures\Tablet_on.rvmat"}},{0.5,{"ExpansionATMRaiding\Data\textures\Tablet_damage.rvmat"}},{0.3,{"ExpansionATMRaiding\Data\textures\Tablet_damage.rvmat"}},{0,{"ExpansionATMRaiding\Data\textures\Tablet_destruct.rvmat"}}};
				};
			};
		};
		class AnimationSources
		{
			class top
			{
				source = "user";
				animPeriod = 1;
				initPhase = 0;
			};
		};
	};
	class TabletBattery: Battery9V
	{
		scope = 2;
		displayName = "Tablet Battery";
		descriptionShort = "A Battery to put in tablets";
		model = "ExpansionATMRaiding\Data\TabletBattery.p3d";
		itemSize[] = {1,1};
		weight = 100;
		rotationFlags = 1;
		inventorySlot[] = {"Att_TabletBattery_1","Att_TabletBattery_2","Att_TabletBattery_3","Att_TabletBattery_4","Att_TabletBattery_5"};
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"ExpansionATMRaiding\Data\textures\Battery_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] = {{1,{"ExpansionATMRaiding\Data\textures\Battery.rvmat"}},{0.7,{"ExpansionATMRaiding\Data\textures\Battery.rvmat"}},{0.5,{"ExpansionATMRaiding\Data\textures\Battery.rvmat"}},{0.3,{"ExpansionATMRaiding\Data\textures\Battery.rvmat"}},{0,{"ExpansionATMRaiding\Data\textures\Battery_Ruined.rvmat"}}};
				};
			};
		};
	};
};
