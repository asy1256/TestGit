#pragma once
#include "iMap.h"

class cHeightMap : public iMap
{
private:
	LPD3DXMESH					m_pMesh;
	std::vector<D3DXVECTOR3>	m_vecVertex;
	D3DMATERIAL9				m_mtl;
	LPDIRECT3DTEXTURE9			m_pTexture;
	int							m_nRow;

public:
	cHeightMap();
	virtual ~cHeightMap();

	void Setup( IN char* szFolder, IN char* szRaw, IN char* szTexture );

	virtual bool GetHeight( IN float x, OUT float& y, IN float z ) override;
	virtual void Render() override;
};

