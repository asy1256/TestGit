#include "stdafx.h"
#include "cGroup.h"


cGroup::cGroup()
{
}


cGroup::~cGroup()
{
}

void cGroup::Render()
{
	g_pD3DDevice->SetFVF( ST_PNT_VERTEX::FVF );
	g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof( ST_PNT_VERTEX ) );
}
