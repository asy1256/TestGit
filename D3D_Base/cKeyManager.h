#pragma once

#include <bitset>

#define KEYMAX 256
#define KEYMANAGER cKeyManager::GetInstance()

class cKeyManager
{
	SINGLETONE(cKeyManager)
private:
	std::bitset<KEYMAX> _keyUp;
	std::bitset<KEYMAX> _keyDown;

public:
	bool isOnceKeyDown(int key);
	bool isOnceKeyUp(int key);
	bool isStayKeyDown(int key);
	bool isToggleKey(int key);

	std::bitset<KEYMAX> getKeyDown() { return _keyDown; }
	std::bitset<KEYMAX> getKeyUp() { return _keyUp; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }
};

