#include "stdafx.h"
#include "Geometry3D.h"

#include <cmath>
#include <cfloat>

Geometry3D::Geometry3D()
{
}

Geometry3D::~Geometry3D()
{
}

D3DXVECTOR3 Geometry3D::GetMin(const AABB & aabb)
{
	D3DXVECTOR3 p1 = aabb.origin + aabb.size;
	D3DXVECTOR3 p2 = aabb.origin - aabb.size;

	return D3DXVECTOR3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
}

D3DXVECTOR3 Geometry3D::GetMax(const AABB & aabb)
{
	D3DXVECTOR3 p1 = aabb.origin + aabb.size;
	D3DXVECTOR3 p2 = aabb.origin - aabb.size;

	return D3DXVECTOR3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
}

AABB Geometry3D::FromMinMax(const D3DXVECTOR3 & min, const D3DXVECTOR3 & max)
{
	return AABB((min + max) * 0.5f, (max - min) * 0.5f);
}
