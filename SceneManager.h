#pragma once
#include "IScene.h"
#include "Audio.h"
class SceneManager
{
private:
	IScene* scene;
public:
	void Initialize();
	void Update();
	void Draw();

public:
	static SceneManager* GetInstance();
	static void DestroyInstance();
private:
	SceneManager();
	~SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(const SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&&) = delete;
};

extern SceneManager* sceneManager;