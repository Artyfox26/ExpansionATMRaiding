class ATMInfected
{
	int Amount;
	int SpawnRadius;
	ref TStringArray Entities;
	
	void ATMInfected(int _amount, int _radius, TStringArray _entities = NULL)
	{
		Amount = _amount;
		SpawnRadius = _radius;
		Entities = _entities;
	}
};
