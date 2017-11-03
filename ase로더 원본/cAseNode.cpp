#include "stdafx.h"
#include "cAseNode.h"

#include "cMtlTex.h"

cAseNode::cAseNode()
	: m_pParentsWorldTM(NULL)
	, m_pMtlTex(NULL)
{
}

cAseNode::~cAseNode()
{
}

void cAseNode::Setup()
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
	ZeroMemory(&m_stMtl, sizeof(m_stMtl));
	m_stMtl.Ambient.r = m_stMtl.Ambient.g = m_stMtl.Ambient.b = 1.0f;
	m_stMtl.Diffuse.r = m_stMtl.Diffuse.g = m_stMtl.Diffuse.b = 1.0f;

	f = 0;

	//애니메이션 로테이션 쿼터니온값 보정
	for (int i = 0; i < m_vecAniRot.size(); ++i)
	{
		if (i != 0)
		{
			m_vecAniRot[i].qtnion = m_vecAniRot[i - 1].qtnion * m_vecAniRot[i].qtnion;
		}
	}

	//부모 여부확인후에 부모가 있을시 부모역행렬을 노드TM에 곱해서 로컬생성후
	//로컬의 역행렬을 버텍스에 곱해서 위치 조정
	if (m_pParentsNodeTM)
	{
		D3DXMATRIXA16 pworld;
		D3DXMatrixInverse(&pworld, 0, &(*m_pParentsNodeTM));
		m_matLocalTM = m_stNodeTm.matTm * pworld;
	}
	else
	{
		m_matLocalTM = m_stNodeTm.matTm;
	}

	D3DXMATRIXA16 nodeinvers;
	D3DXMatrixInverse(&nodeinvers, 0, &m_stNodeTm.matTm);
	
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&m_vecVertex[i].p, &m_vecVertex[i].p, &nodeinvers);
	}

	for each (auto p in m_vecChild)
	{
		p->Setup();
	}
}

void cAseNode::Update()
{
	D3DXMATRIXA16 matR, matT;
	if (f > 3200) { f = 640; }
	f += 50;

	//IDirect3DStateBlock9 v;

	AnimationRotTM(f, matR);
	AnimationPosTM(f, matT);
	
	m_matLocalTM = matR * matT;

	if (m_pParentsWorldTM != NULL) { m_matWorldTM = m_matLocalTM * (*m_pParentsWorldTM); }
	else { m_matWorldTM = m_matLocalTM; }
	
	for each (auto p in m_vecChild)
	{
		p->Update();
	}
}

void cAseNode::Render()
{
	if (m_pMtlTex != NULL)
	{
		g_pD3DDevice->SetMaterial(&m_pMtlTex->GetMaterial());
		if (m_pMtlTex->GetTexture() != NULL)
		{
			g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		}

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}
	else { g_pD3DDevice->SetMaterial(&m_stMtl); }

	for each (auto p in m_vecChild)
	{
		p->Render();
	}
}

void cAseNode::AnimationRotTM(int& f, D3DXMATRIXA16& matr)
{
	D3DXMatrixIdentity(&matr);

	int nowframe = f;
	int next = 0;

	if (m_vecAniRot.empty())
	{
		matr = m_matLocalTM;
		matr._41 = matr._42 = matr._43 = 0.0f;
		return;
	}

	if (nowframe <= m_vecAniRot.front().frame)
	{
		D3DXMatrixRotationQuaternion(&matr, &m_vecAniRot.front().qtnion);
		return;
	}

	if (nowframe >= m_vecAniRot.back().frame)
	{
		D3DXMatrixRotationQuaternion(&matr, &m_vecAniRot.back().qtnion);
		return;
	}

	for (int i = 0; i < m_vecAniRot.size(); ++i)
	{
		if (nowframe < m_vecAniRot[i].frame)
		{
			next = i;
			break;
		}
	}

	float t = (nowframe - m_vecAniRot[next - 1].frame) /
		(float)(m_vecAniRot[next].frame - m_vecAniRot[next - 1].frame);
	D3DXQUATERNION tempQ;
	D3DXQuaternionSlerp(&tempQ, &m_vecAniRot[next - 1].qtnion, &m_vecAniRot[next].qtnion, t);
	D3DXMatrixRotationQuaternion(&matr, &tempQ);
}

void cAseNode::AnimationPosTM(int& f, D3DXMATRIXA16& matt)
{
	D3DXMatrixIdentity(&matt);

	int nowframe = f;
	int next = 0;

	if (m_vecAniPos.empty())
	{
		matt._41 = m_matLocalTM._41;
		matt._42 = m_matLocalTM._42;
		matt._43 = m_matLocalTM._43;
		return;
	}

	if (nowframe <= m_vecAniPos.front().frame)
	{
		matt._41 = m_vecAniPos.front().position.x;
		matt._42 = m_vecAniPos.front().position.y;
		matt._43 = m_vecAniPos.front().position.z;
		return;
	}

	if (nowframe >= m_vecAniPos.back().frame)
	{
		matt._41 = m_vecAniPos.back().position.x;
		matt._42 = m_vecAniPos.back().position.y;
		matt._43 = m_vecAniPos.back().position.z;
		return;
	}

	for (int i = 0; i < m_vecAniPos.size(); ++i)
	{
		if (nowframe < m_vecAniPos[i].frame)
		{
			next = i;
			break;
		}
	}

	float t = (nowframe - m_vecAniPos[next - 1].frame) /
		(float)(m_vecAniPos[next].frame - m_vecAniPos[next - 1].frame);
	D3DXVECTOR3 tempV;
	D3DXVec3Lerp(&tempV, &m_vecAniPos[next - 1].position, &m_vecAniPos[next].position, t);

	matt._41 = tempV.x;
	matt._42 = tempV.y;
	matt._43 = tempV.z;
}

void cAseNode::AddChild(cAseNode* child)
{
	child->m_pParentsWorldTM = &m_matWorldTM;
	child->m_pParentsNodeTM = &m_stNodeTm.matTm;
	m_vecChild.push_back(child);
}

void cAseNode::Destroy(void)
{
	for each (auto p in m_vecChild)
	{
		p->Destroy();
	}

	delete this;
}
