#pragma once

class cPyramid;

class cGrid
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;
	std::vector<cPyramid*>		m_vecPyramid;

public:
	cGrid();
	~cGrid();

	void Setup(int nNumHalfTile = 15, float fInterval = 1.0f);
	void Render();
};

