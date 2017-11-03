#pragma once

class cGameNode
{
public:
	cGameNode();
	virtual ~cGameNode();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

