#include "stdafx.h"
#include "cRawLoader.h"

cRawLoader::cRawLoader()
{
}

cRawLoader::~cRawLoader()
{
}

void cRawLoader::Load(OUT LPD3DXMESH & mesh, IN char * szFolder, IN char * szFile)
{
	std::vector<ST_PNT_VERTEX> tempVecV;

	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	int whsize = sqrt(filesize);
	fseek(fp, 0, SEEK_SET);

	tempVecV.resize(filesize);
	int i = 0;
	int height = 0;

	for (int z = (whsize - 1) / 2; z > -(whsize - 1) / 2 - 1; --z)
	{
		int v = -z + (whsize - 1) / 2;
		for (int x = -(whsize - 1) / 2; x < (whsize - 1) / 2 + 1; ++x)
		{
			height = fgetc(fp);
			tempVecV[i].p = D3DXVECTOR3(x, height / 10.0f, z);
			D3DXVec3Normalize(&tempVecV[i].n, &tempVecV[i].p);
			int u = x + (whsize - 1) / 2;
			tempVecV[i].t = D3DXVECTOR2((float)u / (whsize - 1), (float)v / (whsize - 1));
			++i;
		}
	}

	/*for (int i = 0; i < tempVecV.size(); i += 3)
	{
		D3DXVECTOR3 u, v, n;
		u = tempVecV[i + 1].p - tempVecV[i].p;
		v = tempVecV[i + 2].p - tempVecV[i].p;

		D3DXVec3Cross(&n, &u, &v);
		D3DXVec3Normalize(&n, &n);

		tempVecV[i].n = n;
		tempVecV[i + 1].n = n;
		tempVecV[i + 2].n = n;
	}*/

	D3DXCreateMeshFVF(((whsize - 1) * (whsize - 1)) * 2, filesize, D3DXMESH_32BIT | D3DXMESH_MANAGED, ST_PNT_VERTEX::FVF, g_pD3DDevice, &mesh);

	ST_PNT_VERTEX* vtxbuffer;
	DWORD* idxbuffer;

	mesh->LockVertexBuffer(0, (void**)&vtxbuffer);

	for (int i = 0; i < tempVecV.size(); ++i)
	{
		vtxbuffer[i] = tempVecV[i];
	}

	mesh->UnlockVertexBuffer();

	mesh->LockIndexBuffer(0, (void**)&idxbuffer);

	for (int i = 0, j = 0; i < (whsize - 1) * (whsize - 1) * 2 * 3; i += 6, ++j)
	{
		if (j == filesize - whsize) { break; }
		if (j % whsize == whsize - 1) { continue; }

		idxbuffer[i] = j;
		idxbuffer[i + 1] = j + whsize + 1;
		idxbuffer[i + 2] = j + whsize;
		idxbuffer[i + 3] = j;
		idxbuffer[i + 4] = j + 1;
		idxbuffer[i + 5] = j + whsize + 1;
	}

	mesh->UnlockIndexBuffer();

	mesh->LockAttributeBuffer(0, &idxbuffer);

	for (int i = 0; i < (whsize - 1) * (whsize - 1) * 2; ++i)
	{
		idxbuffer[i] = 0;
	}

	mesh->UnlockAttributeBuffer();

	DWORD* adjInfo = new DWORD[mesh->GetNumFaces() * 3];
	DWORD* adjoptInfo = new DWORD[mesh->GetNumFaces() * 3];
	mesh->GenerateAdjacency(0.0f, adjInfo);

	HRESULT ok = S_FALSE;

	mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, adjInfo, adjoptInfo, 0, 0);

	delete[] adjInfo;
	delete[] adjoptInfo;
}
