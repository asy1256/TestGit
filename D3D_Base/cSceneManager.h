#pragma once

#define g_pSceneManager cSceneManager::GetInstance()

class cGameNode;

class cSceneManager
{
private:
	SINGLETONE(cSceneManager);

public:
	typedef std::map<std::string, cGameNode*> mapSceneList;
	typedef std::map<std::string, cGameNode*>::iterator mapSceneIter;
private:
	static cGameNode* _currentScene; //«ˆ¿Á æ¿
	static cGameNode* _readyScene; //±≥√º¡ÿ∫Ò¡ﬂ¿Œ æ¿

	mapSceneList _mSceneList; //æ¿∏ÆΩ∫∆Æ
public:
	void Destroy(void);
	void Update(void);
	void Render(void);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//æ¿ √ﬂ∞°«—¥Ÿ
	cGameNode* addScene(std::string sceneName, cGameNode* scene);

	//æ¿ √º¿Œ¡ˆ
	HRESULT changeScene(std::string sceneName);
};
