#pragma once

class cGroup;
class cMtlTex;
class cAseNode;

class cAseLoader
{
private:
	//읽어들인 노드
	std::map<std::string, cAseNode*>	m_mapNode;
	//생성한 메테리얼&텍스쳐
	std::vector<cMtlTex*>				m_vecMtlTex;
	//읽어낸 정점 버텍스 벡터
	std::vector<D3DXVECTOR3>			m_vecVertex;
	//읽어낸 텍스쳐 uv 벡터
	std::vector<D3DXVECTOR2>			m_vecTver;
	int									m_nTverSize;
	//읽어낸 노말 벡터
	std::vector<D3DXVECTOR3>			m_vecNomal;
	//건내줄 버텍스버퍼 내용물
	std::vector<ST_PNT_VERTEX>			m_vecVerBuff;
	//삼각형을 이루는 인덱스를 담는 벡터
	std::vector<ST_TNGLE_IDX>			m_vecIndex;
	//이미지 이름
	std::string							m_strTex;
	//노드 이름
	std::string							m_strNodeName;
	std::string							m_strParentsNodeName;
	//프레임
	DWORD								m_dwFirstFrame;
	DWORD								m_dwLastFrame;
	DWORD								m_dwFrameSpeed;
	DWORD								m_dwTicksPerFrame;
	//파일
	FILE*								m_fp;
public:
	cAseLoader();
	~cAseLoader();

	cAseNode* LoadFile(char* szFolder, char* szFile);

private:
	void ReadSkip();
	void ReadScene();
	void ReadMaterialList();
	void ReadMaterial(int& index);
	void ReadMapDiffuse(int& index);
	void ReadGeometry();
	void ReadMesh();
	void ReadNodeTM();
	void ReadVertexList();
	void ReadFaceList();
	void ReadTverList();
	void ReadTfaceList();
	void ReadNomals();
	void ReadTmAnimation();
	void ReadCtrlPosTrack(std::vector<ST_ANI_POS>& vecpos);
	void ReadCtrlRotTrack(std::vector<ST_ANI_ROT>& vecrot);
	void ReadCtrlRotTcb();
};

