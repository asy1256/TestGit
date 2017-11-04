#include "stdafx.h"
#include "cGeometry3D.h"
#include <cmath>
#include <cfloat>

cGeometry3D::cGeometry3D()
{
}

cGeometry3D::~cGeometry3D()
{
}

// sq 는 제곱값을 반환해줌
// 길이
float cGeometry3D::Length(IN  Line & line)
{
	// 점과 점 사이의 거리
	return D3DXVec3Length(&(line.Start - line.End));
}

float cGeometry3D::LengthSq(IN  Line & line)
{
	// 점과 점 사이의 거리 제곱
	return D3DXVec3LengthSq(&(line.Start - line.End));
}

// 각
float cGeometry3D::Magnitude(IN  D3DXVECTOR3 & v)
{
	// 각
	return sqrtf(D3DXVec3Dot(&v, &v));
}

float cGeometry3D::MagnitudeSq(IN  D3DXVECTOR3 & v)
{
	// 각의 제곱
	return D3DXVec3Dot(&v, &v);
}

// 점
Ray cGeometry3D::FromPoints(IN  D3DXVECTOR3 & from, IN  D3DXVECTOR3 & to)
{
	D3DXVECTOR3 out;
	// 정규화(크기는 없애고 방향만 남기는 작업)
	D3DXVec3Normalize(&out, &(to - from));
	return Ray(from, out);
}

// AABB
D3DXVECTOR3 cGeometry3D::GetMin(IN  AABB & aabb)
{
	D3DXVECTOR3 p1 = aabb.Origin + aabb.Size;
	D3DXVECTOR3 p2 = aabb.Origin - aabb.Size;

	return D3DXVECTOR3(fminf(p1.x,p2.x), fminf(p1.y, p2.y), fminf(p1.z,p2.z));
}

D3DXVECTOR3 cGeometry3D::GetMax(IN  AABB & aabb)
{
	D3DXVECTOR3 p1 = aabb.Origin + aabb.Size;
	D3DXVECTOR3 p2 = aabb.Origin - aabb.Size;

	return D3DXVECTOR3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
}

AABB cGeometry3D::FromMinMax(IN  D3DXVECTOR3 & min, IN  D3DXVECTOR3 & max)
{
	// 해당 물체의 최대위치와 최소위치를 구한다.
	return AABB((min + max) * 0.5f, (max - min) * 0.5f);
}

// Plane
float cGeometry3D::PlaneEquation(IN  D3DXVECTOR3 & pt, IN  Plane & plane)
{
	// 내적을 구해 각을 알아낸다.
	return D3DXVec3Dot(&pt, &plane.Normal);
}

// point test
// point in sphere
bool cGeometry3D::PointInSphere(IN  D3DXVECTOR3 & point, IN  Sphere & sphere)
{
	// 점과 점 사이의 거리 제곱을 찾음
	float magSq = D3DXVec3LengthSq(&(point - sphere.Position));
	// 반지름의 제곱
	float radSq = sphere.Radius * sphere.Radius;
	// 점과 점 사이의 거리 제곱이 반지름 제곱보다 작으면 충돌
	return magSq < radSq;
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Sphere & sphere, IN  D3DXVECTOR3 & point)
{
	// 점과 구의 포지션의 거리를 구해서 저장한다.
	D3DXVECTOR3 SphereToPoint = point - sphere.Position;
	// 노멀라이즈를 사용해 크기는 없애고 방향만 남겨놓는다.
	D3DXVec3Normalize(&SphereToPoint, &SphereToPoint);
	// 방향 * 반지름의 곱을 담는다. 
	SphereToPoint = SphereToPoint * sphere.Radius;
	return SphereToPoint + sphere.Position;
}

// point in aabb
bool cGeometry3D::PointInAABB(IN  D3DXVECTOR3 & point, IN  AABB & aabb)
{
	// 충돌검사할 aabb의 min point 과 max point를 구한다.
	D3DXVECTOR3 Min = GetMin(aabb);
	D3DXVECTOR3 Max = GetMax(aabb);

	// aabb 와 검사할 point의 위치가 aabb in이 안된다면 false
	if (point.x < Min.x || point.y < Min.y || point.z < Min.z) { return false; }
	if (point.x > Max.x || point.y > Max.y || point.z > Max.z) { return false; }

	// aabb와 검사할 point의 위치가 aabb in이 된다면 true
	return true;
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  AABB & aabb, IN  D3DXVECTOR3 & point)
{
	D3DXVECTOR3 Result = point;		// 반환될 녀석
	D3DXVECTOR3 Min = GetMin(aabb); 
	D3DXVECTOR3 Max = GetMax(aabb);

	// point가 aabb안으로 들어가면 aabb위치값 , 안들어갔으면 result값을 반환해서 
	// 구의 중심과 가장 가까운 거리의 점을 찾음.
	Result.x = (Result.x < Min.x) ? Min.x : Result.x;
	Result.y = (Result.y < Min.y) ? Min.y : Result.y;
	Result.z = (Result.z < Min.z) ? Min.z : Result.z;

	Result.x = (Result.x > Max.x) ? Max.x : Result.x;
	Result.y = (Result.y > Max.y) ? Max.y : Result.y;
	Result.z = (Result.z > Max.z) ? Max.z : Result.z;

	return Result;
}

// point in obb
bool cGeometry3D::PointInOBB(IN  D3DXVECTOR3 & point, IN  OBB & obb)
{
	// 점에서 상자의 위치를 빼서 방향을 구함.
	D3DXVECTOR3 Dir = point - obb.Position;

	// for문을 3번 돌린다. (x축, y축, z축)
	// OBB 상자의 로컬축위에 점을 투영하고 해당 축의 OBB상자에 범위까지의 거리를 비교함
	for (int i = 0; i < 3; ++i)
	{
		// XYZ 축을 나타낼 벡터를 만듬.
		 float* Orientation = &obb.Orientation[i * 4]; 
		D3DXVECTOR3	Axis
		(
			Orientation[0],
			Orientation[1],
			Orientation[2]
		);

		// 구한 방향과 각 축에 점을 투영해서 얻어낸 값을 내적해서 OBB상자가 점에서 얼마나 떨어져있는지 구함.
		// normalize 후에 내적하면 각을 구할 수 있고, normalize하지않고 내적하면 곱이 된다.
		float Distance = D3DXVec3Dot(&Dir, &Axis);

		// Distance가 1보다 크거나 작을경우 점이 OBB상자 안에 없다. 측 충돌이 아니다.
		if (Distance > obb.Size[i]) { return false; }
		if (Distance < -obb.Size[i]) { return false; }
	}

	return true;
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  OBB & obb, IN  D3DXVECTOR3 & point)
{
	// point 에서 obb의 중점으로의 방향
	D3DXVECTOR3	Result = obb.Position;
	D3DXVECTOR3 Dir = point - obb.Position;

	// for문을 3번 돌린다. (x축, y축, z축)
	for (int i = 0; i < 3; ++i)
	{
		// XYZ 축을 나타낼 벡터를 만듬.
		 float* Orientation = &obb.Orientation[i * 4]/*.asArray*/;

		D3DXVECTOR3	Axis
		(
			Orientation[0],
			Orientation[1],
			Orientation[2]
		);

		// 구한 방향과 각 축에 점을 투영해서 얻어낸 값을 내적해서 OBB상자가 점에서 얼마나 떨어져있는지 구함.
		// normalize 후에 내적하면 각을 구할 수 있고, normalize하지않고 내적하면 곱이 된다.
		float Distance = D3DXVec3Dot(&Dir, &Axis);

		// Distance가 1보다 크거나 작을때 점을 조정해준다.
		if (Distance > obb.Size[i]) { Distance = obb.Size[i]; }
		if (Distance < -obb.Size[i]) { Distance = -obb.Size[i]; }

		Result = Result + (Axis * Distance);
	}
	return Result;
}

// point in plane
bool cGeometry3D::PointOnPlane(IN  D3DXVECTOR3 & point, IN  Plane & plane)
{
	float Dot = D3DXVec3Dot(&point, &plane.Normal);
	return Dot - plane.Distance == 0.0f; // 점이 평면의 표면에 있는지 확인 한다.
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Plane & plane, IN  D3DXVECTOR3 & point)
{
	float Dot = D3DXVec3Dot(&plane.Normal,&point);
	float Distance = Dot - plane.Distance;
	return point - plane.Normal * Distance;
}

// point on line
bool cGeometry3D::PointOnLine(IN  D3DXVECTOR3 & point, IN  Line & line)
{
	// 점에서 가까운 선의 한 점을 구한다.
	D3DXVECTOR3 Closest = ClosestPoint(line, point);
	float		DistanceSq = MagnitudeSq(Closest - point);
	return DistanceSq == 0.0f; // 점이 선의 위에 있는지 확인한다.
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Line & line, IN  D3DXVECTOR3 & point)
{
	D3DXVECTOR3	LVec = line.End - line.Start; // Line Vector

	float t = D3DXVec3Dot(&(point - line.Start), &(LVec / D3DXVec3Dot(&LVec, &LVec))); 
	t = fmaxf(t, 0.0f);	// Clamp to 0
	t = fmaxf(t, 1.0f);	// Clamp to 1

	return line.Start + LVec * t;
}

// point on ray
bool cGeometry3D::PointOnRay(IN  D3DXVECTOR3 & point, IN  Ray & ray)
{
	if (point == ray.Origin) { return true; }

	// A - B = B에서 A로 가는 방향이 나온다.
	D3DXVECTOR3 Norm = point - ray.Origin;

	// 크기는 필요없으니까 방향만 남겨놓는다.
	D3DXVec3Normalize(&Norm, &Norm);

	float Diff = D3DXVec3Dot(&Norm, &ray.Direction);

	// 점에서 ray로 가는 방향과 ray의 방향이 같으면 1이 나오는데 그러면 충돌.
	return Diff == 1.0f; // Consider using epsilon!
}

D3DXVECTOR3 cGeometry3D::ClosestPoint(IN  Ray & ray, IN  D3DXVECTOR3 & point)
{
	float t = D3DXVec3Dot(&(point - ray.Origin), &ray.Direction);

	// x 실수, y 실수 <- 둘중 큰 녀석을 반환해줌.
	// t값을 양수로 받기위해 0과 비교해서 클 경우에 t를 받고 아닐경우 0을 받는다.
	t = fmaxf(t, 0.0f); 

	return D3DXVECTOR3(ray.Origin + ray.Direction * t);
}

// sphere test
// sphere to Sphere
bool cGeometry3D::SphereSphere(IN  Sphere & s1, IN  Sphere & s2)
{
	float radiisum = s1.Radius + s2.Radius; // 두 구의 반지름의 합을 구함
	float sqDistance = MagnitudeSq(s1.Position - s2.Position); // 두 구의 중점을 이은 선의 길이의 제곱을 구함

	return sqDistance < radiisum * radiisum; // 구한 길이의 제곱이 구한 반지름의 합보다 작으면 충돌임.
}

// sphere to aabb
bool cGeometry3D::SphereAABB(IN  Sphere & sphere, IN  AABB & aabb)
{
	// AABB박스에서 구까지 가장 가까운 점을 찾아서 저장.
	D3DXVECTOR3 closestPoint = ClosestPoint(aabb, sphere.Position);

	// sphere와 aabb의 거리의 제곱
	float distSq = MagnitudeSq(sphere.Position - closestPoint);
	// sphere의 반지름의 제곱
	float radiusSq = sphere.Radius * sphere.Radius;

	return distSq < radiusSq;
}

// sphere to obb
bool cGeometry3D::SphereOBB(IN  Sphere & sphere, IN  OBB & obb)
{
	// obb박스에서 sphere원점까지 가까운 거리에 있는 점을 구함.
	D3DXVECTOR3 closestPoint = ClosestPoint(obb, sphere.Position);

	float distSq = MagnitudeSq(sphere.Position - closestPoint); // 구한 점에서 sphere까지의 거리 제곱을 구함.
	float radiusSq = sphere.Radius * sphere.Radius; // sphere의 반지름의 제곱을 구함.

	return distSq < radiusSq; // 두개를 비교해서 반지름의 제곱보다 작으면 충돌함.
}

// sphere to plane
bool cGeometry3D::SpherePlane(IN  Sphere & sphere, IN  Plane & plane)
{ 
	// plane에서 sphere원점까지 가까운 거리에 있는 점을 구함.
	D3DXVECTOR3 closestPoint = ClosestPoint(plane, sphere.Position);

	float distSq = MagnitudeSq(sphere.Position - closestPoint); // 구한 점에서 sphere까지의 거리 제곱을 구함.
	float radiusSq = sphere.Radius * sphere.Radius; // sphere의 반지름의 제곱을 구함.

	return distSq < radiusSq; // 두개를 비교해서 반지름의 제곱보다 작으면 충돌함.
}

// aabb to aabb
bool cGeometry3D::AABBAABB(IN  AABB & aabb1, IN  AABB & aabb2)
{
	// first AABB의 최대 최소 포지션을 구함.
	D3DXVECTOR3 aMin = GetMin(aabb1);
	D3DXVECTOR3 aMax = GetMax(aabb1);

	// second AABB의 최대 최소 포지션을 구함.
	D3DXVECTOR3 bMin = GetMin(aabb2);
	D3DXVECTOR3 bMax = GetMax(aabb2);

	// 그림을 그려서 확인하면 편함.
	return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);
}

// aabb to obb
Interval cGeometry3D::GetInterval(IN  AABB & rect, IN  D3DXVECTOR3 & axis)
{
	// AABB의 최대 최소 포지션을 구함.
	D3DXVECTOR3 min = GetMin(rect);
	D3DXVECTOR3 max = GetMax(rect);

	// AABB 박스의 vertex 포지션을 구함.
	D3DXVECTOR3 vertex[8] =
	{
		D3DXVECTOR3(min.x, max.y, max.z),
		D3DXVECTOR3(min.x, max.y, min.z),
		D3DXVECTOR3(min.x, min.y, max.z),
		D3DXVECTOR3(min.x, min.y, min.z),
		D3DXVECTOR3(max.x, max.y, max.z),
		D3DXVECTOR3(max.x, max.y, min.z),
		D3DXVECTOR3(max.x, min.y, max.z),
		D3DXVECTOR3(max.x, min.y, min.z)
	};

	// 최대 최소를 float 형으로 가지고 있는 구조체
	Interval result;

	result.min = result.max = D3DXVec3Dot(&axis, &vertex[0]);

	for (int i = 1; i < 8; ++i)
	{
		// 버텍스에서 축까지의 길이를 구함.
		float projection = D3DXVec3Dot(&axis, &vertex[i]);

		// AABB박스의 최대 최소 버텍스의 포지션을 구해줌.
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}

	return result;
}

Interval cGeometry3D::GetInterval(IN  OBB & rect, IN  D3DXVECTOR3 & axis)
{
	D3DXVECTOR3	vertex[8];

	// obb center 중점
	D3DXVECTOR3 C = rect.Position;
	// obb extent 넓이
	D3DXVECTOR3 E = rect.Size;

	 float* o = rect.Orientation;
	//rect.Orientation._11;
	// obb axis 축
	// 원래 3*3 행렬짜리를 사용해야 하지만 지원안해주기때문에 4*4행렬을 사용.
	// 0   1   2   3                                   0    1    2
	// 4   5   6   7                                   4    5    6
	// 8   9  10  11                                   8    9   10  << 이것들만 사용함.
    //12  13  14  15 << 3*3 행렬을 사용해야하기떄문에    
	// 회전에 관하여 사용하기때문에 행렬의 12~15는 이동 관련이라 제외.
	// 행렬의 3 7 11 15는 가중치라 제외. xyz를 담당하는 위 3*3로 사용하는것임.
	D3DXVECTOR3 A[] = 
	{
		D3DXVECTOR3(o[0],o[1],o[2]),
		D3DXVECTOR3(o[4],o[5],o[6]),
		D3DXVECTOR3(o[8],o[9],o[10])
	};

	// 버택스 좌표 = 중점 (+-) 축 * 넓이 (+-) 축 * 넓이 (+-) 축 * 넓이
	vertex[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2]; // +++
	vertex[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2]; // -++
	vertex[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2]; // +-+
	vertex[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2]; // ++-
	vertex[4] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2]; // --+
	vertex[5] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2]; // -+-
	vertex[6] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2]; // +--
	vertex[7] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2]; // ---

	rect.vecVertex.resize(8);

	for (int i = 0; i < rect.vecVertex.size(); ++i)
	{
		D3DXVECTOR3 temp = vertex[i];
		rect.vecVertex[i] = temp;
	}

	Interval result;
	result.min = result.max = D3DXVec3Dot(&axis, &vertex[0]);

	for (int i = 0; i < 8; ++i)
	{
		float projection = D3DXVec3Dot(&axis, &vertex[i]);
		result.min = (projection < result.min) ? projection : result.min;
		result.max = (projection > result.max) ? projection : result.max;
	}
		
	return result;
}

// 비교에 사용할 obb박스와 aabb박스를 임시로 만들어줌.
bool cGeometry3D::OverlapOnAxis(IN  AABB & aabb, IN  OBB & obb, IN  D3DXVECTOR3 & axis)
{
	Interval a = GetInterval(aabb, axis);
	Interval b = GetInterval(obb, axis);

	return ((b.min <= a.max) && (a.min <= b.max));
}

bool cGeometry3D::AABBOBB(IN  AABB & aabb, IN  OBB & obb)
{
	 float* o = obb.Orientation;

	// 3D상에서 물체간의 충돌을 확인하기위해서 최소 15개의 방향에서 체크를 해야하기 때문에
	// aabb박스의 축 xyz 3개와 obb박스의 축 3개. 그리고 그 둘을 외적해서 만든 벡터 9개로 검사를 한다.
	D3DXVECTOR3 test[15] = {
		D3DXVECTOR3(1,0,0), // AABB axis 1
		D3DXVECTOR3(0,1,0), // AABB axis 2
		D3DXVECTOR3(0,0,1), // AABB aixs 3
		D3DXVECTOR3(o[0], o[1], o[2]), // OBB axis 1
		D3DXVECTOR3(o[4], o[5], o[6]), // OBB axis 2
		D3DXVECTOR3(o[8], o[9], o[10]) // OBB axis 3
	};

	for (int i = 0; i < 3; ++i) // aabb박스의 축3개와 obb박스의 축3개로 외적해서 test[6]부터 끝까지 추가해준다.
	{
		D3DXVec3Cross(&test[6 + i * 3 + 0], &test[i], &test[3]);
		D3DXVec3Cross(&test[6 + i * 3 + 1], &test[i], &test[4]);
		D3DXVec3Cross(&test[6 + i * 3 + 2], &test[i], &test[5]);
	}

	for (int i = 0; i < 15; ++i)
	{
		if (!OverlapOnAxis(aabb, obb, test[i])) { return false;	}// seperating aixs found 
	}
		return true; // seperating aixs not  found;
}

// aabb to plane
bool cGeometry3D::AABBPlane(IN  AABB & aabb, IN  Plane & plane)
{
	float pLen = aabb.Size.x * fabsf(plane.Normal.x) +
		aabb.Size.y * fabsf(plane.Normal.y) +
		aabb.Size.z * fabsf(plane.Normal.z);

	float dot = D3DXVec3Dot(&plane.Normal, &aabb.Origin);
	float dist = dot - plane.Distance;

	return fabsf(dist) <= pLen;
}

// obb to obb
bool cGeometry3D::OverlapOnAxis(IN  OBB & obb1, IN  OBB & obb2, IN  D3DXVECTOR3 & axis)
{
	Interval a = GetInterval(obb1, axis);
	Interval b = GetInterval(obb2, axis);

	return ((b.min <= a.max) && (a.min <= b.max));
}

bool cGeometry3D::OBBOBB(IN  OBB & obb1, IN  OBB & obb2)
{
	 float* o1 = obb1.Orientation;
	 float* o2 = obb2.Orientation;

	D3DXVECTOR3 test[15] = {
		D3DXVECTOR3(o1[0],o1[1],o1[2]),
		D3DXVECTOR3(o1[4],o1[5],o1[6]),
		D3DXVECTOR3(o1[8],o1[9],o1[10]),

		D3DXVECTOR3(o2[0],o2[1],o2[2]),
		D3DXVECTOR3(o2[4],o2[5],o2[6]),
		D3DXVECTOR3(o2[8],o2[9],o2[10])
	};

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3Cross(&test[6 + i * 3 + 0], &test[i], &test[0]);
		D3DXVec3Cross(&test[6 + i * 3 + 1], &test[i], &test[1]);
		D3DXVec3Cross(&test[6 + i * 3 + 2], &test[i], &test[2]);
	}

	for (int i = 0; i < 15; ++i)
	{
		if (!OverlapOnAxis(obb1, obb2, test[i]))
		{
			return false; // seperating axis found
		}
	}

	return true; // seperating axis not found
}

// obb to plane
bool cGeometry3D::OBBPlane(IN  OBB & obb, IN  Plane & plane)
{
	// Local variables for readability only
	 float* o = obb.Orientation;
	D3DXVECTOR3 rot[] = {// rotation / orientation
		D3DXVECTOR3(o[0],o[1],o[2]),
		D3DXVECTOR3(o[4],o[5],o[6]),
		D3DXVECTOR3(o[8],o[9],o[10])
	};
	D3DXVECTOR3 normal = plane.Normal;

	float pLen =
		obb.Size.x * fabsf(D3DXVec3Dot(&normal, &rot[0])) +
		obb.Size.y * fabsf(D3DXVec3Dot(&normal, &rot[1])) +
		obb.Size.z * fabsf(D3DXVec3Dot(&normal, &rot[2]));

	float dist = D3DXVec3Dot(&plane.Normal, &obb.Position) - plane.Distance;

	return fabsf(dist) <= pLen;
}

// plane to plane
bool cGeometry3D::PlanePlane(IN  Plane & plane1, IN  Plane & plane2)
{
	// cross product returns 0 when used on parallel lines
	D3DXVECTOR3 d;
	D3DXVec3Cross(&d,&plane1.Normal, &plane2.Normal);

	return D3DXVec3Dot(&d, &d) != 0; // Consider using an epsilon;
}
