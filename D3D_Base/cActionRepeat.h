#pragma once
#include "cAction.h"

class cActionRepeat : public cAction, public iActionDelegate
{
protected:
	SYNTHESIZE(cAction*, m_pAction, Action);

public:
	cActionRepeat();
	virtual ~cActionRepeat();

	virtual void Start() override;
	virtual void Update() override;

	virtual void OnActionFinish(cAction* pSender) override;
};

