#pragma once

class cCubeMan;

class cInterpolation
{
private:
	std::vector<ST_PC_VERTEX>	m_vecLineVertex;

	cCubeMan*					m_pArrCubeMan[2];

public:
	cInterpolation();
	~cInterpolation();

	void Setup();
	void Update();
	void Render();
};

