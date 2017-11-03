#include "stdafx.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "cMtlTex.h"


cObjLoader::cObjLoader()
{
}


cObjLoader::~cObjLoader()
{
}

void cObjLoader::Load( OUT std::vector<cGroup*>& vecGroup,
	IN char* szFolder, IN char* szFile )
{
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<ST_PNT_VERTEX> vecVertex;

	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	while (true)
	{
		if (feof(fp))
		{
			break;
		}

		char szBuf[1024];
		fgets(szBuf, 1024, fp);

		if ( szBuf[0] == '#' )
		{
			continue;
		}
		else if ( szBuf[0] == 'm' )
		{
			char szMtlFile[1024];
			sscanf_s( szBuf, "%*s %s", szMtlFile, 1024 );
			// 메테리얼 정보 로드
			LoadMtlLib(szFolder, szMtlFile);
		}
		else if ( szBuf[0] == 'g' )
		{
			if ( !vecVertex.empty() )
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetVertex( vecVertex );
				vecGroup.push_back( pGroup );
				vecVertex.clear();
			}
		}
		else if ( szBuf[0] == 'v' )
		{
			if ( szBuf[1] == ' ' )	// 로컬 버텍스 좌표
			{
				float x, y, z;
				sscanf_s( szBuf, "%*s %f %f %f", &x, &y, &z );
				vecV.push_back( D3DXVECTOR3( x, y, z ) );
			}
			else if ( szBuf[1] == 't' ) // UV 좌표
			{
				float u, v;
				sscanf_s( szBuf, "%*s %f %f %*f", &u, &v );
				vecVT.push_back( D3DXVECTOR2( u, v ) );
			}
			else if ( szBuf[1] == 'n' )	// 노말정보
			{
				float x, y, z;
				sscanf_s( szBuf, "%*s %f %f %f", &x, &y, &z );
				vecVN.push_back( D3DXVECTOR3( x, y, z ) );
			}
		}
		else if ( szBuf[0] == 'u' )
		{

		}
		else if ( szBuf[0] == 'f' )
		{
			int nIndex[3][3];
			sscanf_s( szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2] );

			for ( int i = 0; i < 3; i++ )
			{
				ST_PNT_VERTEX v;
				v.p = vecV[nIndex[i][0] - 1];
				v.t = vecVT[nIndex[i][1] - 1];
				v.n = vecVN[nIndex[i][2] - 1];
				vecVertex.push_back( v );
			}
		}
	}

	fclose(fp);
}

void cObjLoader::LoadMtlLib( IN char * szFolder, IN char * szMtlFile )
{
	std::string sFullPath( szFolder );
	sFullPath += ( std::string( "/" ) + std::string( szMtlFile ) );

	FILE* fp;
	fopen_s( &fp, sFullPath.c_str(), "r" );

	std::string sMtlName;

	while ( true )
	{
		if ( feof( fp ) )
		{
			break;
		}

		char szBuf[1024];
		fgets( szBuf, 1024, fp );

		if ( szBuf[0] == '#' )
		{
			continue;
		}
		else if ( szBuf[0] == 'n' )
		{
			char szMtlName[1024];
			sscanf_s( szBuf, "%*s %s", szMtlName, 1024 );
			sMtlName = std::string( szMtlName );
			if ( m_mapMtlTex.find( sMtlName ) == m_mapMtlTex.end() )
			{
				m_mapMtlTex[sMtlName] = new cMtlTex;
			}
		}
		else if ( szBuf[0] == 'K' )
		{
			if ( szBuf[1] == 'a' )
			{
				float r, g, b;
				sscanf_s( szBuf, "%*s %f %f %f", &r, &g, &b );
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.r = r;
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.g = g;
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.b = b;
				m_mapMtlTex[sMtlName]->GetMaterial().Ambient.a = 1.0f;
			}
			else if ( szBuf[1] == 'd' )
			{
				float r, g, b;
				sscanf_s( szBuf, "%*s %f %f %f", &r, &g, &b );
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.r = r;
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.g = g;
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.b = b;
				m_mapMtlTex[sMtlName]->GetMaterial().Diffuse.a = 1.0f;
			}
			else if ( szBuf[1] == 's' )
			{
				float r, g, b;
				sscanf_s( szBuf, "%*s %f %f %f", &r, &g, &b );
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.r = r;
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.g = g;
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.b = b;
				m_mapMtlTex[sMtlName]->GetMaterial().Specular.a = 1.0f;
			}
		}
		else if ( szBuf[0] == 'd' )
		{
			//float d;
			//sscanf_s( szBuf, "%*s %f", &d );
			//m_map
		}
		else if ( szBuf[0] == 'N' )
		{
			if ( szBuf[1] == 's' )
			{
				float Ns;
				sscanf_s( szBuf, "%*s %f", &Ns );
				m_mapMtlTex[sMtlName]->GetMaterial().Power = Ns;
			}
		}
		//else if ( szBuf[0] == 'm' )
		//{
		//	char szTexFile[1024];
		//	sscanf_s( szBuf, "%*s %s", szTexFile, 1024 );
		//	std::string sTexFullPath = std::string( szFolder );
		//	sTexFullPath += std::string( "/" ) + std::string( szTexFile );

		//	LPDIRECT3DTEXTURE9 pTexture = //
		//	m_mapMtlTex[sMtlName]->SetTexture( pTexture );
		//}
	}

	fclose( fp );
}

void cObjLoader::CreatMeshFromFile(OUT LPD3DXMESH& mesh, IN char * szFolder, IN char * szFile)
{
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<ST_PNT_VERTEX> vecVertex;

	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	while (true)
	{
		if (feof(fp))
		{
			break;
		}

		char szBuf[1024];
		fgets(szBuf, 1024, fp);

		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'm')
		{
			char szMtlFile[1024];
			sscanf_s(szBuf, "%*s %s", szMtlFile, 1024);
			// 메테리얼 정보 로드
			LoadMtlLib(szFolder, szMtlFile);
		}
		else if (szBuf[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				//cGroup* pGroup = new cGroup;
				//pGroup->SetVertex(vecVertex);
				//vecGroup.push_back(pGroup);
				//vecVertex.clear();
			}
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == ' ')	// 로컬 버텍스 좌표
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szBuf[1] == 't') // UV 좌표
			{
				float u, v;
				sscanf_s(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szBuf[1] == 'n')	// 노말정보
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{

		}
		else if (szBuf[0] == 'f')
		{
			int nIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]);

			for (int i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[nIndex[i][0] - 1];
				v.t = vecVT[nIndex[i][1] - 1];
				v.n = vecVN[nIndex[i][2] - 1];
				vecVertex.push_back(v);
			}
		}
	}

	D3DXCreateMeshFVF(vecVertex.size() / 3, vecVertex.size(), 0, ST_PNT_VERTEX::FVF, g_pD3DDevice, &mesh);
	ST_PNT_VERTEX* tV;
	WORD* tW;
	DWORD* tDW;
	D3DXMATRIXA16 matS, matR, matWorld;
	D3DXMatrixScaling(&matS, 0.02f, 0.02f, 0.02f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	matWorld = matS * matR;

	mesh->LockVertexBuffer(0, (void**)&tV);

	for (int i = 0; i < vecVertex.size(); ++i)
	{
		tV[i] = vecVertex[i];
		D3DXVec3TransformCoord(&tV[i].p, &tV[i].p, &matWorld);
	}

	mesh->UnlockVertexBuffer();

	mesh->LockIndexBuffer(0, (void**)&tW);

	for (int i = 0; i < vecVertex.size(); ++i)
	{
		tW[i] = i;
	}

	mesh->UnlockIndexBuffer();

	mesh->LockAttributeBuffer(0, &tDW);

	for (int i = 0; i < vecVertex.size() / 3; ++i)
	{
		tDW[i] = 0;
	}

	mesh->UnlockAttributeBuffer();

	DWORD* adjInfo = new DWORD[mesh->GetNumFaces() * 3];
	DWORD* adjoptInfo = new DWORD[mesh->GetNumFaces() * 3];
	mesh->GenerateAdjacency(0.0f, adjInfo);

	HRESULT ok = S_FALSE;

	ok = mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, adjInfo, adjoptInfo, 0, 0);

	if (ok == S_OK)
	{
		int a = 0;
	}
	else
	{
		int b = 0;
	}

	delete[] adjInfo;
	delete[] adjoptInfo;

	fclose(fp);
}
