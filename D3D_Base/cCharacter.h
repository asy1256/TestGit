#pragma once

class cAction;

class cCharacter : public cGameObject
{
protected:
	float			m_fRotY;
	D3DXMATRIXA16	m_matWorldTM;

	SYNTHESIZE(cAction*, m_pAction, Action);


public:
	cCharacter();
	virtual ~cCharacter();


	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnActionFinish(cAction* pSender) override;
};

