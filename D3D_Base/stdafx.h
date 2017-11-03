// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <mmsystem.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <vector>
#include <list>
#include <set>
#include <map>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3dx9.h>
#include "Asciitok.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

extern HWND g_hWnd;
extern POINT g_ptMouse;
extern LPPOINT g_pMouse;

#define SAFE_RELEASE(p) { if (p) p->Release(); p = NULL; }
#define SAFE_DELETE(p) { if (p) delete p; p = NULL; }

#define SINGLETONE(class_name) \
	private:	\
		class_name(void);	\
		~class_name(void);	\
	public:	\
		static class_name* GetInstance()	\
		{	\
			static class_name instance;	\
			return &instance;	\
		}

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(varType& var){ varName = var; }

#define SAFE_ADD_REF(p)	{ if (p) p->AddRef(); }
#define SYNTHESIZE_ADD_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){\
	if ( varName != var ) {\
		SAFE_ADD_REF( var ); \
		SAFE_RELEASE( varName ); \
		varName = var; \
	}\
}

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	D3DXVECTOR2 t;

	ST_PNT_VERTEX()
		: p(0, 0, 0), t(0, 0), n(0, 0, 0)
	{}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_NODE_TM
{
	D3DXMATRIXA16 matTm;
	D3DXVECTOR3 vecPos;
	D3DXQUATERNION qtnionRot;
	D3DXVECTOR3 vecScale;
	D3DXQUATERNION qtnionScale;

	ST_NODE_TM()
		: vecPos(0, 0, 0)
		, vecScale(0, 0, 0)
	{
		D3DXMatrixIdentity(&matTm);
		D3DXQuaternionIdentity(&qtnionRot);
		D3DXQuaternionIdentity(&qtnionScale);
	}
};

struct ST_ANI_ROT
{
	int frame;
	D3DXQUATERNION qtnion;

	ST_ANI_ROT()
		: frame(0)
	{
		D3DXQuaternionIdentity(&qtnion);
	}
};

struct ST_ANI_POS
{
	int frame;
	D3DXVECTOR3 position;
	ST_ANI_POS()
		: frame(0)
		, position(0, 0, 0)
	{}
};

struct ST_TNGLE_IDX
{
	WORD posIdx1, posIdx2, posIdx3;
	WORD uvIdx1, uvIdx2, uvIdx3;
	WORD nmlIdx1, nmlIdx2, nmlIdx3;
	ST_TNGLE_IDX()
		: posIdx1(0), posIdx2(0), posIdx3(0)
		, uvIdx1(0), uvIdx2(0), uvIdx3(0)
		, nmlIdx1(0), nmlIdx2(0), nmlIdx3(0)
	{}
};

#include "cObject.h"
#include "cGameObject.h"

#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cObjectManager.h"
#include "cTimeManager.h"
#include "cSceneManager.h"
#include "cKeyManager.h"