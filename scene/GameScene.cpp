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
Model* GameScene::backCubeModel = nullptr;

GameScene::GameScene()
{
}
GameScene::~GameScene()
{
	Particle::UnLoad();
	delete backCubeModel;	// 今はデストラクター一回しか呼ばないためエラー起こらない
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

	backCubeModel = Model::CreateFromOBJ("backCube", true);
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
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(RaceStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage)));

	stageSelect = move(make_unique<StageSelect>(stages.size()));

	viewProjection_.fovAngleY = DegreeToRad(50);
	viewProjection_.eyePos = { 0,0,-50 };
	viewProjection_.targetPos = { 0,0,0 };
	viewProjection_.up = { 0,1,0 };
	viewProjection_.Initialize();

	isSelectEnd = false;

	player->Init();

	stageSelect->Initialize();

	currentStage = 0;
	gameState = isSelect;

	//for (int i = 0; i < 20; i++)
	//{
	//	GenerateBackCube(
	//		{
	//			(float)Random::Range(-100,100),
	//			//0.0f,
	//			(float)Random::Range(-50,50),
	//			(float)Random::Range(50,100),
	//		});
	//}
}

void GameScene::Update()
{
	//BackGroundUpdate();

	if (gameState == isGame)
	{
		if (input_->TriggerKey(DIK_RETURN))
		{
			pause = !pause;
		}

		if (!pause)
		{
			stages[currentStage]->Update();
		}

		if (input_->TriggerKey(DIK_ESCAPE))
		{
			gameState = isSelect;
			player->Init();

			stageSelect->ResetObjPos();
			viewProjection_.eye = { 0,0,-50 };
			viewProjection_.target = { 0,0,0 };
			viewProjection_.Initialize();
			isSelectEnd = false;
			SlowMotion::GetInstance()->Init();
		}

		if (sceneChange->GetisChange() == true)
		{
			gameState = isSelect;

			player->Init();
			stageSelect->ResetObjPos();
			viewProjection_.eye = { 0,0,-50 };
			viewProjection_.target = { 0,0,0 };
			viewProjection_.UpdateMatrix();
			isSelectEnd = false;
			sceneChange->SetisChange(false);
		}
	}
	else if (gameState == isSelect)
	{
		//if (player->GetPos().y >= 20 && isSelectEnd == true)

		if (stageSelect->GetTextPos(currentStage).y >= 5 && isSelectEnd == true)
		{
			//player->SetPos({ 0,20,0 });

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
			CurrentStageInit();
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

	//BackGroundDraw();

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
	case 0:	// デバッグ用のステージ
		ground->Init(10000);
		//stages[currentStage]->GenerateBlock({ 20,0,0 }, false, { 20,2,2 });

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
		ground->Init(80);
		stages[currentStage]->GenerateThorn({ 20,20,0 });
		stages[currentStage]->GenerateThorn({ -20,20,0 });
		break;
	case 4:
		ground->Init(50);
		stages[currentStage]->GenerateGoal({ 100,20,0 });
		break;
	case 5:
		ground->Init(55);
		stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		break;
	case 6:
		ground->Init(60);
		stages[currentStage]->GenerateBlock({ 20,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -20,0,0 }, true, { 2,2,2 });
		break;
	case 7:
		ground->Init(50);
		stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ 4,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -4,0,0 }, true, { 2,2,2 });
		break;
	case 8:
		ground->Init(80);
		stages[currentStage]->GenerateCannon({ 40,0,0 }, { 0,0,DegreeToRad(135) });
		stages[currentStage]->GenerateCannon({ -40,0,0 }, { 0,0,DegreeToRad(45) });
		break;
	case 9:
		ground->Init(40);
		stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateCannon({ 40,0,0 }, { 0,0,DegreeToRad(135) });
		stages[currentStage]->GenerateCannon({ -40,0,0 }, { 0,0,DegreeToRad(45) });
	}
}

void GameScene::SelectUpdate()
{
	float speed = player->GetAttackMoveSpeed() / 2;

	if (player->GetisReverse() == false && player->GetisGround() == false &&
		player->GetPos().y - player->GetRadius() * 3 <=
		stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y)
	{
		player->SetPos(
			{
				0,
				stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y + player->GetRadius() + 2,
				0
			});

		// テキストの座標
		auto tempPos1 = stageSelect->GetTextPos(currentStage);
		tempPos1.y -= speed * 2;
		if (tempPos1.y <= -5)
		{
			tempPos1.y = -5;
		}
		stageSelect->SetTextPos(tempPos1, currentStage);

		// 板の座標
		auto tempPos2 = stageSelect->GetSelectPos(currentStage);
		tempPos2.y -= speed;
		if (tempPos2.y <= -5)
		{
			tempPos2.y = -5;
		}
		stageSelect->SetSelectPos(tempPos2, currentStage);

		// 板のスケール
		auto tempScale2 = stageSelect->GetSelectScale(currentStage);
		tempScale2.y -= speed;

		if (tempScale2.y < 0.1)
		{
			tempScale2.y = 0.1;
		}
		stageSelect->SetSelectScale(tempScale2, currentStage);

		//if (player->GetPos().y <= 2)
		if (player->GetPos().y <= -3.5 + player->GetRadius() * 2.5)
		{
			player->SetPos({ player->GetPos().x, -3.5f + player->GetRadius(), player->GetPos().z });
			player->SetisReverse(true);
		}
	}
	if (player->GetisReverse() == true)
	{
		isSelectEnd = 1;

		// テキストの座標
		auto tempPos1 = stageSelect->GetTextPos(currentStage);
		tempPos1.y += speed * 2;
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
		if (player->GetisGround() == true)
		{
			player->SetPos(
				{
					0,
					stageSelect->GetTextPos(currentStage).y + stageSelect->GetTextScale(currentStage).y + player->GetRadius() + 2,
					0
				});
		}
	}
}

void GameScene::GenerateBackCube(const Vector3& pos)
{
	backCubeTrans.emplace_back(move(make_unique<WorldTransform>()));
	backCubeTrans.back()->Initialize();
	backCubeTrans.back()->translation_ = pos;
	float scale = Random::RangeF(5, 10);
	backCubeTrans.back()->scale_ = { scale,scale,scale };
	backCubeTrans.back()->rotation_ = { 0,0,0 };
	backCubeTrans.back()->rotation_ =
	{
		DegreeToRad(Random::Range(0, 360)),
		DegreeToRad(Random::Range(0, 360)),
		DegreeToRad(Random::Range(0, 360)),
	};
	backCubeMoveSpeed.emplace_back(Random::RangeF(0.01, 0.05));
	backCubeMoveAngle.emplace_back(Random::RangeF(0.5, 1));
	int sign = Random::Range(-1, 1) >= 0 ? 1 : -1;
	backCubeAngleSign.emplace_back(sign);
}
void GameScene::BackGroundUpdate()
{
	// 更新
	for (int i = 0; i < backCubeTrans.size(); i++)
	{
		backCubeTrans[i]->translation_.x -= backCubeMoveSpeed[i];
		backCubeTrans[i]->rotation_.x += backCubeAngleSign[i] * DegreeToRad(backCubeMoveAngle[i]);
		backCubeTrans[i]->rotation_.y += backCubeAngleSign[i] * DegreeToRad(backCubeMoveAngle[i]);
		backCubeTrans[i]->rotation_.z += backCubeAngleSign[i] * DegreeToRad(backCubeMoveAngle[i]);
		backCubeTrans[i]->UpdateMatrix();
	}
	// 削除
	for (int i = 0; i < backCubeTrans.size(); i++)
	{
		if (backCubeTrans[i]->translation_.x <= -120)
		{
			backCubeTrans[i]->translation_.x = 120;
			//backCubeTrans.erase(backCubeTrans.begin() + i);
			//break;
		}
	}
}
void GameScene::BackGroundDraw()
{
	for (int i = 0; i < backCubeTrans.size(); i++)
	{
		backCubeModel->Draw(*backCubeTrans[i].get(), viewProjection_);
	}
}
