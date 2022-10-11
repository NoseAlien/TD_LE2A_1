#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <memory>
#include "Player.h"
#include "Ground.h"
#include "Random.h"
#include "SceneChange.h"
#include "Collision.h"
#include "Particle.h"

using namespace std;
using namespace MathUtility;
ViewProjection viewProjection_{};

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Load()
{
	Random::Initialize();
	Stage::Load();
	player->Load();
	ground->Load();
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	slowMotion = SlowMotion::GetInstance();
	slowMotion->Init();

	viewProjection_.fovAngleY = DegreeToRad(50);
	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 0,0,0 };
	viewProjection_.up = { 0,1,0 };
	viewProjection_.Initialize();
	isSelectEnd = false;

	player->Init();
	//ground->Init(100);

	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage)));
	stages.emplace_back(move(make_unique<Stage>(RaceStage)));

	for (int i = 0; i < stages.size(); i++)
	{
		stages[i]->Init();
	}

	stageSelect = move(make_unique<StageSelect>());
	stageSelect->Initialize(stages.size());

	currentStage = 0;
	gameState = isSelect;
}

void GameScene::Update()
{
	if (gameState == isGame)
	{
		stages[currentStage]->Update();
		if (input_->TriggerKey(DIK_ESCAPE))
		{
			gameState = isSelect;
			player->Init();
			//currentStage = stageSelect->GetCurrentStage();
			stageSelect->ResetObjPos();
		}
	}
	else if (gameState == isSelect)
	{
		if (player->GetPos().y >= 20 && isSelectEnd == true)
		{
			player->SetPos({ 0,20,0 });

			CurrentStageInit();
			sceneChange->StartSceneChange();
			isSelectEnd = false;
		}

		player->SelectSceneUpdate();
		currentStage = stageSelect->GetCurrentStage();
		stageSelect->Update();
		SelectUpdate();

		//if (input_->TriggerKey(DIK_SPACE))
		//{
		//	CurrentStageInit();
		//	sceneChange->StartSceneChange();
		//}

	}

	if (sceneChange->GetisChange() == true)
	{
		gameState = isGame;
	}

	slowMotion->Update();
	sceneChange->Update();


	debugText_->SetPos(20, 20);
	debugText_->Printf("CurrentStage = %d", currentStage);

	debugText_->SetPos(20, 40);
	debugText_->Printf("GroundHP = %d", ground->GetHP());
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	if (gameState == isGame)
	{
		stages[currentStage]->Draw();
	}
	else if (gameState == isSelect)
	{
		player->Draw(viewProjection_);
		stageSelect->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	if (gameState == isGame)
	{
		stages[currentStage]->DrawCountDown();
	}

	sceneChange->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

IScene* GameScene::GetNextScene()
{
	return nullptr;
}

void GameScene::CurrentStageInit()
{
	stages[currentStage]->Init();
	player->Init();
	player->SetStageType(stages[currentStage]->GetStageType());

	switch (currentStage)
	{
	case 0:
		ground->Init(10000);
		break;
	case 1:
		ground->Init(25);
		break;
	case 2:
		ground->Init(35);
		stages[currentStage]->GenerateThorn({ 20,20,0 });
		stages[currentStage]->GenerateThorn({ -20,20,0 });
		break;
	case 3:
		ground->Init(100);
		stages[currentStage]->GenerateCannon({ 40,0,0 }, { 0,0,DegreeToRad(135) });
		stages[currentStage]->GenerateCannon({ -40,0,0 }, { 0,0,DegreeToRad(45) });
		break;
	case 4:
		ground->Init(100);
		stages[currentStage]->GenerateGoal({ 100,20,0 });
		//stages[currentStage]->GenerateCannon({ 40,0,0 }, { 0,0,DegreeToRad(135) });
		//stages[currentStage]->GenerateCannon({ -40,0,0 }, { 0,0,DegreeToRad(45) });
		break;
	}
}

void GameScene::SelectUpdate()
{
	float speed = player->GetAttackMoveSpeed() / 2;
	if (player->GetisReverse() == false && player->GetPos().y <=
		stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y - 2)
	{
		player->SetPos(
			{
				0,
				stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y
				,0
			});

		// テキストの座標
		auto tempPos1 = stageSelect->GetTextPos(currentStage);
		tempPos1.y -= speed * 2;
		if (tempPos1.y <= -5)
		{
			tempPos1.y = -5;
		}
		stageSelect->SetTextPos(tempPos1, currentStage);

		auto tempPos2 = stageSelect->GetSelectPos(currentStage);
		tempPos2.y -= speed;
		if (tempPos2.y <= -5)
		{
			tempPos2.y = -5;
		}
		stageSelect->SetSelectPos(tempPos2, currentStage);

		auto tempScale2 = stageSelect->GetSelectScale(currentStage);
		tempScale2.y -= speed;

		if (tempScale2.y < 0.1)
		{
			tempScale2.y = 0.1;
		}
		stageSelect->SetSelectScale(tempScale2, currentStage);

		if (player->GetPos().y <= -1.5)
		{
			player->SetisReverse(true);
		}
	}
	if (player->GetisReverse() == true)
	{
		isSelectEnd = 1;

		// テキストの座標
		auto tempPos1 = stageSelect->GetTextPos(currentStage);
		tempPos1.y += speed;
		if (tempPos1.y >= 5)
		{
			tempPos1.y = 5;
		}
		stageSelect->SetTextPos(tempPos1, currentStage);

		auto tempPos2 = stageSelect->GetSelectPos(currentStage);
		tempPos2.y += speed;
		if (tempPos2.y >= 0)
		{
			tempPos2.y = 0;
		}
		stageSelect->SetSelectPos(tempPos2, currentStage);

		auto tempScale2 = stageSelect->GetSelectScale(currentStage);
		tempScale2.y += speed;

		if (tempScale2.y > 5)
		{
			tempScale2.y = 5;
		}
		stageSelect->SetSelectScale(tempScale2, currentStage);

	}

}
