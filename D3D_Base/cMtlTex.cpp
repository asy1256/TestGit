#include "stdafx.h"
#include "cMtlTex.h"


cMtlTex::cMtlTex()
	: m_pTexture(NULL)
{
	ZeroMemory( &m_mtl, sizeof( D3DMATERIAL9 ) );
}


cMtlTex::~cMtlTex()
{
	SAFE_RELEASE( m_pTexture );
}
