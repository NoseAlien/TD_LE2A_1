﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"
#include "Random.h"

using namespace std;
using namespace MathUtility;

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	player->DestroyInstance();
	enemy->DestroyInstance();
	collision->DestroyInstance();
}

void GameScene::Initialize()
{
	Random::Initialize();

	Stage::Load();
	player->Load();
	enemy->Load();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	viewProjection_.fovAngleY = DegreeToRad(50);
	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 0,0,0 };
	viewProjection_.up = { 0,1,0 };
	viewProjection_.Initialize();

	stages.emplace_back(move(make_unique<Stage>()));
	stages.emplace_back(move(make_unique<Stage>()));

	for (int i = 0; i < stages.size(); i++)
	{
		stages[i]->Init();
	}

	currentStage = 0;
}

void GameScene::Update()
{
	stages[currentStage]->Update();

	if (input_->TriggerKey(DIK_1))
	{
		stages[currentStage]->Init();
		currentStage = 0;
	}
	if (input_->TriggerKey(DIK_2))
	{
		stages[currentStage]->Init();
		currentStage = 1;
		stages[currentStage]->GenerateThorn({ 20,20,0 });
		stages[currentStage]->GenerateThorn({ -20,20,0 });
	}

	debugText_->SetPos(20, 20);
	debugText_->Printf("CurrentStage = %d", currentStage);

	debugText_->SetPos(20, 40);
	debugText_->Printf("FloorHP = %d", enemy->GetHP());
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

	stages[currentStage]->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


