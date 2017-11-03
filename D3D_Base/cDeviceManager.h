#pragma once

#define g_pDeviceManager cDeviceManager::GetInstance()
#define g_pD3DDevice cDeviceManager::GetInstance()->GetDevice()
#define g_pSprite cDeviceManager::GetInstance()->GetSprite()

class cDeviceManager
{
	SINGLETONE(cDeviceManager)

private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;
	LPD3DXSPRITE		m_pSprite;

public:
	LPDIRECT3DDEVICE9	GetDevice();
	LPD3DXSPRITE		GetSprite();
	void Destroy();
};

