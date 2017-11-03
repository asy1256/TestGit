#include "stdafx.h"
#include "cSphereMesh.h"

cSphereMesh::cSphereMesh()
	: m_pMesh(NULL)
	, m_vecPosition(0, 0, 0)
	, m_fRadius(0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cSphereMesh::~cSphereMesh()
{
	SAFE_RELEASE(m_pMesh);
}

void cSphereMesh::Setup(D3DXVECTOR3 pos, float radius)
{
	ZeroMemory(&m_mtl, sizeof(D3DMATERIAL9));
	m_mtl.Ambient.r = 1.0f;
	m_mtl.Ambient.g = 1.0f;
	m_mtl.Ambient.b = 1.0f;
	m_mtl.Ambient.a = 1.0f;

	m_mtl.Diffuse.r = 1.0f;
	m_mtl.Diffuse.g = 1.0f;
	m_mtl.Diffuse.b = 1.0f;
	m_mtl.Diffuse.a = 1.0f;

	m_fRadius = radius;
	m_vecPosition = pos;
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);
	m_matWorld = matT;

	D3DXCreateSphere(g_pD3DDevice, m_fRadius, 20, 20, &m_pMesh, 0);
}

void cSphereMesh::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetMaterial(&m_mtl);

	m_pMesh->DrawSubset(0);
}

bool cSphereMesh::HitCheck(D3DXVECTOR3 raypos, D3DXVECTOR3 raydir)
{
	D3DXVECTOR3 v = raypos - m_vecPosition;

	float b = 2.0f * D3DXVec3Dot(&raydir, &v);
	float c = D3DXVec3Dot(&v, &v) - (m_fRadius * m_fRadius);

	float discimi = (b * b) - (4.0f * c);

	if (discimi < 0.0f) { return false; }

	discimi = sqrtf(discimi);

	float s0 = (-b + discimi) / 2.0f;
	float s1 = (-b - discimi) / 2.0f;

	if (s0 >= 0.0f || s1 >= 0.0f) { return true; }

	return false;
}

void cSphereMesh::ChangeColor(D3DXVECTOR3 raypos, D3DXVECTOR3 raydir)
{
	bool Change = HitCheck(raypos, raydir);

	if (Change)
	{
		m_mtl.Ambient.r = 1.0f;
		m_mtl.Ambient.g = 0.0f;
		m_mtl.Ambient.b = 0.0f;
		m_mtl.Ambient.a = 1.0f;

		m_mtl.Diffuse.r = 1.0f;
		m_mtl.Diffuse.g = 0.0f;
		m_mtl.Diffuse.b = 0.0f;
		m_mtl.Diffuse.a = 1.0f;
	}
	else
	{
		m_mtl.Ambient.r = 1.0f;
		m_mtl.Ambient.g = 1.0f;
		m_mtl.Ambient.b = 1.0f;
		m_mtl.Ambient.a = 1.0f;

		m_mtl.Diffuse.r = 1.0f;
		m_mtl.Diffuse.g = 1.0f;
		m_mtl.Diffuse.b = 1.0f;
		m_mtl.Diffuse.a = 1.0f;
	}
}
