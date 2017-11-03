#include "stdafx.h"
#include "cCharacter.h"


cCharacter::cCharacter()
	: m_pAction(NULL)
	, m_fRotY(0.0f)
{
}


cCharacter::~cCharacter()
{
	SAFE_RELEASE(m_pAction);
}

void cCharacter::Setup()
{
}

void cCharacter::Update()
{
	D3DXMATRIXA16 matR, matT;
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	D3DXVECTOR3	vUp(0, 1, 0);
	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &m_vDirection);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Cross(&vUp, &m_vDirection, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXMatrixLookAtLH(&matR, &D3DXVECTOR3(0, 0, 0), &m_vDirection, &vUp);
	D3DXMatrixTranspose(&matR, &matR);

	if (m_pAction)
	{
		m_pAction->Update();
	}

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorldTM = matR * matT;
}

void cCharacter::Render()
{
}

void cCharacter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cCharacter::OnActionFinish(cAction * pSender)
{
}
