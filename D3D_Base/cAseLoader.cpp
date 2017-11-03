#include "stdafx.h"
#include "cAseLoader.h"

#include "cGroup.h"
#include "cMtlTex.h"
#include "cAseNode.h"

cAseLoader::cAseLoader()
	: m_nTverSize(0)
{
}

cAseLoader::~cAseLoader()
{
	for each (auto p in m_vecMtlTex)
	{
		SAFE_DELETE(p);
	}
}

cAseNode* cAseLoader::LoadFile(char * szFolder, char * szFile)
{
	std::string sFullPath(szFolder);
	sFullPath += (std::string("/") + std::string(szFile));

	char line[1024];
	char title[256];

	fopen_s(&m_fp, sFullPath.c_str(), "r");

	if (m_fp == NULL)
	{
		//ㄴㄴ
	}
	else
	{
		while (true)
		{
			if (feof(m_fp)) { break; }

			fgets(line, 1024, m_fp);

			sscanf_s(line, "%s", title, sizeof(title));

			if (_stricmp(title, ID_SCENE) == 0) { ReadScene(); }
			else if (_stricmp(title, ID_MATERIAL_LIST) == 0) { ReadMaterialList(); }
			else if (_stricmp(title, ID_GEOMETRY) == 0) { ReadGeometry(); }
		}
	}

	return m_mapNode.begin()->second;
}

void cAseLoader::ReadSkip()
{
	char line[1024];
	char title[256];
	
	int nLevel = 1;

	do
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, "}") == 0) { --nLevel; }

	} while (nLevel > 0);
}

void cAseLoader::ReadScene()
{
	char line[1024];
	char title[256];

	while (true)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_FIRSTFRAME) == 0)
		{
			sscanf_s(line, "%*s %d", &m_dwFirstFrame);
		}
		else if (_stricmp(title, ID_LASTFRAME) == 0)
		{
			sscanf_s(line, "%*s %d", &m_dwLastFrame);
		}
		else if (_stricmp(title, ID_FRAMESPEED) == 0)
		{
			sscanf_s(line, "%*s %d", &m_dwFrameSpeed);
		}
		else if (_stricmp(title, ID_TICKSPERFRAME) == 0)
		{
			sscanf_s(line, "%*s %d", &m_dwTicksPerFrame);
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadMaterialList()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_MATERIAL_COUNT) == 0)
		{
			int size = 0;
			sscanf_s(line, "%*s %d", &size);

			m_vecMtlTex.resize(size);

			for (int i = 0; i < size; ++i)
			{
				m_vecMtlTex[i] = new cMtlTex;
			}
		}
		else if (_stricmp(title, ID_MATERIAL) == 0)
		{
			int mtltexindex = 0;
			sscanf_s(line, "%*s %d", &mtltexindex);

			ReadMaterial(mtltexindex);
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadMaterial(int& index)
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_AMBIENT) == 0)
		{
			D3DCOLORVALUE tempc = {};
			tempc.a = 1.0f;

			sscanf_s(line, "%*s %f %f %f", &tempc.r, &tempc.g, &tempc.b);
			m_vecMtlTex[index]->GetMaterial().Ambient = tempc;
		}
		else if (_stricmp(title, ID_DIFFUSE) == 0)
		{
			D3DCOLORVALUE tempc = {};
			tempc.a = 1.0f;

			sscanf_s(line, "%*s %f %f %f", &tempc.r, &tempc.g, &tempc.b);
			m_vecMtlTex[index]->GetMaterial().Diffuse = tempc;
		}
		else if (_stricmp(title, ID_SPECULAR) == 0)
		{
			D3DCOLORVALUE tempc = {};
			tempc.a = 1.0f;

			sscanf_s(line, "%*s %f %f %f", &tempc.r, &tempc.g, &tempc.b);
			m_vecMtlTex[index]->GetMaterial().Specular = tempc;
		}
		else if (_stricmp(title, ID_MAP_DIFFUSE) == 0) { ReadMapDiffuse(index); }
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadMapDiffuse(int& index)
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_BITMAP) == 0)
		{
			m_strTex = (std::string)line;

			m_strTex.erase(m_strTex.end() - 1);
			m_strTex.erase(m_strTex.end() - 1);

			while (1)
			{
				if (m_strTex[0] != '/') { m_strTex.erase(m_strTex.begin()); }
				else { m_strTex.erase(m_strTex.begin()); break; }
			}

			LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(m_strTex);

			m_vecMtlTex[index]->SetTexture(pTexture);
		}
		if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadGeometry()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_NODE_NAME) == 0)
		{
			std::string str = (std::string) line;

			str.erase(str.size() - 2);

			while (1)
			{
				if (str[0] != '\"') { str.erase(str.begin()); }
				else { str.erase(str.begin()); break; }
			}
			
			m_strNodeName = str;

			if (m_mapNode.find(m_strNodeName) == m_mapNode.end())
			{
				m_mapNode[m_strNodeName] = new cAseNode;
			}
		}
		else if (_stricmp(title, ID_NODE_PARENT) == 0)
		{
			std::string str = (std::string) line;

			str.erase(str.size() - 2);

			while (1)
			{
				if (str[0] != '\"') { str.erase(str.begin()); }
				else { str.erase(str.begin()); break; }
			}

			cAseNode* child = m_mapNode.find(m_strNodeName)->second;

			m_strParentsNodeName = str;
			m_mapNode.find(m_strParentsNodeName)->second->AddChild(child);
		}
		else if (_stricmp(title, ID_NODE_TM) == 0) { ReadNodeTM(); }
		else if (_stricmp(title, ID_MESH) == 0) { ReadMesh(); }
		else if (_stricmp(title, ID_TM_ANIMATION) == 0) { ReadTmAnimation(); }
		else if (_stricmp(title, ID_MATERIAL_REF) == 0)
		{
			int mtltexindex = 0;
			sscanf_s(line, "%*s %d", &mtltexindex);
			m_mapNode[m_strNodeName]->GetVerBuff();
			//m_mapNode.find(m_strNodeName)->second->SetVecVertex(m_vecIndex);
			m_mapNode.find(m_strNodeName)->second->SetMtlTex(m_vecMtlTex[mtltexindex]);
			m_vecVertex.clear();
			m_vecTver.clear();
			m_vecIndex.clear();
			m_vecNomal.clear();
			m_vecVerBuff.clear();
			break;
		}
		else if (_stricmp(title, "}") == 0)
		{
			if (m_vecVerBuff.size() != 0)
			{
				//m_mapNode.find(m_strNodeName)->second->SetVecVertex(m_vecIndex);
				m_vecVertex.clear();
				m_vecTver.clear();
				m_vecIndex.clear();
				m_vecNomal.clear();
				m_vecVerBuff.clear();
			}
			break;
		}
	}
}

void cAseLoader::ReadMesh()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_MESH_NUMVERTEX) == 0)
		{
			int size = 0;
			sscanf_s(line, "%*s %d", &size);
			m_vecVertex.resize(size);
			m_vecNomal.resize(size);
		}
		else if (_stricmp(title, ID_MESH_NUMFACES) == 0)
		{
			int size = 0;
			sscanf_s(line, "%*s %d", &size);
			m_vecIndex.resize(size);
		}
		else if (_stricmp(title, ID_MESH_NUMTVERTEX) == 0)
		{
			sscanf_s(line, "%*s %d", &m_nTverSize);
			m_vecTver.resize(m_nTverSize);
		}
		else if (_stricmp(title, ID_MESH_VERTEX_LIST) == 0) { ReadVertexList(); }
		else if (_stricmp(title, ID_MESH_FACE_LIST) == 0) { ReadFaceList(); }
		else if (_stricmp(title, ID_MESH_TVERTLIST) == 0) { ReadTverList(); }
		else if (_stricmp(title, ID_MESH_TFACELIST) == 0) { ReadTfaceList(); }
		else if (_stricmp(title, ID_MESH_NORMALS) == 0) { ReadNomals(); }
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadNodeTM()
{
	char line[1024];
	char title[256];

	ST_NODE_TM Tm;
	float x, y, z, w;
	x = y = z = w = 0.0f;

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_TM_ROW0) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &Tm.matTm._11, &Tm.matTm._13, &Tm.matTm._12);
		}
		else if (_stricmp(title, ID_TM_ROW1) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &Tm.matTm._31, &Tm.matTm._33, &Tm.matTm._32);
		}
		else if (_stricmp(title, ID_TM_ROW2) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &Tm.matTm._21, &Tm.matTm._23, &Tm.matTm._22);
		}
		else if (_stricmp(title, ID_TM_ROW3) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &Tm.matTm._41, &Tm.matTm._43, &Tm.matTm._42);
		}
		else if (_stricmp(title, ID_TM_POS) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &Tm.vecPos.x, &Tm.vecPos.z, &Tm.vecPos.y);
		}
		else if (_stricmp(title, ID_TM_ROTAXIS) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &x, &z, &y);
		}
		else if (_stricmp(title, ID_TM_ROTANGLE) == 0)
		{
			sscanf_s(line, "%*s %f", &w);
			Tm.qtnionRot.x = sinf(w / 2.0f) * x;
			Tm.qtnionRot.y = sinf(w / 2.0f) * y;
			Tm.qtnionRot.z = sinf(w / 2.0f) * z;
			Tm.qtnionRot.w = cosf(w / 2.0f);
		}
		else if (_stricmp(title, ID_TM_SCALE) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &Tm.vecScale.x, &Tm.vecScale.z, &Tm.vecScale.y);
		}
		else if (_stricmp(title, ID_TM_SCALEAXIS) == 0)
		{
			sscanf_s(line, "%*s %f %f %f", &x, &z, &y);
		}
		else if (_stricmp(title, ID_TM_SCALEAXISANG) == 0)
		{
			sscanf_s(line, "%*s %f", &w);
			Tm.qtnionScale.x = sinf(w / 2.0f) * x;
			Tm.qtnionScale.y = sinf(w / 2.0f) * y;
			Tm.qtnionScale.z = sinf(w / 2.0f) * z;
			Tm.qtnionScale.w = cosf(w / 2.0f);
		}
		else if (_stricmp(title, "}") == 0)
		{
			m_mapNode[m_strNodeName]->SetNodeTm(Tm);
			break;
		}
	}
}

void cAseLoader::ReadVertexList()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));


		if (_stricmp(title, ID_MESH_VERTEX) == 0)
		{
			int index = 0;
			float pos[3] = {};
			sscanf_s(line, "%*s %d %f %f %f", &index, &pos[0], &pos[2], &pos[1]);
			m_vecVertex[index] = D3DXVECTOR3(pos[0], pos[1], pos[2]);
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadFaceList()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_MESH_FACE) == 0)
		{
			int vecindex = 0;
			int vertexindex[3] = {};

			sscanf_s(line, "%*s %d: %*s %d %*s %d %*s %d", &vecindex, &vertexindex[0], &vertexindex[2], &vertexindex[1]);
			m_vecIndex[vecindex].posIdx1 = vertexindex[0];
			m_vecIndex[vecindex].posIdx2 = vertexindex[1];
			m_vecIndex[vecindex].posIdx3 = vertexindex[2];
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadTverList()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_MESH_TVERT) == 0)
		{
			int index = 0;
			float u = 0, v = 0;
			sscanf_s(line, "%*s %d %f %f", &index, &u, &v);
			m_vecTver[index] = D3DXVECTOR2(u, (1.0f - v));
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadTfaceList()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_MESH_TFACE) == 0)
		{
			int vecindex = 0;
			int tverindex[3] = {};

			sscanf_s(line, "%*s %d %d %d %d", &vecindex, &tverindex[0], &tverindex[2], &tverindex[1]);
			m_vecIndex[vecindex].uvIdx1 = tverindex[0];
			m_vecIndex[vecindex].uvIdx2 = tverindex[1];
			m_vecIndex[vecindex].uvIdx3 = tverindex[2];
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadNomals()
{
	char line[1024];
	char title[256];

	int baseindex = 0;
	int nomalindex = 0;
	D3DXVECTOR3 nomal;

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_MESH_FACENORMAL) == 0)
		{
			sscanf_s(line, "%*s %d", &baseindex);
		}
		else if (_stricmp(title, ID_MESH_VERTEXNORMAL) == 0)
		{
			int nmalIdx = 0;
			sscanf_s(line, "%*s %d %f %f %f", &nmalIdx, &nomal.x, &nomal.z, &nomal.y);

			m_vecNomal[nmalIdx] = nomal;

			if (nomalindex == 1) { m_vecIndex[baseindex].nmlIdx3 = nmalIdx; }
			else if (nomalindex == 2) { m_vecIndex[baseindex].nmlIdx2 = nmalIdx; }
			else { m_vecIndex[baseindex].nmlIdx1 = nmalIdx; }
			++nomalindex;
			if (nomalindex == 3) { nomalindex = 0; }
		}
		else if (_stricmp(title, "}") == 0)
		{
			IDirect3DVertexBuffer9* tempBuff;
			ST_PNT_VERTEX* tempV;
			IDirect3DIndexBuffer9* tempIdx;
			WORD* tempW;
			//노말까지 다 읽고나면 현재오브젝트의 uv벡터 사이즈 확인하고
			if (m_nTverSize == 0)
			{
				//사이즈가 0이면 텍스쳐를 사용하지 않는거니까 위치벡터의 사이즈로 버퍼벡터를 리사이즈
				m_vecVerBuff.resize(m_vecVertex.size());
				//인덱스버퍼 생성
				g_pD3DDevice->CreateIndexBuffer((m_vecIndex.size() * 3) * sizeof(WORD), 0,
					D3DFMT_INDEX16, D3DPOOL_MANAGED, &tempIdx, 0);
				tempIdx->Lock(0, 0, (void**)&tempW, 0);

				//면(폴리곤)만큼 만들어낸 인덱스구조체 벡터를 사이즈만큼 돌린다
				for (int i = 0; i < m_vecIndex.size(); ++i)
				{
					//uv벡터가 없기때문에 인덱스벡터의 위치인덱스를 기반으로 버퍼벡터에 넣어준다
					m_vecVerBuff[m_vecIndex[i].posIdx1].p = m_vecVertex[m_vecIndex[i].posIdx1];
					m_vecVerBuff[m_vecIndex[i].posIdx1].n = m_vecNomal[m_vecIndex[i].nmlIdx1];
					m_vecVerBuff[m_vecIndex[i].posIdx2].p = m_vecVertex[m_vecIndex[i].posIdx2];
					m_vecVerBuff[m_vecIndex[i].posIdx2].n = m_vecNomal[m_vecIndex[i].nmlIdx2];
					m_vecVerBuff[m_vecIndex[i].posIdx3].p = m_vecVertex[m_vecIndex[i].posIdx3];
					m_vecVerBuff[m_vecIndex[i].posIdx3].n = m_vecNomal[m_vecIndex[i].nmlIdx3];
					//인덱스버퍼에 순서 옮겨담기
					tempW[i * 3] = m_vecIndex[i].posIdx1;
					tempW[i * 3 + 1] = m_vecIndex[i].posIdx2;
					tempW[i * 3 + 2] = m_vecIndex[i].posIdx3;
				}

				tempIdx->Unlock();
				//버텍스버퍼 생성
				g_pD3DDevice->CreateVertexBuffer(m_vecVerBuff.size() * sizeof(ST_PNT_VERTEX),
					0, ST_PNT_VERTEX::FVF, D3DPOOL_MANAGED, &tempBuff, 0);
				tempBuff->Lock(0, 0, (void**)&tempV, 0);
				//버텍스버퍼에 값 옮겨담기
				for (int i = 0; i < m_vecVerBuff.size(); ++i)
				{
					tempV[i] = m_vecVerBuff[i];
				}

				tempBuff->Unlock();
			}
			else
			{
				//사이즈가 0이 아니면 텍스쳐를 사용하니 사이즈만큼 버퍼벡터를 리사이즈
				m_vecVerBuff.resize(m_nTverSize);
				//인덱스버퍼 생성
				g_pD3DDevice->CreateIndexBuffer((m_vecIndex.size() * 3) * sizeof(WORD), 0,
					D3DFMT_INDEX16, D3DPOOL_MANAGED, &tempIdx, 0);
				tempIdx->Lock(0, 0, (void**)&tempW, 0);
				//면(폴리곤)만큼 만들어낸 인덱스구조체 벡터를 사이즈만큼 돌린다
				for (int i = 0; i < m_vecIndex.size(); ++i)
				{
					//uv벡터가 있으니 인덱스벡터의 uv인덱스를 기반으로 버퍼벡터에 넣어준다
					m_vecVerBuff[m_vecIndex[i].uvIdx1].p = m_vecVertex[m_vecIndex[i].posIdx1];
					m_vecVerBuff[m_vecIndex[i].uvIdx1].t = m_vecTver[m_vecIndex[i].uvIdx1];
					m_vecVerBuff[m_vecIndex[i].uvIdx1].n = m_vecNomal[m_vecIndex[i].nmlIdx1];
					m_vecVerBuff[m_vecIndex[i].uvIdx2].p = m_vecVertex[m_vecIndex[i].posIdx2];
					m_vecVerBuff[m_vecIndex[i].uvIdx2].t = m_vecTver[m_vecIndex[i].uvIdx2];
					m_vecVerBuff[m_vecIndex[i].uvIdx2].n = m_vecNomal[m_vecIndex[i].nmlIdx2];
					m_vecVerBuff[m_vecIndex[i].uvIdx3].p = m_vecVertex[m_vecIndex[i].posIdx3];
					m_vecVerBuff[m_vecIndex[i].uvIdx3].t = m_vecTver[m_vecIndex[i].uvIdx3];
					m_vecVerBuff[m_vecIndex[i].uvIdx3].n = m_vecNomal[m_vecIndex[i].nmlIdx3];
					//인덱스버퍼에 순서 옮겨담기
					tempW[i * 3] = m_vecIndex[i].uvIdx1;
					tempW[i * 3 + 1] = m_vecIndex[i].uvIdx2;
					tempW[i * 3 + 2] = m_vecIndex[i].uvIdx3;
				}

				tempIdx->Unlock();
				//버텍스버퍼 생성
				g_pD3DDevice->CreateVertexBuffer(m_vecVerBuff.size() * sizeof(ST_PNT_VERTEX), 0, ST_PNT_VERTEX::FVF,
					D3DPOOL_MANAGED, &tempBuff, 0);
				tempBuff->Lock(0, 0, (void**)&tempV, 0);
				//버텍스버퍼에 값 옮겨담기
				for (int i = 0; i < m_vecVerBuff.size(); ++i)
				{
					tempV[i] = m_vecVerBuff[i];
				}

				tempBuff->Unlock();
			}
			m_mapNode[m_strNodeName]->SetVerBuff(tempBuff);
			m_mapNode[m_strNodeName]->SetIdxBuff(tempIdx);
			break;
		}
	}
}

void cAseLoader::ReadTmAnimation()
{
	char line[1024];
	char title[256];

	std::vector<ST_ANI_ROT> anirot;
	std::vector<ST_ANI_POS> anipos;

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_POS_TRACK) == 0) { ReadCtrlPosTrack(anipos); }
		else if (_stricmp(title, ID_ROT_TRACK) == 0) { ReadCtrlRotTrack(anirot); }
		else if (_stricmp(title, ID_CONTROL_ROT_TCB) == 0) { ReadCtrlRotTcb(); }
		else if (_stricmp(title, "}") == 0)
		{
			m_mapNode[m_strNodeName]->SetAniPos(anipos);
			m_mapNode[m_strNodeName]->SetAniRot(anirot);
			break;
		}
	}
}

void cAseLoader::ReadCtrlPosTrack(std::vector<ST_ANI_POS>& vecpos)
{
	char line[1024];
	char title[256];

	ST_ANI_POS tpos = {};

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_POS_SAMPLE) == 0)
		{
			sscanf_s(line, "%*s %d %f %f %f", &tpos.frame, &tpos.position.x, &tpos.position.z, &tpos.position.y);
			vecpos.push_back(tpos);
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadCtrlRotTrack(std::vector<ST_ANI_ROT>& vecrot)
{
	char line[1024];
	char title[256];

	ST_ANI_ROT trot = {};
	float x, y, z, w;
	x = y = z = w = 0.0f;

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, ID_ROT_SAMPLE) == 0)
		{
			sscanf_s(line, "%*s %d %f %f %f %f", &trot.frame, &x, &z, &y, &w);
			trot.qtnion.x = sinf(w / 2.0f) * x;
			trot.qtnion.y = sinf(w / 2.0f) * y;
			trot.qtnion.z = sinf(w / 2.0f) * z;
			trot.qtnion.w = cosf(w / 2.0f);
			vecrot.push_back(trot);
		}
		else if (_stricmp(title, "}") == 0) { break; }
	}
}

void cAseLoader::ReadCtrlRotTcb()
{
	char line[1024];
	char title[256];

	while (1)
	{
		fgets(line, 1024, m_fp);

		sscanf_s(line, "%s", title, sizeof(title));

		if (_stricmp(title, "}") == 0) { break; }
	}
}
