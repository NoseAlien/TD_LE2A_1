#include "SceneManager.h"
#include "GameScene.h"
#include "Player.h"
#include "Ground.h"
#include "Collision.h"
#include "SceneChange.h"
#include "Star.h"
#include "WindPressureEffect.h"
#include "DamageEffect.h"

SceneManager::SceneManager()
{
	scene = new GameScene;
}

SceneManager::~SceneManager()
{
	delete scene;
	player->DestroyInstance();
	ground->DestroyInstance();
	collision->DestroyInstance();
	sceneChange->DestroyInstance();
	Star::UnLoad();
	Stage::UnLoad();
	Block::UnLoad();
	Thorn::UnLoad();
	Cannon::UnLoad();
	StageSelect::UnLoad();
	WindPressureEffect::DestroyInstance();
	DamageEffect::DestroyInstance();
}


void SceneManager::Initialize()
{
	GameScene::Load();
	Star::Load();
	Block::Load();
	Thorn::Load();
	Cannon::Load();
	Goal::Load();
	StageSelect::Load();

	sceneChange->Load();
	scene->Initialize();
}

void SceneManager::Update()
{
	scene->Update();

	if (scene->GetisEnd())
	{
		IScene* nextScene = scene->GetNextScene();
		nextScene->Initialize();
		delete scene;
		scene = nextScene;
	}
}

void SceneManager::Draw()
{
	scene->Draw();
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager* sceneManager = new SceneManager;
	return sceneManager;
}
void SceneManager::DestroyInstance()
{
	delete SceneManager::GetInstance();
}
SceneManager* sceneManager = SceneManager::GetInstance();