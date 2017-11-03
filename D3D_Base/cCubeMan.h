#pragma once

#include "cCharacter.h"

class cCubeNode;
class cHeightMap;

class cCubeMan : public cCharacter
{
protected:
	cCubeNode*			m_pRoot;
	D3DMATERIAL9		m_stMtl;
	LPDIRECT3DTEXTURE9	m_pTexture;

	cHeightMap* m_pHiMap;

public:
	cCubeMan();
	~cCubeMan();

	void Setup();
	void Update();
	void Render();

	D3DXVECTOR3& GetPosition(void) { return m_vPosition; }
	void SetHeightMap(cHeightMap* map) { m_pHiMap = map; }
};

