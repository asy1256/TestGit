#pragma once

typedef D3DXVECTOR3 Point;

typedef struct Line
{
	Point start;
	Point end;

	inline Line() {}
	inline Line(const Point& s, const Point& e)
		: start(s)
		, end(e)
	{
	}
};

typedef struct AABB
{
	Point origin;
	D3DXVECTOR3 size;

	inline AABB() : size(1, 1, 1) { }
	inline AABB(const D3DXVECTOR3& o, const D3DXVECTOR3& s) : origin(o), size(s) { }
} AABB;

class Geometry3D
{
public:
	Geometry3D();
	~Geometry3D();


	float Length(const Line& line);
	float LengthSq(const Line& line);
	D3DXVECTOR3 GetMin(const AABB& aabb);
	D3DXVECTOR3 GetMax(const AABB& aabb);
	AABB FromMinMax(const D3DXVECTOR3& min, const D3DXVECTOR3& max);
};

