#include "stdafx.h"
#include "cAseLoaderScene.h"

#include "cCamera.h"
#include "cAseLoader.h"
#include "cAseNode.h"
#include "cGrid.h"

cAseLoaderScene::cAseLoaderScene()
{
}

cAseLoaderScene::~cAseLoaderScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pAseLoader);
	m_pAseRoot->Destroy();
	SAFE_DELETE(m_pGrid);
}

HRESULT cAseLoaderScene::Setup()
{
	m_pCamera = NULL;
	m_pAseLoader = NULL;
	m_pAseRoot = NULL;
	m_pGrid = NULL;

	m_pCamera = new cCamera;
	m_pCamera->Setup();
	m_pAseLoader = new cAseLoader;
	m_pAseRoot = m_pAseLoader->LoadFile("woman", "woman_01_all.ase");
	m_pAseRoot->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	return S_OK;
}

void cAseLoaderScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (m_pAseRoot)
		m_pAseRoot->Update();
}

void cAseLoaderScene::Render()
{
	if (m_pAseRoot)
		m_pAseRoot->Render();

	if (m_pGrid)
		m_pGrid->Render();
}

void cAseLoaderScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}
