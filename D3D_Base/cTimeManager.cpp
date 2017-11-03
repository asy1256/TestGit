#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
{
	//m_dwLastUpdateTime = GetTickCount();
	m_dwLastUpdateTime = timeGetTime();
}


cTimeManager::~cTimeManager()
{
}

void cTimeManager::Update()
{
	//DWORD dwCurrTime = GetTickCount();
	DWORD dwCurrTime = timeGetTime();
	m_fEllapsedTime = (dwCurrTime - m_dwLastUpdateTime) / 1000.0f;
	m_dwLastUpdateTime = dwCurrTime;
}

float cTimeManager::GetEllapsedTime()
{
	return m_fEllapsedTime;
}

float cTimeManager::GetLastUpdateTime()
{
	return m_dwLastUpdateTime / 1000.0f;
}