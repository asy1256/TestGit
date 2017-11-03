#include "stdafx.h"
#include "cCube.h"

cCube::cCube()
	: m_fRotY(0.0f)
	, m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_pVertex(NULL)
	, m_pIndex(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCube::~cCube()
{
	SAFE_RELEASE(m_pVertex);
	SAFE_RELEASE(m_pIndex);
}

void cCube::Setup()
{
	buffer();

	//g_pD3DDevice->CreateVertexBuffer(8 * sizeof(ST_PN_VERTEX), 0, ST_PN_VERTEX::FVF, D3DPOOL_MANAGED, &m_pVertex, 0);
	g_pD3DDevice->CreateIndexBuffer(36 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndex, 0);

	ST_PN_VERTEX* vertex;
	WORD* index;
	ST_PN_VERTEX temp;
	temp.n = D3DXVECTOR3(0, 0, -1);

	D3DXCOLOR tempColor;
	tempColor.r = tempColor.g = tempColor.b = 1.0f;

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = tempColor;
	m_stMtl.Diffuse = tempColor;
	m_stMtl.Specular = tempColor;

	//버텍스 설정
	/*{
		m_pVertex->Lock(0, 0, (void**)&vertex, 0);

		temp.p = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		vertex[0] = temp;
		temp.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
		vertex[1] = temp;
		temp.p = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
		vertex[2] = temp;
		temp.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
		vertex[3] = temp;

		temp.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
		vertex[4] = temp;
		temp.p = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
		vertex[5] = temp;
		temp.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		vertex[6] = temp;
		temp.p = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
		vertex[7] = temp;

		m_pVertex->Unlock();
	}*/

	//인덱스 설정
	{
		m_pIndex->Lock(0, 0, (void**)&index, 0);

		index[0] = 0; index[1] = 1; index[2] = 2;
		index[3] = 0; index[4] = 2; index[5] = 3;
		index[6] = 4; index[7] = 6; index[8] = 5;
		index[9] = 4; index[10] = 7; index[11] = 6;
		index[12] = 4; index[13] = 5; index[14] = 1;
		index[15] = 4; index[16] = 1; index[17] = 0;
		index[18] = 3; index[19] = 2; index[20] = 6;
		index[21] = 3; index[22] = 6; index[23] = 7;
		index[24] = 1; index[25] = 5; index[26] = 6;
		index[27] = 1; index[28] = 6; index[29] = 2;
		index[30] = 4; index[31] = 0; index[32] = 3;
		index[33] = 4; index[34] = 3; index[35] = 7;

		m_pIndex->Unlock();
	}

}

void cCube::Update()
{
}

void cCube::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetStreamSource(0, m_pVertex, 0, sizeof(ST_PN_VERTEX));
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetIndices(m_pIndex);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

IDirect3DVertexBuffer9* cCube::buffer()
{
	IDirect3DVertexBuffer9* tempB;
	ST_PN_VERTEX* vertex;
	WORD* index;
	ST_PN_VERTEX temp;

	g_pD3DDevice->CreateVertexBuffer(8 * sizeof(ST_PN_VERTEX), 0, ST_PN_VERTEX::FVF, D3DPOOL_MANAGED, &tempB, 0);


	{
		tempB->Lock(0, 0, (void**)&vertex, 0);

		temp.p = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		vertex[0] = temp;
		temp.p = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
		vertex[1] = temp;
		temp.p = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
		vertex[2] = temp;
		temp.p = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
		vertex[3] = temp;

		temp.p = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
		vertex[4] = temp;
		temp.p = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
		vertex[5] = temp;
		temp.p = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		vertex[6] = temp;
		temp.p = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
		vertex[7] = temp;

		tempB->Unlock();
	}

	D3DVERTEXBUFFER_DESC dc;
	tempB->GetDesc(&dc);

	int a = dc.Size / sizeof(ST_PN_VERTEX);

	this->m_pVertex = tempB;

	return tempB;
}
