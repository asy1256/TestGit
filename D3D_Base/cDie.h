#pragma once

struct ST_DIE_NODE
{
	D3DXVECTOR3 location;
	bool visit;

	ST_DIE_NODE()
		: location(0, 0, 0)
		, visit(false)
	{}
	ST_DIE_NODE(D3DXVECTOR3 pos)
		: visit(false)
	{
		location = pos;
	}
};

struct ST_COST_TABLE
{
	std::vector<float> vecCost;
};

class cDie
{
private:
	std::vector<ST_DIE_NODE> m_vecNode;
	std::vector<ST_COST_TABLE> m_vecCost;

	D3DMATERIAL9 m_mtlPath;
	D3DMATERIAL9 m_mtlNonPath;
	D3DXMATRIXA16 m_matWorld;
	LPD3DXMESH m_pMesh;

public:
	cDie();
	~cDie();

	void Setup();
	void Setup2();
	void Setup3();
	void Proses();
	void Proses2();

	void Render();
};