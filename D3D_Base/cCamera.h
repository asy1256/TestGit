#pragma once
class cCamera
{
private:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3	m_vLookAt;
	D3DXVECTOR3	m_vUp;

	POINT		m_ptPrevMouse;
	bool		m_isLButtonDown;
	float		m_fCameraDistance;
	D3DXVECTOR3	m_vCamRotAngle;

public:
	cCamera();
	~cCamera();

	void Setup();
	void Update();
	void Update(D3DXVECTOR3 look);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

