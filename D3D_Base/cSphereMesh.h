#pragma once

class cSphereMesh
{
private:
	LPD3DXMESH m_pMesh;
	D3DXVECTOR3 m_vecPosition;
	D3DXMATRIXA16 m_matWorld;
	D3DMATERIAL9 m_mtl;
	float m_fRadius;

public:
	cSphereMesh();
	~cSphereMesh();

	void Setup(D3DXVECTOR3 pos, float radius);
	void Render();

	bool HitCheck(D3DXVECTOR3 raypos, D3DXVECTOR3 raydir);
	void ChangeColor(D3DXVECTOR3 raypos, D3DXVECTOR3 raydir);
};