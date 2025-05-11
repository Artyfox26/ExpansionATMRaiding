/**
 * ATMData.c
 *
 * By: LieutenantMaster
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ATMData
{
	string Classname;
	string LocationName
	vector Position;
	vector Rotation;
	
	void ATMData(string _classname, string _location, vector _pos, vector _rot)
	{
		Classname 		= _classname;
		LocationName 	= _location;
		Position 		= _pos;
		Rotation 		= _rot;
	}
};
