#include "stdafx.h"
#include "cCubeMan.h"

#include "cCubeBody.h"
#include "cCubeHead.h"
#include "cCubeLArm.h"
#include "cCubeRArm.h"
#include "cCubeLLeg.h"
#include "cCubeRLeg.h"
#include "cAction.h"

#include "cHeightMap.h"

cCubeMan::cCubeMan()
	: m_pRoot(NULL)
	, m_pTexture(NULL)
{
}


cCubeMan::~cCubeMan()
{
	if (m_pRoot)
	{
		m_pRoot->Destroy();
	}

	SAFE_RELEASE(m_pTexture);
}

void cCubeMan::Setup()
{
	D3DXMatrixIdentity(&m_matWorldTM);

	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice,
		L"batman.png", &m_pTexture)))
	{
		assert(false && "텍스처 로드 실패!");
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	cCubeBody* pBody = new cCubeBody;
	pBody->Setup();
	pBody->SetParentWorldTM(&m_matWorldTM);

	m_pRoot = pBody;

	cCubeHead* pHead = new cCubeHead;
	pHead->Setup();

	m_pRoot->AddChild(pHead);

	cCubeLArm* pLArm = new cCubeLArm;
	pLArm->Setup();
	pLArm->SetRotDeltaX(-0.1f);

	m_pRoot->AddChild(pLArm);

	cCubeRArm* pRArm = new cCubeRArm;
	pRArm->Setup();
	pRArm->SetRotDeltaX(0.1f);

	m_pRoot->AddChild(pRArm);

	cCubeLLeg* pLLeg = new cCubeLLeg;
	pLLeg->Setup();
	pLLeg->SetRotDeltaX(0.1f);

	m_pRoot->AddChild(pLLeg);

	cCubeRLeg* pRLeg = new cCubeRLeg;
	pRLeg->Setup();
	pRLeg->SetRotDeltaX(-0.1f);

	m_pRoot->AddChild(pRLeg);
}

void cCubeMan::Update()
{
	//cCharacter::Update();

	if (KEYMANAGER->isStayKeyDown('W'))
	{
		m_vPosition = m_vPosition + (m_vDirection * 0.1f);
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		m_vPosition = m_vPosition - (m_vDirection * 0.1f);
	}
	if (KEYMANAGER->isStayKeyDown('A')) { m_fRotY -= 0.1f; }
	if (KEYMANAGER->isStayKeyDown('D')) { m_fRotY += 0.1f; }

	D3DXMATRIXA16	matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorldTM = matR * matT;

	m_pHiMap->GetHeight(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	if (m_pRoot)
	{
		m_pRoot->Update();
	}
}

void cCubeMan::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pD3DDevice->SetTexture(0, m_pTexture);

	if (m_pRoot)
	{
		m_pRoot->Render();
	}
}
