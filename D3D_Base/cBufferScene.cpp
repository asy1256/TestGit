#include "stdafx.h"
#include "cBufferScene.h"

#include "cCamera.h"
#include "cCube.h"
#include "cAseLoader.h"
#include "cAseNode.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "cSphereMesh.h"

cBufferScene::cBufferScene()
	: m_pCamra(NULL)
	, m_pCube(NULL)
	, m_pAseLoader(NULL)
	, m_pRoot(NULL)
	, m_pMesh(NULL)
	, m_pObjLoader(NULL)
{
}

cBufferScene::~cBufferScene()
{
	SAFE_DELETE(m_pCamra);
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pAseLoader);
	SAFE_RELEASE(m_pMesh);
	SAFE_DELETE(m_pObjLoader);
	if (m_pRoot)
		m_pRoot->Destroy();

	for (int i = 0; i < m_vecGroup.size(); ++i)
	{
		SAFE_DELETE(m_vecGroup[i]);
	}

	for (int i = 0; i < m_vecSphere.size(); ++i)
	{
		SAFE_DELETE(m_vecSphere[i]);
	}
}

HRESULT cBufferScene::Setup()
{
	m_pCamra = new cCamera;
	m_pCamra->Setup();

	//m_pCube = new cCube;
	//m_pCube->Setup();

	//m_pAseLoader = new cAseLoader;
	//m_pRoot = m_pAseLoader->LoadFile("woman", "woman_01_all.ase");
	//m_pRoot->Setup();

	//m_pObjLoader = new cObjLoader;
	//m_pObjLoader->CreatMeshFromFile(m_pMesh, "obj", "map.obj");
	//m_pObjLoader->Load(m_vecGroup, "obj", "map.obj");
	//
	//D3DXMATRIXA16 matS, matR, matWorld;
	//D3DXMatrixScaling(&matS, 0.02f, 0.02f, 0.02f);
	//D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	//matWorld = matS * matR;
	//
	//for (int i = 0; i < m_vecGroup.size(); ++i)
	//{
	//	//std::vector<ST_PNT_VERTEX> temp = m_vecGroup[i]->GetVertex();
	//	for (int j = 0; j < m_vecGroup[i]->GetVertex().size(); ++j)
	//	{
	//		D3DXVec3TransformCoord(&m_vecGroup[i]->GetVertex()[j].p, &m_vecGroup[i]->GetVertex()[j].p, &matWorld);
	//	}
	//}
	
	cSphereMesh* temp;
	for (int i = 0, x = -3; i < 4; ++i, x += 2)
	{
		temp = new cSphereMesh;
		temp->Setup(D3DXVECTOR3(x, 0, 0), 1.0f);
		m_vecSphere.push_back(temp);
	}

	return S_OK;
}

void cBufferScene::Update()
{
	if (m_pCamra)
		m_pCamra->Update();

	if (m_pRoot)
		m_pRoot->Update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		D3DXVECTOR3 raypos(0, 0, 0);
		D3DXVECTOR3 raydir(0, 0, 0);

		Picking(raypos, raydir, (int)g_ptMouse.x, (int)g_ptMouse.y);

		for (int i = 0; i < m_vecSphere.size(); ++i)
		{
			m_vecSphere[i]->ChangeColor(raypos, raydir);
		}
	}
}

void cBufferScene::Render()
{
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (m_pCube)
		m_pCube->Render();

	if (m_pRoot)
		m_pRoot->Render();

	if (m_pMesh)
	{
		m_pMesh->DrawSubset(0);
	}

	for (int i = 0; i < m_vecSphere.size(); ++i)
	{
		m_vecSphere[i]->Render();
	}

	//성능테스트
	{
		//LARGE_INTEGER Frequnsy;
		//LARGE_INTEGER vecTimeST;
		//LARGE_INTEGER vecTimeED;
		//QueryPerformanceFrequency(&Frequnsy);
		//
		//QueryPerformanceCounter(&vecTimeST);
		//for (int i = 0; i < 1000; ++i)
		//{
		//	for (int j = 0; j < m_vecGroup.size(); ++j)
		//	{
		//		m_vecGroup[j]->Render();
		//	}
		//}
		//QueryPerformanceCounter(&vecTimeED);
		//
		//LARGE_INTEGER meshTimeST;
		//LARGE_INTEGER meshTimeED;
		//QueryPerformanceCounter(&meshTimeST);
		//for (int i = 0; i < 1000; ++i)
		//{
		//	if (m_pMesh)
		//	{
		//		m_pMesh->DrawSubset(0);
		//	}
		//}
		//QueryPerformanceCounter(&meshTimeED);
		//
		//__int64 vecUseTime = vecTimeED.QuadPart - vecTimeST.QuadPart;
		//__int64 meshUseTime = meshTimeED.QuadPart - meshTimeST.QuadPart;
		//
		//double vec = (double)vecUseTime / (double)Frequnsy.QuadPart;
		//double mesh = (double)meshUseTime / (double)Frequnsy.QuadPart;
		//
		//int a = 0;
	}
}

void cBufferScene::Picking(D3DXVECTOR3& raypos, D3DXVECTOR3& raydir, int x, int y)
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 proj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &proj);

	raydir.x = (((2.0f * x) / vp.Width) - 1.0f) / proj(0, 0);
	raydir.y = (((-2.0f * y) / vp.Height) + 1.0f) / proj(1, 1);
	raydir.z = 1.0f;

	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	D3DXVec3TransformCoord(&raypos, &raypos, &matInvView);
	D3DXVec3TransformNormal(&raydir, &raydir, &matInvView);
	D3DXVec3Normalize(&raydir, &raydir);
}

void cBufferScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamra)
	{
		m_pCamra->WndProc(hWnd, message, wParam, lParam);
	}
}
