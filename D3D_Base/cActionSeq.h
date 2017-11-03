#pragma once

#include "cAction.h"

class cActionSeq : public cAction, public iActionDelegate
{
protected:
	std::list<cAction*>				m_lstAction;
	std::list<cAction*>::iterator	m_itCurrentAction;

public:
	cActionSeq();
	virtual ~cActionSeq();

	void AddAction(cAction* pAction);

	virtual void Start() override;
	virtual void Update() override;

	virtual void OnActionFinish(cAction* pSender) override;
};

