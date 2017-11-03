#pragma once
class cCubeNode
{
protected:
	std::vector<ST_PNT_VERTEX>	m_vecVertex;

	D3DXVECTOR3		m_vLocalPos;

	D3DXMATRIXA16	m_matLocalTM;
	D3DXMATRIXA16	m_matWorldTM;

	D3DXMATRIXA16*	m_pMatParentWorldTM;

	std::vector<cCubeNode*>		m_vecChild;

	float	m_fRotX;

	SYNTHESIZE(float, m_fRotDeltaX, RotDeltaX);

public:
	cCubeNode();
	virtual ~cCubeNode();

	void AddChild(cCubeNode* pChild);
	void Destroy();

	virtual void Setup();
	virtual void Update();
	void Render();

	void SetParentWorldTM(D3DXMATRIXA16* mat) { m_pMatParentWorldTM = mat; }
};

