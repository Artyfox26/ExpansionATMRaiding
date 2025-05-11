////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 9.75
//https://mikero.bytex.digital/Downloads
//'now' is Sun May 11 12:43:33 2025 : 'file' last modified on Fri Jun 07 14:21:44 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class ATMRaiding_Client
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {};
		weapons[] = {};
	};
};
class CfgMods
{
	class ATMRaiding_Client
	{
		dir = "ATMRaiding_Client";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ATMRaiding_Client";
		credits = "LieutenantMaster";
		author = "LieutenantMaster";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"World"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"ATMRaiding_Client/Scripts/4_World"};
			};
		};
	};
};
