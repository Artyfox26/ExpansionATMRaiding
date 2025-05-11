modded class ExpansionATMBase
{
    int m_IsRaided;

    void ExpansionATMBase()
    {
		RegisterNetSyncVariableInt("m_IsRaided", -1, 1);
    }
};
