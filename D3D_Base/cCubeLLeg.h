#pragma once
#include "cCubeNode.h"
class cCubeLLeg :
	public cCubeNode
{
public:
	cCubeLLeg();
	~cCubeLLeg();

	virtual void Setup() override;
};

