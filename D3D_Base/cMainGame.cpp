#include "stdafx.h"
#include "cMainGame.h"

cMainGame::cMainGame()
{
}

cMainGame::~cMainGame()
{
	g_pTextureManager->Destroy();
	g_pSceneManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	D3DXVECTOR3		vDir(-1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Setup()
{
	SetLight();
	//g_pSceneManager->addScene("AseScene", new cAseLoaderScene);
	//g_pSceneManager->addScene("BufferScene", new cBufferScene);
	g_pSceneManager->addScene("RawScene", new cRawLoaderScene);

	g_pSceneManager->changeScene("RawScene");
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	g_pSceneManager->Update();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 122),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	g_pSceneManager->Render();

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
