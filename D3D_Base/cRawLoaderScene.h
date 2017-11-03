#pragma once

#include "cGameNode.h"

class cCamera;
class cGrid;
class cHeightMap;
class cCubeMan;
class cDie;

class cRawLoaderScene : public cGameNode
{
private:
	cCamera* m_pCamera;
	cGrid* m_pGrid;
	cHeightMap* m_pHiMap;
	cCubeMan* m_pCubeMan;
	
	cDie* m_pDie;

public:
	cRawLoaderScene();
	~cRawLoaderScene();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};