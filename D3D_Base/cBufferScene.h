#pragma once

#include "cGameNode.h"

class cCamera;
class cCube;
class cAseLoader;
class cAseNode;
class cObjLoader;
class cGroup;
class cSphereMesh;

class cBufferScene : public cGameNode
{
private:
	cCamera* m_pCamra;
	cCube* m_pCube;
	cAseLoader* m_pAseLoader;
	cAseNode* m_pRoot;
	cObjLoader* m_pObjLoader;
	std::vector<cGroup*> m_vecGroup;
	std::vector<cSphereMesh*> m_vecSphere;

	LPD3DXMESH m_pMesh;

public:
	cBufferScene();
	~cBufferScene();

	virtual HRESULT Setup();
	virtual void Update();
	virtual void Render();

	void Picking(D3DXVECTOR3& raypos, D3DXVECTOR3& raydir, int x, int y);

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};