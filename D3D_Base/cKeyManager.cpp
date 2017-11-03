#include "stdafx.h"
#include "cKeyManager.h"

cKeyManager::cKeyManager()
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->setKeyUp(i, false);
		this->setKeyDown(i, false);
	}
}

cKeyManager::~cKeyManager()
{
}

bool cKeyManager::isOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->getKeyDown()[key])
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);
	return false;
}

bool cKeyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}
	return false;
}

bool cKeyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

bool cKeyManager::isToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}
