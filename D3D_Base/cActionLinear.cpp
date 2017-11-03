#include "stdafx.h"
#include "cActionLinear.h"
#include "cCubeMan.h"

cActionLinear::cActionLinear()
	: m_fActionTime(0.0f)
	, m_fPassedTime(0.0f)
	, m_fStartTime(0.0f)
	, m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
	, m_vPrevPosition(0, 0, 0)
{
}


cActionLinear::~cActionLinear()
{
}


void cActionLinear::Start()
{
	if (m_pOwner)
	{
		m_pOwner->SetPosition(m_vFrom);
		m_fPassedTime = 0.0f;
		m_fStartTime = g_pTimeManager->GetLastUpdateTime();
	}
}

void cActionLinear::Update()
{
	m_fPassedTime = g_pTimeManager->GetLastUpdateTime() - m_fStartTime;

	if (m_fPassedTime > m_fActionTime)
	{
		m_pOwner->SetPosition(m_vTo);
		if (m_pDelegate)
		{
			m_pDelegate->OnActionFinish(this);
		}
	}
	else
	{
		float t = m_fPassedTime / m_fActionTime;

		D3DXVECTOR3 p = (1.0f - t) * m_vFrom + t * m_vTo;
		m_pOwner->SetPosition(p);

		D3DXVECTOR3 vDir = p - m_vPrevPosition;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pOwner->SetDirection(vDir);
		m_vPrevPosition = p;
	}
}