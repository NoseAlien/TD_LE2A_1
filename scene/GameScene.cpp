#include "GameScene.h"
#include "TextureManager.h"
#include "Collision.h"
#include <cassert>
#include <memory>
using namespace std;

double DegreeToRad(double num)
{
	return num / 180 * MathUtility::PI;
}

double RadToDegree(double num)
{
	return num / MathUtility::PI * 180;
}

GameScene::GameScene()
{
	player = move(make_unique<Player>());
	enemy = move(make_unique<Enemy>());
	starTexture = TextureManager::Load("star.png");
}

GameScene::~GameScene()
{
	collision->DestroyInstance();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	player->Init();

	viewProjection_.fovAngleY = DegreeToRad(50);

	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 0,0,0 };
	viewProjection_.up = { 0,1,0 };
	viewProjection_.Initialize();
}

void GameScene::Update()
{
	if (collision->SphereHitSphere(
		player->GetPos(), player->GetRadius(), enemy->GetPos(), enemy->GetRadius()))
	{
		Vector3 tempPos =
		{
			enemy->GetPos().x,
			enemy->GetPos().y/* + 12*/,
			enemy->GetPos().z
		};
		player->SetisReverse(true);

		if (player->GetisHaveStar() == true)
		{
			enemy->Damage(5);
		}
		else
		{
			if (player->GetisHeavyAttack() == true)
			{
				GenerateStar(tempPos);
			}
			enemy->Damage(1);
		}
	}

	for (const auto& temp : stars)
	{
		if (collision->SphereHitSphere(
			player->GetPos(), player->GetRadius(), temp->GetPos(), temp->GetRadius()))
		{
			if (temp->GetisMove() == true)
			{
				player->SetisHaveStar(true);
				stars.remove(temp);
				break;
			}
		}
	}

	player->Update();
	for (const auto& temp : stars)
	{
		temp->Update();
	}

	debugText_->SetPos(20, 20);
	debugText_->Printf("EnemyHP = %d", enemy->GetHP());
}

void GameScene::Draw() {

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

	player->Draw(viewProjection_);
	if (enemy->GetHP() > 0)
	{
		enemy->Draw(viewProjection_);
	}
	for (const auto& temp : stars)
	{
		temp->Draw(viewProjection_, starTexture);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateStar(const Vector3 pos)
{
	for (int i = 0; i < 2; i++)
	{
		stars.emplace_back(move(make_unique<Star>()));
		if (i == 0)
		{
			stars.back()->Generate({ pos.x,pos.y + 8,pos.z }, pos.y + 12, -1);
		}
		if (i == 1)
		{
			stars.back()->Generate({ pos.x,pos.y + 8,pos.z }, pos.y + 12, 1);
		}
	}
}
