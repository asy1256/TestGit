#pragma once
#include <timeapi.h>

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETONE(cTimeManager);

	DWORD m_dwLastUpdateTime;
	float m_fEllapsedTime;

public:
	void Update();
	float GetEllapsedTime();
	float GetLastUpdateTime();
};

