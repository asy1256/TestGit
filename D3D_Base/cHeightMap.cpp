#include "stdafx.h"
#include "cHeightMap.h"


cHeightMap::cHeightMap()
	: m_pMesh(NULL)
	, m_pTexture(NULL)
{
}


cHeightMap::~cHeightMap()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pMesh );
}

void cHeightMap::Setup( IN char * szFolder, IN char * szRaw, IN char * szTexture )
{
	std::string	sFolder( szFolder );
	std::string sRaw = sFolder + std::string( szRaw );
	std::string sTex = sFolder + std::string( szTexture);

	FILE* fp = NULL;
	fopen_s( &fp, sRaw.c_str(), "rb" );

	fseek( fp, 0, SEEK_END );
	int nFileSize = ftell( fp );
	int nRow = m_nRow = ( int ) ( sqrt( ( float ) nFileSize ) + 0.0001f );
	assert( nRow * nRow == nFileSize && "정방형 파일만 가능하다!" );
	int nNumTile = nRow - 1;

	fseek( fp, 0, SEEK_SET );

	std::vector<ST_PNT_VERTEX>	vecVertex;
	vecVertex.resize( nFileSize );
	m_vecVertex.resize(nFileSize);

	std::vector<DWORD>	vecIndex;
	vecIndex.resize( nNumTile * nNumTile * 2 * 3 );

	for ( int i = 0; i < nFileSize; i++ )
	{
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3( i % nRow ,
			(unsigned char)fgetc(fp) / 10.0f,
			i / nRow );
		v.n = D3DXVECTOR3( 0, 1, 0 );
		v.t = D3DXVECTOR2((i % nRow) / (float) nRow,
			( i / nRow ) / ( float ) nRow);
		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
	}

	fclose( fp );


	for ( int x = 0; x < nNumTile; x++ )
	{
		for ( int z = 0; z < nNumTile; z++ )
		{
			int _0 = ( z + 0 ) * nRow + ( x + 0 );
			int _1 = ( z + 1 ) * nRow + ( x + 0 );
			int _2 = ( z + 0 ) * nRow + ( x + 1 );
			int _3 = ( z + 1 ) * nRow + ( x + 1 );

			vecIndex.push_back( _0 );
			vecIndex.push_back( _1 );
			vecIndex.push_back( _2 );

			vecIndex.push_back( _3 );
			vecIndex.push_back( _2 );
			vecIndex.push_back( _1 );
		}
	}

	for ( int x = 1; x < nNumTile; x++ )
	{
		for ( int z = 1; z < nNumTile; z++ )
		{
			int nLeft	= ( z + 0 ) * nRow + ( x - 1 );
			int nRight	= ( z + 0 ) * nRow + ( x + 1 );
			int nUp		= ( z + 1 ) * nRow + ( x + 0 );
			int nDown	= ( z - 1 ) * nRow + ( x + 0 );

			D3DXVECTOR3	vLeftToRight = vecVertex[nRight].p - vecVertex[nLeft].p;
			D3DXVECTOR3	vDownToUp = vecVertex[nUp].p - vecVertex[nDown].p;
			D3DXVECTOR3	vNormal;
			D3DXVec3Cross( &vNormal, &vDownToUp, &vLeftToRight );
			D3DXVec3Normalize( &vNormal, &vNormal );

			int nIndex = z * nRow + x;
			vecVertex[nIndex].n = vNormal;
		}
	}

	D3DXCreateMeshFVF( vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh );

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer( 0, ( LPVOID* ) &pV );
	memcpy( pV, &vecVertex[0], vecVertex.size() * sizeof( ST_PNT_VERTEX ) );
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer( 0, ( LPVOID* ) &pI );
	memcpy( pI, &vecIndex[0], vecIndex.size() * sizeof( DWORD ) );
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer( 0, &pA );
	ZeroMemory( pA, ( vecIndex.size() / 3 ) * sizeof( DWORD ) );
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj( vecIndex.size() );
	m_pMesh->GenerateAdjacency( 0.0f, &vecAdj[0] );

	m_pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0 );

	ZeroMemory( &m_mtl, sizeof( D3DMATERIAL9 ) );
	m_mtl.Ambient = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	m_mtl.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	m_mtl.Specular = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

	m_pTexture = g_pTextureManager->GetTexture( sTex );
}

void cHeightMap::Render()
{
	D3DXMATRIXA16	matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
	g_pD3DDevice->SetMaterial( &m_mtl );
	g_pD3DDevice->SetTexture( 0, m_pTexture );
	m_pMesh->DrawSubset( 0 );
}

bool cHeightMap::GetHeight( IN float x, OUT float & y, IN float z )
{
	int nLDidx = x + ((int)z * m_nRow);
	int nLUidx = x + ((int)z * m_nRow) + m_nRow;
	int nRDidx = x + ((int)z * m_nRow) + 1;
	int nRUidx = x + ((int)z * m_nRow) + m_nRow + 1;

	float fXdist = x - (int)x;
	float fZdist = z - (int)z;

	D3DXVECTOR3 vecX, vecY, vecZ;
	
	if (fXdist + fZdist < 1.0f)
	{
		vecX = (m_vecVertex[nRDidx] - m_vecVertex[nLDidx]) * fXdist;
		vecZ = (m_vecVertex[nLUidx] - m_vecVertex[nLDidx]) * fZdist;
		vecY = vecX + vecZ;

		y = vecY.y + +m_vecVertex[nLDidx].y + 1.0f;
	}
	else
	{
		vecX = (m_vecVertex[nLUidx] - m_vecVertex[nRUidx]) * fXdist;
		vecZ = (m_vecVertex[nRDidx] - m_vecVertex[nRUidx]) * fZdist;
		vecY = vecX + vecZ;

		y = vecY.y + m_vecVertex[nRUidx].y + 1.0f;
	}

	return false;
}
