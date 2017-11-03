#pragma once

#include "cObject.h"

class cGroup : cObject
{
private:
	SYNTHESIZE_REF( std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex );

public:
	cGroup();
	virtual ~cGroup();

	void Render();
};

