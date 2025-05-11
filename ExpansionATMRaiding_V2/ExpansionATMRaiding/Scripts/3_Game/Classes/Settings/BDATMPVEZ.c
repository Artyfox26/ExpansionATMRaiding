class ATMPVEZ
{
	float Radius;
	string DisplayName;
	bool ShowBorderOnMap;
	bool ShowNameOnMap;
	
	void ATMPVEZ(string _name, float _radius, bool _showBorderOnMap, bool _showNameOnMap)
	{
		Radius = _radius;
		DisplayName = _name;
		ShowBorderOnMap = _showBorderOnMap;
		ShowNameOnMap = _showNameOnMap;
	}
};
