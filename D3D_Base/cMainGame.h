#pragma once

#include "cAseLoaderScene.h"
#include "cBufferScene.h"
#include "cRawLoaderScene.h"

class cMainGame
{
private:

public:
	cMainGame();
	virtual ~cMainGame();

	void SetLight();

	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

