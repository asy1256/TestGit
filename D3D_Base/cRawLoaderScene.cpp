#include "stdafx.h"
#include "cRawLoaderScene.h"

#include "cCamera.h"
#include "cGrid.h"
#include "cHeightMap.h"
#include "cCubeMan.h"
#include "cDie.h"

cRawLoaderScene::cRawLoaderScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pHiMap(NULL)
	, m_pCubeMan(NULL)
	, m_pDie(NULL)
{
}

cRawLoaderScene::~cRawLoaderScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pHiMap);
	SAFE_DELETE(m_pCubeMan);
	SAFE_DELETE(m_pDie);
}

HRESULT cRawLoaderScene::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	//m_pHiMap = new cHeightMap;
	//m_pHiMap->Setup("HeightMapData/", "HeightMap.raw", "terrain.jpg");
	//
	//m_pCubeMan = new cCubeMan;
	//m_pCubeMan->Setup();
	//m_pCubeMan->SetHeightMap(m_pHiMap);

	m_pDie = new cDie;
	m_pDie->Setup2();
	m_pDie->Proses2();

	return S_OK;
}

void cRawLoaderScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (m_pCubeMan)
		m_pCubeMan->Update();

}

void cRawLoaderScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();
	
	if (m_pHiMap)
		m_pHiMap->Render();

	if (m_pCubeMan)
		m_pCubeMan->Render();

	if (m_pDie)
		m_pDie->Render();

}

void cRawLoaderScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}
