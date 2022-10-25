#include "SceneChange.h"
#include "GameScene.h"
#include "MathUtility.h"
using namespace std;

void SceneChange::Load()
{
	//tex = TextureManager::Load("black1x1.png");
	tex = TextureManager::Load("SpriteTexture/sceneChange.png");
	sprite.reset(Sprite::Create(tex, { 0,0 }));
}

void SceneChange::Initialize()
{
	isSceneChange = false;
	isChange = false;
	isGenerate = false;
	isSceneChangeIn = false;
	isSceneChangeOut = false;
	sprite->SetPosition({ 0, -1080 });
	ease.ReSet();
	bgmEase.ReSet();
}

void SceneChange::Update()
{
	if (isSceneChange == true)
	{
		if (isGenerate == true)
		{
			ease.SetEaseTimer(60);
			ease.SetPowNum(5);
			bgmEase.SetEaseTimer(60);
			bgmEase.SetPowNum(5);
			isGenerate = false;
			isSceneChangeIn = true;
		}

		if (isSceneChangeIn == true)
		{
			ease.Update();
			bgmEase.Update();
			sprite->SetPosition(ease.Out({ 0, -1080 }, { 0,0 }));
			Audio::GetInstance()->SetVolume(GameScene::bgm, bgmEase.Out(1, 0));
			//Audio::GetInstance()->SetVolume(GameScene::bgm, 0);

			if (ease.GetisEnd() == true)
			{
				isSceneChangeIn = false;
				isSceneChangeOut = true;
				isChange = true;
				ease.ReSet();
				bgmEase.ReSet();
			}
		}

		if (isSceneChangeOut == true)
		{
			ease.Update();
			bgmEase.Update();
			sprite->SetPosition(ease.In({ 0,0 }, { 0,1080 }));
			Audio::GetInstance()->SetVolume(GameScene::bgm, bgmEase.In(0, 1));

			if (ease.GetisEnd() == true)
			{
				Initialize();
				//isSceneChangeIn = false;
				//isSceneChangeOut = true;
				//isChange = true;
				ease.ReSet();
				bgmEase.ReSet();
			}
		}
	}
	else
	{
		sprite->SetPosition({ 0, -1080 });
	}
}

void SceneChange::Draw()
{
	sprite->Draw2();
}

void SceneChange::StartSceneChange()
{
	if (isSceneChange == false)
	{
		Initialize();
		isSceneChangeIn = true;
		isSceneChange = true;
		isGenerate = true;
	}
}

SceneChange* SceneChange::GetInstance()
{
	static SceneChange* sceneChange = new SceneChange;
	return sceneChange;
}

void SceneChange::DestroyInstance()
{
	delete SceneChange::GetInstance();
}
SceneChange* sceneChange = SceneChange::GetInstance();