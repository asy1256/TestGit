#pragma once

class cRawLoader
{
private:

public:
	cRawLoader();
	~cRawLoader();

	void Load(OUT LPD3DXMESH& mesh, IN char* szFolder, IN char* szFile);
};