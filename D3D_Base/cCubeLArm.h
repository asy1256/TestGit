#pragma once
#include "cCubeNode.h"
class cCubeLArm :
	public cCubeNode
{
public:
	cCubeLArm();
	~cCubeLArm();

	virtual void Setup() override;
};

