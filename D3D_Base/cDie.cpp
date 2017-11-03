#include "stdafx.h"
#include "cDie.h"

cDie::cDie()
	: m_pMesh(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_mtlPath, sizeof(D3DMATERIAL9));
	m_mtlPath.Ambient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_mtlPath.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_mtlPath.Specular = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_mtlNonPath.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlNonPath.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlNonPath.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXCreateSphere(g_pD3DDevice, 0.3f, 30, 30, &m_pMesh, 0);
}

cDie::~cDie()
{
	SAFE_RELEASE(m_pMesh);
}

void cDie::Setup()
{
	ST_DIE_NODE temp;

	//노드 위치 지정
	temp.location = D3DXVECTOR3(0, 0, 0);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(2, 0, 4);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(3, 0, 2);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(3, 0, 2);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(4, 0, 4);
	m_vecNode.push_back(temp);

	//코스트 테이블 리사이즈
	m_vecCost.resize(m_vecNode.size());
	for (int i = 0; i < m_vecCost.size(); ++i)
	{
		m_vecCost[i].vecCost.resize(m_vecNode.size());
	}

	for (int i = 0; i < m_vecCost.size(); ++i)
	{
		for (int j = 0; j < m_vecCost[i].vecCost.size(); ++j)
		{
			m_vecCost[i].vecCost[j] = D3DXVec3Length(&(m_vecNode[j].location - m_vecNode[i].location));;
		}
	}

	int a = 0;
}

void cDie::Setup2()
{
	ST_DIE_NODE temp;

	//노드 위치 지정
	temp.location = D3DXVECTOR3(1, 0, 1);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(-2, 0, -1);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(1, 0, -1);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(3, 0, -1.5f);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(0, 0, -5);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(2.5f, 0, -4);
	m_vecNode.push_back(temp);
	temp.location = D3DXVECTOR3(0, 0, 0);
	m_vecNode.push_back(temp);

	//코스트 테이블 리사이즈
	m_vecCost.resize(m_vecNode.size());
	for (int i = 0; i < m_vecCost.size(); ++i)
	{
		m_vecCost[i].vecCost.resize(m_vecNode.size());
	}

	float aryy[7][7] = { 0, 10000, 2, 10000, 10000, 3, 1,
		10000, 0, 10000, 10000, 4, 10000, 3,
		2, 10000, 0, 1, 10, 10000, 10000,
		10000, 10000, 1, 0, 3, 10000, 10000,
		10000, 4, 10, 3, 0, 5, 6,
		3, 10000, 10000, 10000, 5, 0, 10000,
		1, 3, 10000, 10000, 6, 10000, 0 };

	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			m_vecCost[i].vecCost[j] = aryy[i][j];
		}
	}

	int a = 0;
}

void cDie::Setup3()
{

}

void cDie::Proses()
{
	float min = 10000.0f;
	int from = 0;
	int to = 4;

	std::vector<float> vecCost;
	std::vector<int> vecVia;

	vecCost.resize(m_vecNode.size());

	for (int i = 0; i < vecCost.size(); ++i)
	{
		vecCost[i] = min;
	}

	min = 0;
	vecVia.push_back(from);

	for (int i = from; i < to; ++i)
	{

		for (int j = 0; j < m_vecCost[i].vecCost.size(); ++j)
		{
			if (min + m_vecCost[i].vecCost[j] <= vecCost[j])
			{
				vecCost[j] = min + m_vecCost[i].vecCost[j];
			}
		}

		int a = 0;
		int idx = 0;
		float temp = 10000.0f;

		for (int k = 0; k < vecCost.size(); ++k)
		{
			if (k <= i) { continue; }

			if (vecCost[k] < temp)
			{
				temp = vecCost[k];
				idx = k;
			}
		}

		if (idx != 0)
		{
			bool fair = false;
			for (int l = 0; l < vecVia.size(); ++l)
			{
				if (vecVia[l] == idx) { fair = true; }
			}
			if (!fair) { vecVia.push_back(idx); }
		}
		min = temp;
	}


	int a = 0;
}

void cDie::Proses2()
{
	float min = 10000.0f;
	int from = 6;
	int to = 4;
	int curNode = from;

	std::vector<float> vecCost;
	std::vector<float> vecVia;
	vecCost.resize(m_vecNode.size());
	vecVia.resize(m_vecNode.size());

	for (int i = 0; i < vecCost.size(); ++i)
	{
		vecCost[i] = min;
	}

	min = 0;

	for (int i = 0; i < m_vecNode.size(); ++i)
	{
		//코스트 갱신
		for (int j = 0; j < m_vecNode.size(); ++j)
		{
			if (m_vecNode[j].visit) { continue; }
			
			if (vecCost[j] > min + m_vecCost[curNode].vecCost[j])
			{
				vecCost[j] = min + m_vecCost[curNode].vecCost[j];
				vecVia[j] = curNode;
			}
		}
		m_vecNode[curNode].visit = true;
		float temp = 10000.0f;
		//미니멈 값 갱신 및 현재노드 변경
		for (int j = 0; j < m_vecNode.size(); ++j)
		{
			if (m_vecNode[j].visit) { continue; }

			if (vecCost[j] < temp)
			{
				temp = vecCost[j];
				curNode = j;
			}
		}
		min = temp;
	}

	std::vector<int> way;

	way.push_back(to);

	while (true)
	{
		if (way.back() == from) { break; }
		way.push_back(vecVia[way.back()]);
	}

	for (int i = 0; i < m_vecNode.size(); ++i)
	{
		m_vecNode[i].visit = false;
	}

	for (int i = 0; i < way.size(); ++i)
	{
		m_vecNode[way[i]].visit = true;
	}

	int a = 0;
}

void cDie::Render()
{
	for (int i = 0; i < m_vecNode.size(); ++i)
	{
		if (m_vecNode[i].visit) { g_pD3DDevice->SetMaterial(&m_mtlPath); }
		else { g_pD3DDevice->SetMaterial(&m_mtlNonPath); }
		D3DXMatrixTranslation(&m_matWorld, m_vecNode[i].location.x, m_vecNode[i].location.y, m_vecNode[i].location.z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pMesh->DrawSubset(0);
	}
}
