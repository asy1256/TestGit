#include "stdafx.h"
#include "cCubeNode.h"


cCubeNode::cCubeNode()
	: m_pMatParentWorldTM(NULL)
	, m_vLocalPos(0, 0, 0)
	, m_fRotX(0.0f)
	, m_fRotDeltaX(0.0f)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cCubeNode::~cCubeNode()
{
	int a = 0;
}

void cCubeNode::AddChild(cCubeNode * pChild)
{
	pChild->m_pMatParentWorldTM = &m_matWorldTM;

	m_vecChild.push_back(pChild);
}

void cCubeNode::Destroy()
{
	for each (auto p in m_vecChild)
	{
		p->Destroy();
	}

	delete this;
}

void cCubeNode::Setup()
{

	std::vector<ST_PNT_VERTEX> vecVertex;
	vecVertex.resize(8);

	vecVertex[0].p.x = -0.5f; vecVertex[0].p.y = -0.5f; vecVertex[0].p.z = -0.5f;
	vecVertex[1].p.x = -0.5f; vecVertex[1].p.y = 0.5f; vecVertex[1].p.z = -0.5f;
	vecVertex[2].p.x = 0.5f; vecVertex[2].p.y = 0.5f; vecVertex[2].p.z = -0.5f;
	vecVertex[3].p.x = 0.5f; vecVertex[3].p.y = -0.5f; vecVertex[3].p.z = -0.5f;

	vecVertex[4].p.x = -0.5f; vecVertex[4].p.y = -0.5f; vecVertex[4].p.z = 0.5f;
	vecVertex[5].p.x = -0.5f; vecVertex[5].p.y = 0.5f; vecVertex[5].p.z = 0.5f;
	vecVertex[6].p.x = 0.5f; vecVertex[6].p.y = 0.5f; vecVertex[6].p.z = 0.5f;
	vecVertex[7].p.x = 0.5f; vecVertex[7].p.y = -0.5f; vecVertex[7].p.z = 0.5f;


	//Å©±â°¡ (1,1,1) »óÀÚ ¼¼ÆÃ
	std::vector<DWORD> vecIndex;

	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);	//¾Õ¸é À­»ï°¢

	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);	//¾Õ¸é ¾Æ·§»ï°¢

	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);	//µÞ¸é À­»ï°¢

	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);	//µÞ¸é ¾Æ·§»ï°¢

	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);	//¿Þ¸é À­»ï°¢

	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);	//¿Þ¸é ¾Æ·§»ï°¢

	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);	//¿À¸¥¸é À­»ï°¢

	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);	//¿À¸¥¸é ¾Æ·§»ï°¢

	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);	//À­¸é À­»ï°¢

	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);	//À­¸é ¾Æ·§»ï°¢

	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);	//¾Æ·§¸é À­»ï°¢

	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);	//¾Æ·§¸é ¾Æ·§»ï°¢

	m_vecVertex.resize(36);

	for (size_t i = 0; i < vecIndex.size(); i += 3)
	{
		m_vecVertex[i] = vecVertex[vecIndex[i]];
		m_vecVertex[i + 1] = vecVertex[vecIndex[i + 1]];
		m_vecVertex[i + 2] = vecVertex[vecIndex[i + 2]];
	}

	D3DXVECTOR3 u, v, n;

	for (int i = 0; i < 36; i += 3)
	{
		u = m_vecVertex[i + 1].p - m_vecVertex[i].p;
		v = m_vecVertex[i + 2].p - m_vecVertex[i].p;

		D3DXVec3Cross(&n, &u, &v);
		D3DXVec3Normalize(&n, &n);

		m_vecVertex[i].n = n;
		m_vecVertex[i + 1].n = n;
		m_vecVertex[i + 2].n = n;
	}
}

void cCubeNode::Update()
{
	m_fRotX += m_fRotDeltaX;

	if (m_fRotX > D3DX_PI / 6.0f)
	{
		m_fRotX = D3DX_PI / 6.0f;
		m_fRotDeltaX *= -1;
	}
	
	if (m_fRotX < -D3DX_PI / 6.0f)
	{
		m_fRotX = -D3DX_PI / 6.0f;
		m_fRotDeltaX *= -1;
	}

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	D3DXMatrixRotationX(&matR, m_fRotX);

	D3DXMatrixTranslation(&matT, m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);
	m_matLocalTM = matR * matT;

	m_matWorldTM = m_matLocalTM;

	if (m_pMatParentWorldTM)
	{
		m_matWorldTM *= (*m_pMatParentWorldTM);
	}

	for each (auto p in m_vecChild)
	{
		p->Update();
	}
}

void cCubeNode::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PNT_VERTEX));

	for each (auto p in m_vecChild)
	{
		p->Render();
	}
}
