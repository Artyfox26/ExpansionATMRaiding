class CfgPatches
{
	class ExpATMPayCheck
	{
		requiredVersion = 0.1;
		requiredAddons[] = {};
		units[] = {};
		weapons[] = {};
	};
};

class CfgMods
{
	class ExpATMPayCheck
	{
		dir = "ExpATMPayCheck";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ExpATMPayCheck";
		credits = "LieutenantMaster";
		author = "LieutenantMaster, Artyfox";
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
					"ExpATMPayCheck/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"ExpATMPayCheck/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"ExpATMPayCheck/Scripts/5_Mission"
				};
			};
		};
	};
};
