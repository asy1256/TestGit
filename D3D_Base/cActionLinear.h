#pragma once
#include "cAction.h"

class cActionLinear : public cAction
{
protected:
	DWORD m_fStartTime;
	float m_fPassedTime;

	D3DXVECTOR3 m_vPrevPosition;

	SYNTHESIZE(D3DXVECTOR3, m_vFrom, From);
	SYNTHESIZE(D3DXVECTOR3, m_vTo, To);
	SYNTHESIZE(float, m_fActionTime, ActionTime);

public:
	cActionLinear();
	virtual ~cActionLinear();

	virtual void Start() override;
	virtual void Update() override;
};