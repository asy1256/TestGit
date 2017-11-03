#include "stdafx.h"
#include "cGameObject.h"


cGameObject::cGameObject()
	: m_vDirection(0, 0, 1)
	, m_vPosition(0, 0, 0)
{
}


cGameObject::~cGameObject()
{
}

void cGameObject::OnActionFinish(cAction * pSender)
{
}
