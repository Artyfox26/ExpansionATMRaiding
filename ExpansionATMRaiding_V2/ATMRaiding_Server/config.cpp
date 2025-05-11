////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 9.75
//https://mikero.bytex.digital/Downloads
//'now' is Sun May 11 12:43:14 2025 : 'file' last modified on Tue Nov 12 20:20:23 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class ATMRaiding_Server
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"ATMRaiding_Client"};
		units[] = {};
		weapons[] = {};
	};
};
class CfgMods
{
	class ATMRaiding_Server
	{
		dir = "ATMRaiding_Server";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ATMRaiding_Server";
		credits = "LieutenantMaster";
		author = "LieutenantMaster";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"ATMRaiding_Server/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"ATMRaiding_Server/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"ATMRaiding_Server/Scripts/5_Mission"};
			};
		};
	};
};
