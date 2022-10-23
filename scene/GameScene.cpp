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
uint32_t GameScene::backGroundTexture = 0;
uint32_t GameScene::selectFrameTexture = 0;

GameScene::GameScene()
{

}
GameScene::~GameScene()
{
	Particle::UnLoad();
	// 今はデストラクター一回しか呼ばないためエラー起こらない
	delete backCubeModel;
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

	backGroundTexture = TextureManager::Load("SpriteTexture/backGround/rock.png");
	selectFrameTexture = TextureManager::Load("SpriteTexture/backGround/select_screen.png");
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

	backGroundSprite.reset(Sprite::Create(backGroundTexture, { 0,0 }));
	selectFrameSprite.reset(Sprite::Create(selectFrameTexture, { 0,0 }));

	stages.clear();
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 1)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 2)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 3)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage, 4)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage, 5)));
	stages.emplace_back(move(make_unique<Stage>(RaceStage, 6)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 7)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 8)));
	stages.emplace_back(move(make_unique<Stage>(CannonStage, 9)));
	stages.emplace_back(move(make_unique<Stage>(RaceStage, 10)));
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 1)));


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
	//int lenght = 50;
	//Vector2 p1 = { cosf(DegreeToRad(90)) * lenght,sinf(DegreeToRad(90)) * lenght };
	//Vector2 p2 = { cosf(DegreeToRad(180)) * lenght,sinf(DegreeToRad(180)) * lenght };
	//Vector2 p3 = { cosf(DegreeToRad(90)),sinf(DegreeToRad(90)) };
	//Vector2 p4 = { cosf(DegreeToRad(90)),sinf(DegreeToRad(90)) };
	//temp->SetTextureRect(p3, p2, p4, p1);

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

		// シーンチェンジの時
		if (sceneChange->GetisChange() == true)
		{
			//gameState = isSelect;

			player->Init();
			stageSelect->ResetObjPos();
			viewProjection_.eye = { 0,0,-50 };
			viewProjection_.target = { 0,0,0 };
			viewProjection_.UpdateMatrix();
			isSelectEnd = false;
			sceneChange->SetisChange(false);

			if (stages[currentStage]->GetGameClear() == true)
			{
				currentStage += 1;
				if (currentStage >= stages.size())
				{
					currentStage = stages.size() - 1;
					gameState = isSelect;
				}
				stageSelect->SetCurrentStage(currentStage);
				CurrentStageInit();
			}
			if (stages[currentStage]->GetGameOver() == true)
			{
				gameState = isSelect;
			}
		}
	}
	else if (gameState == isSelect)
	{

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

		// シーンチェンジの時
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

	//debugText_->SetPos(20, 20);
	//debugText_->Printf("CurrentStage = %d", currentStage);

	//debugText_->SetPos(20, 40);
	//debugText_->Printf("GroundHP = %d", ground->GetHP());

	//debugText_->SetPos(20, 60);
	//debugText_->Printf("playerPos = %f,%f", player->GetPos().x, player->GetPos().y);

	//debugText_->SetPos(20, 120);
	//debugText_->Printf("mousePos = %f,%f", input_->GetMousePosition().x, input_->GetMousePosition().y);
}
void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	backGroundSprite->Draw2();	// 背景の描画

	if (gameState == isGame)
	{
		stages[currentStage]->DrawEffectBack();	// モデルの後ろに表示するエフェクトの描画
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	// ------------------------------------------------------------------ //

	// 3Dオブジェクト描画
	Model::PreDraw(commandList);
	if (gameState == isGame)
	{
		stages[currentStage]->Draw();	// 現在のステージのモデルを描画
	}
	else if (gameState == isSelect)
	{
		stageSelect->Draw();	// セレクトボックスの描画
	}
	Model::PostDraw();

	// ------------------------------------------------------------------ //

	// スプライトの描画
	Sprite::PreDraw(commandList);
	if (gameState == isGame)
	{
		stages[currentStage]->DrawSprite();	// ステージ内のスプライトを描画(UI、クリア文字)
	}
	else if (gameState == isSelect)
	{
		stageSelect->DrawSprite();
		selectFrameSprite->Draw2(); // セレクト画面のフレーム
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	Sprite::PostDraw();

	// ------------------------------------------------------------------ //

	// ゲーム時手前に表示するエフェクトの描画
	Sprite::PreDraw(commandList);
	if (gameState == isGame)
	{
		stages[currentStage]->DrawEffectFront();	// エフェクト
	}
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();

	// ------------------------------------------------------------------ //

	// フレームより前のモデルを描画
	Model::PreDraw(commandList);
	if (gameState == isSelect)
	{
		player->Draw(viewProjection_);	// 自機
	}
	Model::PostDraw();

	// シーンチェンジの描画
	Sprite::PreDraw(commandList);
	sceneChange->Draw();
	Sprite::PostDraw();

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
		ground->Init(10);
		break;
	case 1:
		ground->Init(25);
		for (int i = 0; i < 3; i++)
		{
			stages[currentStage]->GenerateStar({ (float)(-20 + i * 20),0,0 });
		}

		//stages[currentStage]->GenerateThorn({ 25,21.5,0 }, false);
		//stages[currentStage]->GenerateThorn({ -25,21.5,0 }, false);
		break;
	case 2:
		ground->Init(35);
		for (int i = 0; i < 4; i++)
		{
			stages[currentStage]->GenerateStar({ (float)(-30 + i * 20),0,0 });
		}

		//stages[currentStage]->GenerateThorn({ -20,21.5,0 }, false);
		//stages[currentStage]->GenerateThorn({ 20,-10,0 }, true);
		//stages[currentStage]->GenerateThorn({ 0,-10,0 }, true);
		break;
	case 3:
		ground->Init(40);
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });

		break;
	case 4:
		ground->Init(55);
		ground->SetThickness(14);
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });

		//stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		break;
	case 5:
		ground->Init(60);
		stages[currentStage]->GenerateGoal({ 150,20,0 });
		for (int i = 0; i < 9; i++)
		{
			stages[currentStage]->GenerateStar({ (float)(15 + i * 15),0,0 });
		}

		/*stages[currentStage]->GenerateBlock({ 20,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -20,0,0 }, true, { 2,2,2 });*/
		break;
	case 6:
		ground->Init(50);
		stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		break;
	case 7:
		ground->Init(80);
		stages[currentStage]->GenerateBlock({ +20,+2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ +20,-2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -20,+2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -20,-2,0 }, true, { 2,2,2 });
		//stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		//stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;
	case 8:
		ground->Init(40);
		stages[currentStage]->GenerateBlock({ +15,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -15,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;
	case 9:
		ground->Init(200);
		stages[currentStage]->GenerateBlock({ 30,21,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ 50,-10,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateThorn({ 70,21.5,0 }, false);
		stages[currentStage]->GenerateThorn({ 90,-10,0 }, true);

		stages[currentStage]->GenerateBlock({ 110,0,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateBlock({ 130,+2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ 130,-2,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateBlock({ 150,-10,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateThorn({ 170,21.5,0 }, false);

		stages[currentStage]->GenerateBlock({ 190,0,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateBlock({ 210,+0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ 210,-4,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateThorn({ 210,-10,0 }, true);

		stages[currentStage]->GenerateBlock({ 230,-10,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateThorn({ 250,21.5,0 }, false);
		stages[currentStage]->GenerateThorn({ 255,21.5,0 }, false);

		stages[currentStage]->GenerateBlock({ 275,+2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ 275,-2,0 }, true, { 2,2,2 });

		stages[currentStage]->GenerateGoal({ 300,20,0 });
		break;
	case 10:	// デバッグ用のステージ
		ground->Init(10000);
		stages[currentStage]->GenerateBlock({ -10,-10,0 }, true, { 2,2,2 });

		//stages[currentStage]->SetisEndurance(true);
		//stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		//stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		//stages[currentStage]->GenerateBlock({ 20,0,0 }, false, { 20,2,2 });
		break;

	default:
		break;
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
		// 距離：0.5 プライヤーが下に降りるフレーム：7.5
		tempPos1.y -= 23 / 7.5f;
		if (tempPos1.y <= -12)
		{
			tempPos1.y = -12;
		}
		stageSelect->SetTextPos(tempPos1, currentStage);

		// 板のスケール
		auto tempScale2 = stageSelect->GetSelectScale(currentStage);
		// スケールサイズ：0.5 プライヤーが下に降りるフレーム：7.5
		tempScale2.y -= 0.5f / 7.5f;
		if (tempScale2.y < 0.0001)
		{
			tempScale2.y = 0.0001;
		}
		stageSelect->SetSelectScale(tempScale2, currentStage);

		//if (player->GetPos().y <= 2)
		if (player->GetPos().y <= -7)
		{
			player->SetPos({ player->GetPos().x, -7, player->GetPos().z });
			player->SetisReverse(true);
		}
	}
	if (player->GetisReverse() == true)
	{
		isSelectEnd = 1;

		// テキストの座標
		auto tempPos1 = stageSelect->GetTextPos(currentStage);
		// 距離：0.5 プライヤーが下に降りるフレーム：7.5
		tempPos1.y += 23 / 7.5f;
		if (tempPos1.y >= 11)
		{
			tempPos1.y = 11;
		}
		stageSelect->SetTextPos(tempPos1, currentStage);

		auto tempScale2 = stageSelect->GetSelectScale(currentStage);
		// スケールサイズ：0.5 プライヤーが下に降りるフレーム：7.5
		tempScale2.y += 0.5 / 7.5;
		if (tempScale2.y > 0.5f)
		{
			tempScale2.y = 0.5f;
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