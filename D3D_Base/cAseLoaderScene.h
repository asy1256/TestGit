#pragma once

#include "cGameNode.h"

class cCamera;
class cAseLoader;
class cAseNode;
class cGrid;

class cAseLoaderScene : public cGameNode
{
private:
	cCamera* m_pCamera;
	cAseLoader* m_pAseLoader;
	cAseNode* m_pAseRoot;
	cGrid* m_pGrid;
public:
	cAseLoaderScene();
	~cAseLoaderScene();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};