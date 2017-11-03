#pragma once

#include "cAction.h"

class cGameObject : public cObject, public iActionDelegate
{
protected:
	SYNTHESIZE(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cGameObject();
	virtual ~cGameObject();

	virtual void OnActionFinish(cAction* pSender) override;
};

