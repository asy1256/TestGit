#pragma once
class cCube
{
private:
	IDirect3DVertexBuffer9* m_pVertex;
	IDirect3DIndexBuffer9* m_pIndex;
	
	float			m_fRotY;
	D3DXVECTOR3		m_vPosition;
	D3DXVECTOR3		m_vDirection;
	D3DXMATRIXA16	m_matWorld;
	D3DMATERIAL9	m_stMtl;

public:
	cCube();
	~cCube();

	void Setup();
	void Update();
	void Render();

	IDirect3DVertexBuffer9* buffer();
};

