#include "stdafx.h"
#include "cSceneManager.h"
#include "cMainGame.h"

cGameNode* cSceneManager::_currentScene = NULL;
cGameNode* cSceneManager::_readyScene = NULL;

cSceneManager::cSceneManager()
{
	_currentScene = NULL;
	_readyScene = NULL;
}

cSceneManager::~cSceneManager()
{
}

void cSceneManager::Destroy(void)
{
	//해당키 검색하고..
	mapSceneIter iter = _mSceneList.begin();

	//맵 전체를 돌면서 지워준다
	for (; iter != _mSceneList.end();)
	{
		//지워지면 반복자 증가시키지 않는다
		if (iter->second != NULL)
		{
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			//반복자 증가..
			++iter;
		}
	}

	_mSceneList.clear();
}

void cSceneManager::Update(void)
{
	if (_currentScene) _currentScene->Update();
}

void cSceneManager::Render(void)
{
	if (_currentScene) _currentScene->Render();
}

void cSceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_currentScene) _currentScene->WndProc(hWnd, message, wParam, lParam);
}

cGameNode* cSceneManager::addScene(std::string sceneName, cGameNode* scene)
{
	if (!scene) return NULL;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT cSceneManager::changeScene(std::string sceneName)
{
	//바꿀 씬 찾고...
	mapSceneIter find = _mSceneList.find(sceneName);

	//못찾으면 펄스
	if (find == _mSceneList.end()) return E_FAIL;

	//바꿀 씬 초기화
	if (SUCCEEDED(find->second->Setup()))
	{
		//기존 씬이 있으면 릴리즈
		if (_currentScene) _currentScene->~cGameNode();
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}
