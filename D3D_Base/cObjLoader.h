#pragma once

class cGroup;
class cMtlTex;

class cObjLoader
{
private:
	std::map<std::string, cMtlTex*>	m_mapMtlTex;

public:
	cObjLoader();
	~cObjLoader();

	void Load(OUT std::vector<cGroup*>& vecGroup, 
		IN char* szFolder, IN char* szFile);

	void LoadMtlLib( IN char* szFolder, IN char* szMtlFile );

	void CreatMeshFromFile(OUT LPD3DXMESH& mesh,
		IN char* szFolder, IN char* szFile);

};

