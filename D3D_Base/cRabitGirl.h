#pragma once

#include "cCharacter.h"

class cAseLoader;

class cRabitGirl : public cCharacter
{
private:
	LPD3DXMESH		m_pMesh;

	cAseLoader*		m_pAseLoader;

public:
	cRabitGirl();
	~cRabitGirl();

	void Setup();
	void Update();
	void Render();
};