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
#include <fstream>
#include <sstream>
using namespace std;
using namespace MathUtility;
ViewProjection viewProjection_{};

Audio* GameScene::audio = nullptr;
uint32_t GameScene::backGroundTexture = 0;
uint32_t GameScene::selectFrameTexture = 0;
uint32_t GameScene::backLightTexture = 0;
uint32_t GameScene::backColorTexture = 0;
uint32_t GameScene::titleLogoTexture = 0;
uint32_t GameScene::saturnTexture = 0;
uint32_t GameScene::meteoriteTexture = 0;
uint32_t GameScene::shootingStarTexture = 0;
uint32_t GameScene::preesSpaceStrTerxture = 0;
uint32_t GameScene::escStrTerxture = 0;
uint32_t GameScene::leverTexture1 = 0;
uint32_t GameScene::leverTexture2 = 0;
uint32_t GameScene::ufoTexture = 0;
uint32_t GameScene::buttonTexture1 = 0;
uint32_t GameScene::buttonTexture2 = 0;

uint32_t GameScene::bgm = 0;
uint32_t GameScene::bgmPlaying = 0;
uint32_t GameScene::picopicoSE = 0;
uint32_t GameScene::spaceSE = 0;

void GameScene::SaveData()
{
	ofstream file;
	file.open("data.txt", ios::out);
	for (int i = 0; i < stages.size(); i++)
	{
		file << stages[i]->fastClearTime << "\n";
	}
	file.close();
}
void GameScene::LoadData()
{
	ifstream file;
	file.open("data.txt");
	string line;
	int index = 0;
	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);
		// 半角スペース区切りで行の先頭文字列を取得
		//string key;
		//getline(lineStream, key, ' ');

		DWORD tempTime = 0;
		lineStream >> tempTime;
		stages[index]->fastClearTime = tempTime;
		index++;
		if (index > stages.size() - 1)
		{
			break;
		}
	}
	file.close();
}
GameScene::GameScene()
{

}
GameScene::~GameScene()
{
	SaveData();

	Particle::UnLoad();
}

void GameScene::Load()
{
	Random::Initialize();
	Stage::Load();
	Player::audio = audio;
	Ground::audio = audio;
	Block::audio = audio;
	player->Load();
	ground->Load();
	Particle::Load();

	backGroundTexture = TextureManager::Load("SpriteTexture/backGround/rock.png");
	selectFrameTexture = TextureManager::Load("SpriteTexture/backGround/select_screen.png");
	backLightTexture = TextureManager::Load("SpriteTexture/backGround/light.png");
	backColorTexture = TextureManager::Load("SpriteTexture/backGround/backColor1x1.png");
	titleLogoTexture = TextureManager::Load("SpriteTexture/title_logo.png");
	saturnTexture = TextureManager::Load("SpriteTexture/backGround/saturn.png");
	meteoriteTexture = TextureManager::Load("SpriteTexture/backGround/meteorite.png");
	shootingStarTexture = TextureManager::Load("SpriteTexture/backGround/shootingStar.png");
	preesSpaceStrTerxture = TextureManager::Load("SpriteTexture/prees_space.png");
	escStrTerxture = TextureManager::Load("SpriteTexture/esc.png");
	leverTexture1 = TextureManager::Load("SpriteTexture/backGround/lever_1.png");
	leverTexture2 = TextureManager::Load("SpriteTexture/backGround/lever_2.png");
	ufoTexture = TextureManager::Load("SpriteTexture/backGround/UFO.png");
	buttonTexture1 = TextureManager::Load("SpriteTexture/backGround/button_1.png");
	buttonTexture2 = TextureManager::Load("SpriteTexture/backGround/button_2.png");

	bgm = Audio::GetInstance()->LoadWave("bgm/bgm.wav");
	picopicoSE = Audio::GetInstance()->LoadWave("se/picopico.wav");
	spaceSE = Audio::GetInstance()->LoadWave("se/picopico2.wav");

}
void GameScene::UnLoad()
{
	//Audio::GetInstance()->Unload(*bgm);
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

	stages.clear();
	stages.emplace_back(move(make_unique<Stage>(TutorialStage, 0)));	// デフォルト
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 1)));	// デフォルト
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 2)));	// 星３
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 3)));	// 星４

	stages.emplace_back(move(make_unique<Stage>(BaseStage, 4)));	// ボーナス

	stages.emplace_back(move(make_unique<Stage>(CannonStage, 5)));	// 砲台
	stages.emplace_back(move(make_unique<Stage>(CannonStage, 6)));	// 砲台ライン
	stages.emplace_back(move(make_unique<Stage>(RaceStage, 7)));	// レース

	stages.emplace_back(move(make_unique<Stage>(CannonStage, 8)));	// ボーナス

	stages.emplace_back(move(make_unique<Stage>(BaseStage, 9)));	// ブロック
	stages.emplace_back(move(make_unique<Stage>(BaseStage, 10)));	// ブロック＊４
	stages.emplace_back(move(make_unique<Stage>(CannonStage, 11)));	// ブロック砲台

	stages.emplace_back(move(make_unique<Stage>(CannonStage, 12)));	// ボーナス
	stages.emplace_back(move(make_unique<Stage>(RaceStage, 13)));	// レース

	LoadData();

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
	gameState = isTitle;

	BackGroundInit();
	TitleInit();

	isGoToTitle = false;

	// 鳴らしてる最中のBGM
	bgmPlaying = audio_->PlayWave(bgm, true);
}
void GameScene::Update()
{
	BackGroundUpdate();

	if (gameState == isGame)
	{
		//if (input_->TriggerKey(DIK_RETURN))
		//{
		//	pause = !pause;
		//}

		if (!pause)
		{
			stages[currentStage]->Update();
		}

		if (stages[currentStage]->GetGameClear() == false &&
			stages[currentStage]->GetGameOver() == false)
		{
			if (input_->TriggerKey(DIK_ESCAPE))
			{
				sceneChange->StartSceneChange();
				player->Init();
				stageSelect->ResetObjPos();
				SlowMotion::GetInstance()->Init();
			}
		}

		// シーンチェンジの時
		if (sceneChange->GetisChange() == true)
		{
			//gameState = isSelect;

			player->Init();
			viewProjection_.eye = { 0,0,-50 };
			viewProjection_.target = { 0,0,0 };
			viewProjection_.UpdateMatrix();
			isSelectEnd = false;
			sceneChange->SetisChange(false);

			if (stages[currentStage]->GetGameClear() == false &&
				stages[currentStage]->GetGameOver() == false)
			{
				stageSelect->ResetObjPos();
				gameState = isSelect;
			}

			if (stages[currentStage]->GetGameClear() == true)
			{
				currentStage += 1;
				//if (currentStage >= stages.size())
				//{
				//	currentStage = stages.size() - 1;
				//}
				if (currentStage == stages.size())
				{
					currentStage = 0;
					gameState = isSelect;
				}
				else
				{
					CurrentStageInit();
				}
				stageSelect->SetCurrentStage(currentStage);
				stageSelect->ResetObjPos();
			}
			if (stages[currentStage]->GetGameOver() == true)
			{
				stageSelect->SetCurrentStage(currentStage);
				stageSelect->ResetObjPos();
				CurrentStageInit();
			}

		}
	}
	else if (gameState == isSelect)
	{
		if (input_->TriggerKey(DIK_ESCAPE))
		{
			sceneChange->StartSceneChange();
			isGoToTitle = true;
		}

		if (stageSelect->GetTextPos(currentStage).y >= 5 && isSelectEnd == true)
		{
			sceneChange->StartSceneChange();
			isSelectEnd = false;
			isGoToTitle = false;
		}

		player->SelectSceneUpdate();
		currentStage = stageSelect->GetCurrentStage();
		stageSelect->Update();
		SelectUpdate();

		// シーンチェンジの時
		if (sceneChange->GetisChange() == true)
		{
			if (isGoToTitle == true)
			{
				TitleInit();
				gameState = isTitle;
				sceneChange->SetisChange(false);

			}
			else
			{
				gameState = isGame;
				CurrentStageInit();
				sceneChange->SetisChange(false);
			}
		}
	}
	else if (gameState == isTitle)
	{
		TitleUpdate();

		if (input_->TriggerKey(DIK_SPACE))
		{
			sceneChange->StartSceneChange();
			//gameState = isSelect;
			player->Init();

			stageSelect->ResetObjPos();
			viewProjection_.eye = { 0,0,-50 };
			viewProjection_.target = { 0,0,0 };
			viewProjection_.Initialize();
			SlowMotion::GetInstance()->Init();
		}
		if (sceneChange->GetisChange() == true)
		{
			gameState = isSelect;
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
}
void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	backColorSprite->Draw2();

	BackGroundDraw();
	if (gameState != isSelect)
	{
		backGroundSprite->SetColor({ 1,1,1,0.5f });
		backGroundSprite->Draw2();	// 背景の描画
	}
	if (gameState == isTitle)
	{
		TitleDraw();
	}
	else if (gameState == isGame)
	{
		stages[currentStage]->DrawEffectBack();	// モデルの後ろに表示するエフェクトの描画
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	//dxCommon_->ClearDepthBuffer();

	Sprite::PreDraw(commandList, Sprite::BlendMode::kAdd);
	if (gameState == isGame)
	{
		stages[currentStage]->DrawBackLight();
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
		player->Draw(viewProjection_);	// 自機
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
		leverSprite2->Draw2();
		leverSprite1->Draw2();
		buttonSprite1->Draw();
		buttonSprite2->Draw();
	}
	if (gameState != isTitle)
	{
		escStrSprite->Draw2();
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
	//dxCommon_->ClearDepthBuffer();

	// ------------------------------------------------------------------ //

	//// フレームより前のモデルを描画
	//Model::PreDraw(commandList);
	//if (gameState == isSelect)
	//{
	//	
	//}
	//Model::PostDraw();

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
	case 0:	// チュートリアル
		ground->Init(40);
		break;

	case 1: // デフォルト
		ground->Init(50);
		break;

	case 2:	// 星三つ
		ground->Init(80);
		for (int i = 0; i < 3; i++)
		{
			stages[currentStage]->GenerateStar({ (float)(-20 + i * 20),0,0 });
		}
		break;

	case 3:	// 星四つ
		ground->Init(80);
		for (int i = 0; i < 4; i++)
		{
			if (i == 0 || i == 3)
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 20),0,0 }, true);
			}
			else
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 20),0,0 });
			}
		}
		break;

	case 4:	 // ボーナス
		stages[currentStage]->SetisEndurance(true);
		stages[currentStage]->SetEndurancePrameter(40, 10, 100);
		//stages[currentStage]->SetEndurancePrameter(5, 10, 100);
		ground->Init(5000);
		ground->SetisIron(true);
		player->SetMoveType(true);

		for (int i = 0; i < 5; i++)
		{
			if (i == 0 || i == 2 || i == 4)
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 15),0,0 }, true);
			}
			else
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 15),0,0 });
			}
		}
		//stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		//stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;

	case 5:	// 砲台
		ground->Init(100);
		stages[currentStage]->GenerateCannon({ 35,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-35) });
		stages[currentStage]->GenerateCannon({ -35,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(35) });
		break;

	case 6: // 砲台ライン
		ground->Init(55);
		ground->SetThickness(14);
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;

	case 7:	// レース
		ground->Init(55);
		stages[currentStage]->GenerateGoal({ 150,20,0 });

		for (int i = 0; i < 9; i++)
		{
			stages[currentStage]->GenerateStar({ (float)(15 + i * 15),0,0 });
		}

		for (int i = 0; i < 15; i++)
		{
			stages[currentStage]->GenerateThorn({ 148,21.5f - float(i) * 3,0 }, false, { 0,0,DegreeToRad(-90) });
		}
		break;

	case 8: // ボーナス
		stages[currentStage]->SetisEndurance(true);
		stages[currentStage]->SetEndurancePrameter(60, 20, 200);
		ground->Init(5000);
		ground->SetisIron(true);
		player->SetMoveType(true);

		for (int i = 0; i < 5; i++)
		{
			if (i == 0 || i == 2 || i == 4)
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 15),0,0 }, true);
			}
			else
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 15),0,0 });
			}
		}
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;

	case 9:	// ブロック
		ground->Init(150);
		stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		break;

	case 10:	// ボロック＊４
		ground->Init(200);
		stages[currentStage]->GenerateBlock({ +20,+2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ +20,-2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -20,+2,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -20,-2,0 }, true, { 2,2,2 });
		break;

	case 11: // ブロック砲台
		ground->Init(200);
		stages[currentStage]->GenerateBlock({ +15,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateBlock({ -15,0,0 }, true, { 2,2,2 });
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;

	case 12:// ボーナス
		stages[currentStage]->SetisEndurance(true);
		stages[currentStage]->SetEndurancePrameter(60, 30, 300);
		ground->Init(5000);
		ground->SetisIron(true);
		player->SetMoveType(true);

		stages[currentStage]->GenerateBlock({ 0,0,0 }, true, { 2,2,2 });
		for (int i = 0; i < 5; i++)
		{
			if (i == 0 || i == 2 || i == 4)
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 15),0,0 }, true);
			}
			else
			{
				stages[currentStage]->GenerateStar({ (float)(-30 + i * 15),0,0 });
			}
		}
		stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		break;

	case 13:
		ground->Init(55);
		stages[currentStage]->GenerateBlock({ 30,0,0 }, true, { 2,2,2 });
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

		for (int i = 0; i < 15; i++)
		{
			stages[currentStage]->GenerateThorn({ 300,21.5f - float(i) * 3,0 }, false, { 0,0,DegreeToRad(-90) });
		}
		break;

		//case 10:	// デバッグ用のステージ
		//	ground->Init(10000);
		//	stages[currentStage]->GenerateBlock({ 20,-10,0 }, true, { 2,2,2 });
		//	//stages[currentStage]->SetisEndurance(true);
		//	//stages[currentStage]->GenerateCannon({ 40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(-45) });
		//	//stages[currentStage]->GenerateCannon({ -40,-5,0 }, { 0,DegreeToRad(180),DegreeToRad(45) });
		//	//stages[currentStage]->GenerateBlock({ 20,0,0 }, false, { 20,2,2 });
		//	break;

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

void GameScene::TitleInit()
{
	titleLogoMoveAngle = 0;
	titleLogoSprite.reset(Sprite::Create(titleLogoTexture, { 960,270 }));
	titleLogoSprite->SetAnchorPoint({ 0.5f,0.5f });

	saturnSprite.reset(Sprite::Create(saturnTexture, { 570,720 }));
	saturnSprite->SetAnchorPoint({ 0.5f,0.5f });
	saturnCircleMove.lenght = 2;
	saturnCircleMove.maxTimer = 20;
	saturnCircleMove.moveAngle = Random::Range(0, 360);

	ufoSprite.reset(Sprite::Create(ufoTexture, { 2200,760 }));
	ufoSprite->SetAnchorPoint({ 0.5f,0.5f });
	ufoMoveDir = -1;
	ufoResetPosTimer = 0;
	ufoResetPosMaxTimer = 120;
	ufoMoveAngle = 0;

	meteoriteSprite.reset(Sprite::Create(meteoriteTexture, { 1440,320 }));
	meteoriteSprite->SetAnchorPoint({ 0.5f,0.5f });
	meteoriteCircleMove.lenght = 2;
	meteoriteCircleMove.maxTimer = 20;
	meteoriteCircleMove.moveAngle = Random::Range(0, 360);
	meteoriteAngle = Random::Range(0, 360);
	switch (Random::Range(0, 1))
	{
	case 0:
		meteoriteAngleDir = -1;
		break;
	case 1:
		meteoriteAngleDir = 1;
		break;
	}
}
void GameScene::TitleUpdate()
{
	titleLogoMoveAngle++;
	if (titleLogoMoveAngle > 360) titleLogoMoveAngle = 0;
	auto tempPos = titleLogoSprite->GetPosition();
	tempPos.y += cosf(DegreeToRad(titleLogoMoveAngle));
	titleLogoSprite->SetPosition(tempPos);
}
void GameScene::TitleDraw()
{
	titleLogoSprite->Draw2();
}

void GameScene::BackGroundInit()
{
	backGroundSprite.reset(Sprite::Create(backGroundTexture, { 0,0 }));
	selectFrameSprite.reset(Sprite::Create(selectFrameTexture, { 0,0 }));

	backColorSprite.reset(Sprite::Create(backColorTexture, { 0,0 }));
	backColorSprite->SetSize({ 1920,1080 });

	shootingStarSprite.reset(Sprite::Create(shootingStarTexture, { -256,-256 }));
	shootingStarSprite->SetAnchorPoint({ 0,0.5f });
	resetTimer = 0;
	resetMaxTimer = 120;
	shootingStarPos = { -256,-256 };
	shootingStarSubScaleSpeed = 2;

	preesSpaceStrSprite.reset(Sprite::Create(preesSpaceStrTerxture, { 960,824 }));
	preesSpaceStrSprite->SetAnchorPoint({ 0.5f,0.5f });
	preesSpaceStrSprite->SetSize({ 410,74 });
	preesSpaceStrEase.SetPowNum(3);
	preesSpaceStrEase.SetEaseTimer(60);
	preesSpaceStrSubAlpha = 0;

	escStrSprite.reset(Sprite::Create(escStrTerxture, { 64,1048 }));
	escStrSprite->SetAnchorPoint({ 0,1 });

	leverAngle = 0;
	leverTargetAngle = 0;
	leverSprite1.reset(Sprite::Create(leverTexture1, { 1498,890 }));
	leverSprite1->SetAnchorPoint({ 0.5f,0.5f });
	leverSprite2.reset(Sprite::Create(leverTexture2, { 1498,890 }));
	leverSprite2->SetAnchorPoint({ 0.5f,1 });
	buttonSprite1.reset(Sprite::Create(buttonTexture1, { 420,850 }));
	buttonSprite1->SetAnchorPoint({ 0.5f,0.5f });
	buttonSprite2.reset(Sprite::Create(buttonTexture2, { 420,890 }));
	buttonSprite2->SetAnchorPoint({ 0.5f,0.5f });

	backLights.clear();

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 522,262 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 252,394 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 472,594 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 194,942 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 690,776 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 840,430 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1038,138 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1102,464 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1210,686 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1278,278 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1458,78 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1616,422 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1718,876 }, backLightTexture);

	backLights.emplace_back(move(make_unique<BackLight>()));
	backLights.back()->Generate({ 1772,172 }, backLightTexture);
}
void GameScene::BackGroundUpdate()
{
	for (int i = 0; i < backLights.size(); i++)
	{
		backLights[i]->Update();
	}
	if (gameState == isTitle)
	{
		// 土星の処理
		saturnSprite->SetPosition(saturnCircleMove.Move(saturnSprite->GetPosition()));

		// 隕石の処理
		meteoriteSprite->SetPosition(meteoriteCircleMove.Move(meteoriteSprite->GetPosition()));
		meteoriteAngle += meteoriteAngleDir * 0.5f;
		if (meteoriteAngle > 360 || meteoriteAngle < -360)
		{
			meteoriteAngle = 0;
		}
		meteoriteSprite->SetRotation(DegreeToRad(meteoriteAngle));

		// PreesSapce
		preesSpaceStrEase.Update();
		if (preesSpaceStrSubAlpha == false)
		{
			preesSpaceStrSprite->SetColor({ 1,1,1,preesSpaceStrEase.In(1,0) });
			if (preesSpaceStrEase.GetisEnd() == true)
			{
				preesSpaceStrEase.ReSet();
				preesSpaceStrSubAlpha = true;
			}
		}
		else
		{
			preesSpaceStrSprite->SetColor({ 1,1,1,preesSpaceStrEase.Out(0,1) });
			if (preesSpaceStrEase.GetisEnd() == true)
			{
				preesSpaceStrEase.ReSet();
				preesSpaceStrSubAlpha = false;
			}

		}
	}
	else if (gameState == isGame || gameState == isSelect)
	{
		// 土星の処理
		if (saturnSprite->GetPosition().x < -295)
		{
			saturnSprite->SetPosition({ 2215,(float)Random::Range(360,720) });
		}

		saturnSprite->SetPosition(saturnCircleMove.Move(
			{
				saturnSprite->GetPosition().x - 0.05f,
				saturnSprite->GetPosition().y }));

		// 隕石の処理
		if (meteoriteSprite->GetPosition().x < -150)
		{
			meteoriteSprite->SetPosition({ 2070,(float)Random::Range(150,600) });

			switch (Random::Range(0, 1))
			{
			case 0:
				meteoriteAngleDir = -1;
				break;
			case 1:
				meteoriteAngleDir = 1;
				break;
			}
		}

		meteoriteSprite->SetPosition({ meteoriteSprite->GetPosition().x - 1,meteoriteSprite->GetPosition().y });
		meteoriteAngle += meteoriteAngleDir * 0.5f;
		if (meteoriteAngle > 360 || meteoriteAngle < -360)
		{
			meteoriteAngle = 0;
		}
		meteoriteSprite->SetRotation(DegreeToRad(meteoriteAngle));
	}

	// 流れ星の処理
	resetTimer++;
	if (resetTimer >= resetMaxTimer)
	{
		resetTimer = resetMaxTimer;

		if (shootingStarPos.y >= 2048 || shootingStarSprite->GetSize().x <= 0)
		{
			shootingStarPos.x = Random::Range(480, 1920);
			shootingStarPos.y = -128;
			resetMaxTimer = Random::Range(120, 240);
			resetTimer = 0;
			shootingStarSprite->SetSize({ 256,256 });
			shootingStarSubScaleSpeed = Random::Range(2, 5);
		}

		const int speed = 15;
		shootingStarPos.x -= speed;
		shootingStarPos.y += speed;
		shootingStarSprite->SetPosition(shootingStarPos);
		shootingStarSprite->SetSize(
			{
				shootingStarSprite->GetSize().x - shootingStarSubScaleSpeed,
				shootingStarSprite->GetSize().y - shootingStarSubScaleSpeed
			});
	}

	// レバーの処理
	if (gameState == isSelect && sceneChange->GetisSceneChangeNow() == false)
	{
		if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_LEFT))
		{
			audio_->PlayWave(picopicoSE);
		}

		const int angleSpeed = 10;
		if (input_->PushKey(DIK_RIGHT))
		{
			leverTargetAngle = +60;

			leverAngle += angleSpeed;
			if (leverAngle >= leverTargetAngle)
			{
				leverAngle = leverTargetAngle;
			}
		}
		else if (input_->PushKey(DIK_LEFT))
		{
			leverTargetAngle = -60;

			leverAngle -= angleSpeed;
			if (leverAngle <= leverTargetAngle)
			{
				leverAngle = leverTargetAngle;
			}
		}
		if (!input_->PushKey(DIK_LEFT) && !input_->PushKey(DIK_RIGHT))
		{
			if (leverTargetAngle > 0)
			{
				leverAngle -= angleSpeed;
				if (leverAngle <= 0)
				{
					leverTargetAngle = 0;
					leverAngle = 0;
				}
			}
			if (leverTargetAngle < 0)
			{
				leverAngle += angleSpeed;
				if (leverAngle >= 0)
				{
					leverTargetAngle = 0;
					leverAngle = 0;
				}
			}

		}
		//leverSprite2->SetPosition(
		//	{
		//		cosf(DegreeToRad(leverAngle) * 40) + 1498,
		//		sinf(DegreeToRad(leverAngle) * 40) + 810,
		//	});

		leverSprite2->SetRotation(DegreeToRad(leverAngle));
	}

	if (gameState == isTitle || gameState == isSelect)
	{
		if (input_->TriggerKey(DIK_SPACE) && sceneChange->GetisSceneChangeNow() == false)
		{
			audio_->PlayWave(spaceSE);
		}
	}

	// UFOの移動処理
	if (ufoSprite->GetPosition().x <= -220 && ufoMoveDir == -1)
	{
		ufoResetPosTimer++;
		if (ufoResetPosTimer >= ufoResetPosMaxTimer)
		{
			ufoMoveDir = 1;
			ufoSprite->SetPosition({ -220,260 });
			ufoResetPosTimer = 0;
			ufoResetPosMaxTimer = Random::Range(120, 180);
		}

	}
	if (ufoSprite->GetPosition().x >= 2200 && ufoMoveDir == 1)
	{
		ufoResetPosTimer++;
		if (ufoResetPosTimer >= ufoResetPosMaxTimer)
		{
			ufoMoveDir = -1;
			ufoSprite->SetPosition({ 2200,760 });
			ufoResetPosTimer = 0;
			ufoResetPosMaxTimer = Random::Range(120, 180);
		}
	}

	// 向き
	if (ufoMoveDir == -1)
	{
		ufoSprite->SetRotation(DegreeToRad(-30));
	}
	else if (ufoMoveDir == 1)
	{
		ufoSprite->SetRotation(DegreeToRad(+30));
	}

	// 上下動く処理
	ufoMoveAngle++;
	if (ufoMoveAngle > 360)
	{
		ufoMoveAngle = 0;
	}

	ufoSprite->SetPosition(
		{
			ufoSprite->GetPosition().x + ufoMoveDir * 5,
			ufoSprite->GetPosition().y + sin(DegreeToRad(ufoMoveAngle)),
		});

	static int isButtonPush = false;
	static int isButtonReverce = false;

	// ボタン
	if (gameState == isSelect)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			isButtonPush = true;
		}

		if (isButtonPush == true)
		{
			const int speed = 6;
			if (isButtonReverce == false)
			{
				buttonSprite1->SetPosition({ buttonSprite1->GetPosition().x,buttonSprite1->GetPosition().y + speed });

				if (buttonSprite1->GetPosition().y >= 890)
				{
					isButtonReverce = true;
				}
			}
			if (isButtonReverce == true)
			{
				buttonSprite1->SetPosition({ buttonSprite1->GetPosition().x,buttonSprite1->GetPosition().y - speed });

				if (buttonSprite1->GetPosition().y <= 850)
				{
					buttonSprite1->SetPosition({ buttonSprite1->GetPosition().x,850 });
					//isButtonReverce = true;
					isButtonReverce = false;
					isButtonPush = false;

				}
			}
		}
	}
}
void GameScene::BackGroundDraw()
{
	for (int i = 0; i < backLights.size(); i++)
	{
		backLights[i]->Draw();
	}

	shootingStarSprite->Draw2();
	meteoriteSprite->Draw2();
	ufoSprite->Draw2();
	saturnSprite->Draw2();

	if (gameState == isTitle)
	{
		preesSpaceStrSprite->Draw2();
	}
}

void BackLight::Generate(const Vector2& pos, const uint32_t& tex)
{
	backLight.reset(Sprite::Create(tex, pos));
	backLight->SetAnchorPoint({ 0.5f,0.5f });
	alpha = Random::RangeF(0, 1);
}
void BackLight::Update()
{
	if (isReverse == false)
	{
		alpha -= 0.025;
		if (alpha <= 0)
		{
			alpha = 0;
			isReverse = true;
		}
	}
	if (isReverse == true)
	{
		alpha += 0.025;
		if (alpha >= 1)
		{
			alpha = 1;
			isReverse = false;
		}
	}

	backLight->SetColor({ 1,1,1,alpha });
}
void BackLight::Draw()
{
	backLight->Draw2();
}
