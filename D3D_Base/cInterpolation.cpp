#include "stdafx.h"
#include "cInterpolation.h"
#include "cCubeMan.h"
#include "cActionBezier.h"
#include "cActionLinear.h"
#include "cActionSeq.h"
#include "cActionRepeat.h"


cInterpolation::cInterpolation()
{
}


cInterpolation::~cInterpolation()
{
	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(m_pArrCubeMan[i]);
	}
}

void cInterpolation::Setup()
{
	for (int i = 0; i < 2; i++)
	{
		m_pArrCubeMan[i] = new cCubeMan;
		m_pArrCubeMan[i]->Setup();
	}

	for (int i = 0; i < 6; i++)
	{
		float fRadius = 10.0f;
		D3DXVECTOR3 vPos = D3DXVECTOR3(
			cosf(i * D3DX_PI / 3.0f) * fRadius,
			0.0f,
			sinf(i * D3DX_PI / 3.0f) * fRadius);

		ST_PC_VERTEX v;
		v.p = vPos;
		v.c = D3DXCOLOR(1, 1, 0, 1);
		m_vecLineVertex.push_back(v);
	}
	m_vecLineVertex.push_back(m_vecLineVertex.front());

	cActionRepeat* pActionRepeat = new cActionRepeat;
	cActionSeq* pActionSeq = new cActionSeq;

	for (int i = 0; i < m_vecLineVertex.size() - 1; i++)
	{
		cActionLinear* pActionLinear = new cActionLinear;
		pActionLinear->SetFrom(m_vecLineVertex[i].p);
		pActionLinear->SetTo(m_vecLineVertex[i + 1].p);
		pActionLinear->SetActionTime(1.0f);
		pActionLinear->SetOwner(m_pArrCubeMan[0]);
		pActionLinear->SetDelegate(pActionSeq);

		pActionSeq->AddAction(pActionLinear);
	}
	pActionRepeat->SetAction(pActionSeq);

	m_pArrCubeMan[0]->SetAction(pActionRepeat);
	pActionRepeat->Start();

	// bezier
	pActionRepeat = new cActionRepeat;
	pActionSeq = new cActionSeq;

	for (int i = 0; i < m_vecLineVertex.size() - 2; i += 2)
	{
		cActionBezier* pActionBezier = new cActionBezier;
		pActionBezier->SetFrom(m_vecLineVertex[i].p);
		pActionBezier->SetVia(m_vecLineVertex[i + 1].p);
		pActionBezier->SetTo(m_vecLineVertex[i + 2].p);
		pActionBezier->SetActionTime(5.0f);
		pActionBezier->SetOwner(m_pArrCubeMan[1]);
		pActionBezier->SetDelegate(pActionSeq);

		pActionSeq->AddAction(pActionBezier);
	}
	pActionRepeat->SetAction(pActionSeq);

	m_pArrCubeMan[1]->SetAction(pActionRepeat);
	pActionRepeat->Start();
}

void cInterpolation::Update()
{
	for (int i = 0; i < 2; i++)
	{
		m_pArrCubeMan[i]->Update();
	}
}

void cInterpolation::Render()
{
	for (int i = 0; i < 2; i++)
	{
		m_pArrCubeMan[i]->Render();
	}

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP,
		m_vecLineVertex.size() - 1,
		&m_vecLineVertex[0],
		sizeof(ST_PC_VERTEX));
}
