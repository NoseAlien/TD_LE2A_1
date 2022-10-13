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
#include "PrimitiveDrawer.h"

using namespace std;
using namespace MathUtility;
ViewProjection viewProjection_{};

Audio* GameScene::audio = nullptr;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	Particle::UnLoad();
}

void GameScene::Load()
{
	Random::Initialize();
	Stage::Load();
	Player::audio = audio;
	Ground::audio = audio;
	player->Load();
	ground->Load();
	Particle::Load();
}

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	hitstop = HitStop::GetInstance();
	hitstop->Init();
	slowMotion = SlowMotion::GetInstance();
	slowMotion->Init();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	stages.clear();
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage)));
	stages.emplace_back(move(make_unique<Stage>(RaceStage)));

	stageSelect = move(make_unique<StageSelect>());

	viewProjection_.fovAngleY = DegreeToRad(50);
	viewProjection_.eyePos = { 0,0,-50 };
	viewProjection_.targetPos = { 0,0,0 };
	viewProjection_.up = { 0,1,0 };
	viewProjection_.Initialize();

	isSelectEnd = false;

	player->Init();

	stageSelect->Initialize(stages.size());

	currentStage = 0;
	gameState = isSelect;
}

void GameScene::Update()
{


	if (gameState == isGame)
	{
		if (input_->TriggerKey(DIK_ESCAPE))
		{
			gameState = isSelect;
			player->Init();

			stageSelect->ResetObjPos();
			viewProjection_.eyePos = { 0,0,-50 };
			viewProjection_.targetPos = { 0,0,0 };
			viewProjection_.Initialize();
			isSelectEnd = false;
			SlowMotion::GetInstance()->Init();
		}

		if (sceneChange->GetisChange() == true)
		{
			gameState = isSelect;

			player->Init();
			stageSelect->ResetObjPos();
			viewProjection_.eyePos = { 0,0,-50 };
			viewProjection_.targetPos = { 0,0,0 };
			viewProjection_.Initialize();
			isSelectEnd = false;
			sceneChange->SetisChange(false);
		}

		if (input_->TriggerKey(DIK_RETURN))
		{
			pause = !pause;
		}

		if (!pause)
		{
			stages[currentStage]->Update();
		}
	}
	else if (gameState == isSelect)
	{
		//if (player->GetPos().y >= 20 && isSelectEnd == true)

		if (stageSelect->GetTextPos(currentStage).y >= 5 && isSelectEnd == true)
		{
			//player->SetPos({ 0,20,0 });

			CurrentStageInit();
			sceneChange->StartSceneChange();
			isSelectEnd = false;
		}

		player->SelectSceneUpdate();
		currentStage = stageSelect->GetCurrentStage();
		stageSelect->Update();
		SelectUpdate();

		if (sceneChange->GetisChange() == true)
		{
			gameState = isGame;
			sceneChange->SetisChange(false);

		}
	}

	hitstop->Update();
	slowMotion->Update();
	sceneChange->Update();


	debugText_->SetPos(20, 20);
	debugText_->Printf("CurrentStage = %d", currentStage);

	debugText_->SetPos(20, 40);
	debugText_->Printf("GroundHP = %d", ground->GetHP());

	debugText_->SetPos(20, 60);
	debugText_->Printf("playerPos = %f,%f", player->GetPos().x, player->GetPos().y);
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

	if (gameState == isGame)
	{
		stages[currentStage]->DrawLine();
	}

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
		stages[currentStage]->DrawClearTime();
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
	pause = false;

	stages[currentStage]->Init();
	player->Init();
	player->SetStageType(stages[currentStage]->GetStageType());

	switch (currentStage)
	{
	case 0:
		ground->Init(10000);
		stages[currentStage]->GenerateBlock({ 20,0,0 }, { 20,2,2 });
		//stages[currentStage]->GenerateBlock({ 1,0,0 }, { 1,1,1 });
		//stages[currentStage]->GenerateBlock({ 2,0,0 }, { 1,1,1 });
		//stages[currentStage]->GenerateBlock({ 3,0,0 }, { 1,1,1 });

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
		ground->Init(25);
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
		stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y + 1)
	{
		//player->SetScale({ 0,0,0 });
		player->SetPos(
			{
				0,
				stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y + 1
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

		if (player->GetPos().y <= 2)
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
