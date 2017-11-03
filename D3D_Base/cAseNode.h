#pragma once

class cMtlTex;

class cAseNode
{
private:
	IDirect3DVertexBuffer9* m_pVertex;
	IDirect3DIndexBuffer9* m_pIndex;
	D3DVERTEXBUFFER_DESC m_stVerDesc;
	D3DINDEXBUFFER_DESC m_stIdxDesc;

	std::vector<ST_ANI_ROT>			m_vecAniRot;
	std::vector<ST_ANI_POS>			m_vecAniPos;

	D3DXMATRIXA16					m_matLocalTM;
	D3DXMATRIXA16					m_matWorldTM;

	ST_NODE_TM						m_stNodeTm;

	D3DMATERIAL9					m_stMtl;

	D3DXMATRIXA16*					m_pParentsWorldTM;
	D3DXMATRIXA16*					m_pParentsNodeTM;
	cMtlTex*						m_pMtlTex;

	std::vector<cAseNode*> m_vecChild;

	int f;

public:
	cAseNode();
	~cAseNode();

	void Setup();
	void Update();
	void Render();

	void AnimationRotTM(int& f, D3DXMATRIXA16& matr);
	void AnimationPosTM(int& f, D3DXMATRIXA16& matt);

	void AddChild(cAseNode* child);
	void Destroy(void);

	IDirect3DVertexBuffer9& GetVerBuff(void) { return *m_pVertex; }
	IDirect3DIndexBuffer9& GetIdxBuff(void) { return *m_pIndex; }
	void SetVerBuff(IDirect3DVertexBuffer9* buff) { m_pVertex = buff; }
	void SetIdxBuff(IDirect3DIndexBuffer9* buff) { m_pIndex = buff; }
	void SetParentsWorld(D3DXMATRIXA16* pworld) { m_pParentsWorldTM = pworld; }
	void SetMtlTex(cMtlTex* mtltex) { m_pMtlTex = mtltex; }
	void SetNodeTm(ST_NODE_TM& nodetm) { m_stNodeTm = nodetm; }
	void SetAniRot(std::vector<ST_ANI_ROT>& vecrot) { m_vecAniRot = vecrot; }
	void SetAniPos(std::vector<ST_ANI_POS>& vecpos) { m_vecAniPos = vecpos; }
};

