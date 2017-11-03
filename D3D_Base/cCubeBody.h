#pragma once
#include "cCubeNode.h"
class cCubeBody :
	public cCubeNode
{
public:
	cCubeBody();
	~cCubeBody();

	virtual void Setup() override;
};

