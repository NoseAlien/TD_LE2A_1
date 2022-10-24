#include "Stage.h"
#include "Collision.h"
#include "Player.h"
#include "DebugText.h"
#include "Random.h"
#include "GameScene.h"
#include "HitStop.h"
#include "PrimitiveDrawer.h"
#include "SceneChange.h"
using namespace std;

uint32_t Stage::thornTexture = 0;
vector<uint32_t> Stage::startTextTextures = {};
vector<uint32_t> Stage::numberSheet = {};
uint32_t Stage::timeStrTexture = 0;
uint32_t Stage::fastTimeStrTexture = 0;
uint32_t Stage::dotStrTexture = 0;
uint32_t Stage::clearStrTexture = 0;
Model* Stage::lineModel = nullptr;
uint32_t Stage::lineModelTexture;
vector<uint32_t> Stage::stageNumberTextures = {};
uint32_t Stage::overStrTexture;

const float lerp(const float& start, const float& end, const double progress)
{
	double clampedProgress = min(max(progress, 0), 1);
	return start * (1.0f - clampedProgress) + end * clampedProgress;
}

Stage::Stage(const int& stageType, const int& stageNumber) :
	stageType(stageType), playerIsHitGoal(false), stagePcrogress(Start), clearTimeDights(6),
	fastClearTime(100000)
{
	goal = nullptr;

	for (int i = 0; i < startTextTextures.size(); i++)
	{
		startTextSprites[i] = Sprite::Create(startTextTextures[i], { 960,540 });
		startTextSprites[i]->SetAnchorPoint({ 0.5, 0.5 });
	}

	for (int i = 0; i < clearTimeDights; i++)
	{
		clearTimeSprites[i] = Sprite::Create(numberSheet[i], { -128,-128 });
		clearTimeSprites[i]->SetAnchorPoint({ 0.5,0.5 });
		clearTimeSprites[i]->SetSize({ 64,64 });

		fastClearTimeSprites[i] = Sprite::Create(numberSheet[i], { -128,-128 });
		fastClearTimeSprites[i]->SetAnchorPoint({ 0.5,0.5 });
		fastClearTimeSprites[i]->SetSize({ 64,64 });
	}

	timeStrSprite = Sprite::Create(timeStrTexture, { -128,-128 });
	timeStrSprite->SetAnchorPoint({ 0.5, 0.5 });
	timeStrSprite->SetSize({ 128,64 });

	fastTimeStrSprite = Sprite::Create(fastTimeStrTexture, { -128,-128 });
	fastTimeStrSprite->SetAnchorPoint({ 0.5, 0.5 });
	fastTimeStrSprite->SetSize({ 256,64 });

	dotStrSprite = Sprite::Create(dotStrTexture, { -128,-128 });
	dotStrSprite->SetAnchorPoint({ 0.5, 0.5 });
	dotStrSprite->SetSize({ 64,64 });

	dotStrSprite2 = Sprite::Create(dotStrTexture, { -128,-128 });
	dotStrSprite2->SetAnchorPoint({ 0.5, 0.5 });
	dotStrSprite2->SetSize({ 64,64 });

	clearStrSprite = Sprite::Create(clearStrTexture, { 960,270 });
	clearStrSprite->SetAnchorPoint({ 0.5, 0.5 });
	clearStrSprite->SetSize({ 0,0 });

	overStrSprite.reset(Sprite::Create(overStrTexture, { 0,0 }));
	overStrSprite->SetAnchorPoint({ 0.5f,0.5f });
	overStrSprite->SetSize({ 0,0 });

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			enduranceTimeSprites[i] = Sprite::Create(numberSheet[3], { 1820,64 });
		}
		else if (i == 1)
		{
			enduranceTimeSprites[i] = Sprite::Create(numberSheet[0], { 1844,64 });
		}
		enduranceTimeSprites[i]->SetPosition({ (float)(1844 - ((1 - i) * 24)),64 });
		enduranceTimeSprites[i]->SetAnchorPoint({ 0.5, 0.5 });
		enduranceTimeSprites[i]->SetSize({ 32,32 });
	}

	stageNumberSprite.reset(Sprite::Create(stageNumberTextures[stageNumber - 1], { 960,540 }));
	stageNumberSprite->SetAnchorPoint({ 0.5f,0.5f });

	grainScatterEffect = move(make_unique<GrainScatterEffect>());
	repairEffect = move(make_unique<RepairEffect>());

	windPressureEffect = WindPressureEffect::GetInstance();
}
Stage::~Stage()
{
	for (int i = 0; i < startTextTextures.size(); i++)
	{
		delete startTextSprites[i];
	}
	for (int i = 0; i < clearTimeDights; i++)
	{
		delete clearTimeSprites[i];
		delete fastClearTimeSprites[i];
	}

	delete clearStrSprite;
	delete timeStrSprite;
	delete fastTimeStrSprite;
	delete dotStrSprite;
	delete dotStrSprite2;

	for (int i = 0; i < 2; i++)
	{
		delete enduranceTimeSprites[i];
	}

}

Vector2 clearStrSize(0, 0);
float addSizeValue = 512;
void Stage::Load()
{
	startTextTextures.emplace_back(TextureManager::Load("SpriteTexture/Text/StartText1.png"));
	startTextTextures.emplace_back(TextureManager::Load("SpriteTexture/Text/StartText2.png"));
	startTextTextures.emplace_back(TextureManager::Load("SpriteTexture/Text/StartText3.png"));
	startTextTextures.emplace_back(TextureManager::Load("SpriteTexture/Text/StartText4.png"));
	//thornTexture = TextureManager::Load("thorn.png");
	string filepath;
	for (int i = 0; i < 10; i++)
	{
		filepath = "SpriteTexture/NumberSheet/number" + to_string(i) + ".png";
		numberSheet.emplace_back(TextureManager::Load(filepath.c_str()));
	}
	timeStrTexture = TextureManager::Load("SpriteTexture/TimeStr.png");
	fastTimeStrTexture = TextureManager::Load("SpriteTexture/FastTImeStr.png");
	clearStrTexture = TextureManager::Load("SpriteTexture/clear.png");
	overStrTexture = TextureManager::Load("SpriteTexture/gameover.png");
	dotStrTexture = TextureManager::Load("SpriteTexture/Dot.png");

	lineModel = Model::CreateFromOBJ("lineModel", true);
	lineModelTexture = TextureManager::Load("lineModel/lineModel2.png");

	for (int i = 1; i <= 10; i++)
	{
		stageNumberTextures.push_back(
			TextureManager::Load(
				"SpriteTexture/select_nuber/stage_number_" + to_string(i) + ".png"));
	}
}
void Stage::UnLoad()
{
	delete lineModel;
}

void Stage::Init()
{
	windPressureEffect->Clear();

	// ライン関連
	lineTrans = move(make_unique<WorldTransform>());
	lineTrans->Initialize();
	lineTrans->translation_ = { 0,0,5 };
	lineTrans->scale_ = { 1,1,1 };
	lineTrans->UpdateMatrix();
	lineTrans2 = move(make_unique<WorldTransform>());
	lineTrans2->Initialize();
	lineTrans2->translation_ = { 92,0,5 };
	lineTrans2->scale_ = { 1,1,1 };
	lineTrans2->UpdateMatrix();

	clearStrSize = { 0,0 };
	addSizeValue = 512;

	viewProjection_.eyePos = { 0,0,-50 };
	viewProjection_.targetPos = { 0,0,0 };
	viewProjection_.up = { 0,1,0 };
	viewProjection_.UpdateMatrix();
	stars.clear();
	thorns.clear();
	blocks.clear();
	cannons.clear();
	gameClear = false;
	gameOver = false;
	playerIsHitGoal = false;

	// スタートカウント関連
	startTextIndex = 0;
	startTextTimer = 0;
	startTextMaxTimer = 80;
	startTextExrate = 0;
	startTextAngle = -180;
	startTextAlpha = 1;
	//isStartTextEnd = false;
	isStartTextEnd = true;
	stagePcrogress = Start;

	// クリア関連
	startTime = 0;
	endTime = 0;
	clearTime = 0;
	isShowClearTime = false;
	clearTimeLastDightPos = { 2500,952 };
	isMoveClearTime = false;

	fastTimeLastDightPos = { 2500,952 };

	isCameraMoveStep = false;
	stagePcrogress = Start;
	cameraMoveVec = { 0,0,0 };

	isPlayerDieEffectGenerate = false;

	// 耐久戦関連
	isEndurance = false;
	isGetTime = 0;
	enduranceTime = 30;
	enduranceNowTime = 0;
	enduranceStartTime = 0;
	enduranceEndTime = 0;
	enduranceTimeDightsNumber.resize(2);
	enduranceTimeDightsNumber[0] = 3;
	enduranceTimeDightsNumber[0] = 0;

	enduranceLineTrans = move(make_unique<WorldTransform>());
	enduranceLineTrans->Initialize();
	enduranceLineTrans->translation_ = { 0,-3.85,5 };
	enduranceLineTrans->scale_ = { 1,1,1 };
	enduranceLineTrans->UpdateMatrix();

	// ステージ表示関連
	isShowStageNumber = true;
	sizeExrate = 0;
	rotAngel = 0;
	alpha = 1;
	stageNumberSprite->SetSize({ 0,0 });
	stageNumberSprite->SetRotation(0);
	stageNumberSprite->SetColor({ 1,1,1,1 });

	// 星復活関連
	overStrSprite->SetPosition({ 960,1500 });
}

void Stage::Update()
{
	//if (gameClear == true || gameOver == true) return;
	//if (stagePcrogress == End) return;

	//static float sizeExrate = 0;
	//static float rotAngel = 0;
	//static float alpha = 1;

	if (sceneChange->GetisSceneChangeNow() == false)
	{
		// ステージ表示
		ShowStageNumberUpdate();

		// カウントダウン処理
		CountDownUpdate();
	}

	if (stagePcrogress == Play || stagePcrogress == Staging)
	{
		StarUpdate();
		BlockUpdate();
		FloorUpdate();
		ThornUpdate();
		if (stageType == CannonStage)
		{
			CannonUpdate();
		}
		else if (stageType == RaceStage)
		{
			RaceUpdate();
		}
		if (isEndurance == true)
		{
			EnduranceUpdate();
		}
		if (player->GetLife() > 0)
		{
			PlayerUpdate();
		}

		// ゲームクリアの時
		if (ground->GetHP() <= 0 || isGetTime == 2)
		{
			if (stagePcrogress == Play)
			{
				stagePcrogress = Staging;
				SlowMotion::GetInstance()->StartSlowMotion(0.05, 280);
				endTime = GetNowTime();
				clearTime = endTime - startTime;
				if (fastClearTime > clearTime)
				{
					fastClearTime = clearTime;
				}

				isMoveClearTime = true;
				gameClear = true;
			}

			// 変更
			if (stagePcrogress == Staging &&
				SlowMotion::GetInstance()->GetisSlowMotion() == false)
			{
				stagePcrogress = End;
				sceneChange->StartSceneChange();
			}
		}

		// ゲームオーバーの時
		if (ground->GetPos().y + ground->GetScale().y >= 0 ||
			player->GetLife() <= 0 || playerIsHitGoal == true)
		{
			if (stagePcrogress == Play)
			{
				player->SetLife(0);
				stagePcrogress = Staging;
				isCameraMoveStep = true;
				endTime = GetNowTime();
				clearTime = endTime - startTime;
			}
			gameOver = true;
		}
		if (stageType == RaceStage && !isCameraMoveStep)
		{
			viewProjection_.eyePos = { player->GetPos().x,0,-50 };
			viewProjection_.targetPos = { player->GetPos().x ,0,0 };
		}
		if (player->GetDieEffectisEnd() == true)
		{
			overScreenClock++;

			float clearStrPosY = lerp(1500.0f, 400, pow((overScreenClock - 50) / 30.0, 0.2));

			overStrSprite->SetPosition({ 960,clearStrPosY });
			overStrSprite->SetSize({ 768,768 });

			if (overScreenClock >= 160)
			{
				stagePcrogress = End;
				sceneChange->StartSceneChange();
				player->SetDieEffectisEnd(false);
				overScreenClock = 0;
			}
		}
	}

	GameOverCameraUpdate();
	ClearTimeUpdate();

	viewProjection_.ShakeUpdate();
	viewProjection_.UpdateMatrix();
	player->EffectUpdate();
	ground->EffectUpdate();
	windPressureEffect->Update();
	grainScatterEffect->Update();
	repairEffect->Update();
}
void Stage::Draw()
{
	lineModel->Draw(*lineTrans, viewProjection_);
	lineModel->Draw(*lineTrans2, viewProjection_);

	if (isEndurance == true)
	{
		lineModel->Draw(*enduranceLineTrans, viewProjection_, lineModelTexture);
	}

	player->Draw(viewProjection_);
	ground->Draw(viewProjection_);

	for (const auto& temp : stars)
	{
		temp->Draw(viewProjection_);
	}

	for (const auto& temp : thorns)
	{
		temp->Draw(viewProjection_);
	}

	for (const auto& temp : blocks)
	{
		temp->Draw(viewProjection_);
	}

	for (const auto& temp : cannons)
	{
		temp->Draw(viewProjection_);
	}

	if (goal != nullptr)
	{
		goal->Draw(viewProjection_);
	}

	ground->EffectDraw();
}
void Stage::DrawSprite()
{
	ground->DrawSprite();

	// 耐久戦タイマー
	if (isEndurance == true)
	{
		for (int i = 0; i < 2; i++)
		{
			enduranceTimeSprites[i]->Draw2();
		}
	}

	// ステージ表示
	if (isShowStageNumber == true)
	{
		stageNumberSprite->Draw2();
	}

	// クリア描画
	if (gameClear)
	{
		clearStrSprite->Draw2();
		for (int i = 0; i < dightsNumber.size(); i++)
		{
			clearTimeSprites[i]->Draw2();
			fastClearTimeSprites[i]->Draw2();
		}
		dotStrSprite->Draw2();
		dotStrSprite2->Draw2();

		timeStrSprite->Draw2();
		fastTimeStrSprite->Draw2();
	}
	if (gameOver)
	{
		overStrSprite->Draw2();
	}
}
void Stage::DrawEffectFront()
{
	// カウントダウン
	if (isShowStageNumber == false)
	{
		if (startTextIndex < 4)
		{
			startTextSprites[startTextIndex]->Draw2();
		}
	}

	grainScatterEffect->Draw();
	repairEffect->Draw();
	windPressureEffect->Draw();
	player->DrawSpriteFront();
}
void Stage::DrawEffectBack()
{
	player->DrawSpriteBack();
	for (const auto& temp : stars)
	{
		temp->DrawEffectBack();
	}
}

void Stage::ShowStageNumberUpdate()
{
	if (isShowStageNumber == true)
	{
		sizeExrate += 0.04f;
		if (sizeExrate >= 2)
		{
			sizeExrate = 2;
		}
		stageNumberSprite->SetSize({ 600 * sizeExrate,140 * sizeExrate });

		rotAngel += 7.2f;
		if (rotAngel >= 360)
		{
			rotAngel = 360;
		}
		stageNumberSprite->SetRotation(DegreeToRad(rotAngel));

		if (sizeExrate == 2 && rotAngel == 360)
		{
			alpha -= 0.025;
			if (alpha <= 0)
			{
				alpha = 0;
				isShowStageNumber = false;
				isStartTextEnd = false;
			}
			stageNumberSprite->SetColor({ 1,1,1,alpha });
		}
	}
}
void Stage::CountDownUpdate()
{
	if (isStartTextEnd == true) return;

	// カウント
	const float fream = 30;
	if (startTextIndex < 3)
	{
		startTextTimer++;
		if (startTextTimer >= startTextMaxTimer)
		{
			startTextExrate = 0;
			startTextAlpha = 1;
			startTextAngle = -180;
			startTextTimer = 0;
			startTextIndex++;
		}

		// 拡大率
		if (startTextExrate >= 1)
		{
			startTextExrate += 0.005;
		}
		else if (startTextExrate >= 0)
		{
			startTextExrate += 1 / fream;
		}
		startTextSprites[startTextIndex]->SetSize({ 448 * startTextExrate,448 * startTextExrate });

		// 角度
		if (startTextAngle >= 0)
		{
			startTextAngle += 0.25;
		}
		else if (startTextAngle >= -180)
		{
			startTextAngle += 180 / fream;
		}
		startTextSprites[startTextIndex]->SetRotation(DegreeToRad(startTextAngle));

		// アルファ
		startTextAlpha -= 1 / (float)startTextMaxTimer;
		startTextSprites[startTextIndex]->SetColor({ 1,1,1,startTextAlpha });
	}
	else if (startTextIndex == 3)
	{
		stagePcrogress = Play;
		// 拡大率
		if (startTextExrate >= 1.5)
		{
			startTextExrate += 0.005;
		}
		else if (startTextExrate >= 0)
		{
			startTextExrate += 1.5 / fream;
		}
		startTextSprites[startTextIndex]->SetSize({ 726 * startTextExrate,448 * startTextExrate });


		startTextSprites[startTextIndex]->SetRotation(DegreeToRad(0));

		// アルファ
		startTextAlpha -= 1 / (float)startTextMaxTimer;
		startTextSprites[startTextIndex]->SetColor({ 1,1,1,startTextAlpha });

		startTextTimer++;
		if (startTextTimer >= startTextMaxTimer)
		{
			startTextIndex = 4;
			isStartTextEnd = true;
			startTime = GetNowTime();
		}
	}
}
void Stage::ClearTimeUpdate()
{
	if (gameClear)
	{
		clearScreenClock++;

		//float clearStrPosY = lerp(1700.0f, 270, pow((clearScreenClock - 50) / 30.0, 0.2));
		float clearStrPosY = lerp(1700.0f, 270, pow((clearScreenClock - 50) / 30.0, 0.2));

		clearStrSprite->SetPosition({ 960,clearStrPosY });
		clearStrSprite->SetSize({ 768,768 });

		//clearTimeLastDightPos.x = lerp(2400, 1856, pow((clearScreenClock - 100) / 30.0, 0.2));
		dightsNumber = GetDightsNumber(clearTime);

		clearTimeLastDightPos.x = 960 + ((dightsNumber.size() + 1) * 48 + 128) / 2;
		clearTimeLastDightPos.y = lerp(1700, 480, pow((clearScreenClock - 125) / 30.0, 0.2));

		// クリアタイム
		for (int i = 0; i < dightsNumber.size(); i++)
		{
			clearTimeSprites[i]->SetTextureHandle(numberSheet[dightsNumber[i]]);
			clearTimeSprites[i]->SetPosition(
				{
					clearTimeLastDightPos.x - (dightsNumber.size() - (float)i) * 48,
					clearTimeLastDightPos.y
				});
			if (i == dightsNumber.size() - 2)
			{
				dotStrSprite->SetPosition(
					{
						clearTimeLastDightPos.x - (dightsNumber.size() - (float)i) * 48,
						clearTimeLastDightPos.y
					});
			}
		}
		timeStrSprite->SetPosition(
			{
				clearTimeLastDightPos.x - dightsNumber.size() * 48 - 128,
				clearTimeLastDightPos.y
			});

		// 最速タイム
		fastTimedightsNumber = GetDightsNumber(fastClearTime);

		fastTimeLastDightPos.x = 960 + ((fastTimedightsNumber.size() + 1) * 48 + 128) / 2;
		fastTimeLastDightPos.y = lerp(1700, 544, pow((clearScreenClock - 150) / 30.0, 0.2));

		// 桁の座標調整
		for (int i = 0; i < fastTimedightsNumber.size(); i++)
		{
			fastClearTimeSprites[i]->SetTextureHandle(numberSheet[fastTimedightsNumber[i]]);
			fastClearTimeSprites[i]->SetPosition(
				{
					fastTimeLastDightPos.x - (fastTimedightsNumber.size() - (float)i) * 48,
					fastTimeLastDightPos.y
				});
			if (i == fastTimedightsNumber.size() - 2)
			{
				dotStrSprite2->SetPosition(
					{
						fastTimeLastDightPos.x - (fastTimedightsNumber.size() - (float)i) * 48,
						fastTimeLastDightPos.y
					});
			}
		}
		fastTimeStrSprite->SetPosition(
			{
				fastTimeLastDightPos.x - dightsNumber.size() * 48 - 192,
				fastTimeLastDightPos.y
			});
	}
	else
	{
		clearScreenClock = 0;
	}
}
void Stage::GameOverCameraUpdate()
{
	if (isCameraMoveStep)
	{
		Vector3 vec = player->GetPos() + Vector3{ 0, -2, -8 } - viewProjection_.eyePos;
		viewProjection_.eyePos += vec * 0.4;
		viewProjection_.targetPos = viewProjection_.eyePos + Vector3{ 0, 0, 1 };

		if (vec.Magnitude() <= 0.00005 && isPlayerDieEffectGenerate == false)
		{
			//sceneChange->StartSceneChange();
			player->DieEffectGenerate();
			isPlayerDieEffectGenerate = true;
		}
	}
}

void Stage::GenerateStar(const Vector3& pos)
{
	stars.emplace_back(move(make_unique<Star>()));
	stars.back()->Generate(
		{
			pos.x,
			ground->GetPos().y + ground->GetScale().y + 1.5f,
			pos.z
		},
		{ 1,0,0 }, 0);
	stars.back()->SetisCanHit(true);
	stars.back()->SetisGround(true);
	stars.back()->SetisGenerate(true);
	stars.back()->revival->isCanRevival = true;
	stars.back()->revival->pos = stars.back()->GetPos();
}
void Stage::GenerateThorn(const Vector3& pos, const bool& isReverseVertical, const Vector3& scale)
{
	if (isReverseVertical == false)
	{
		thorns.emplace_back(move(make_unique<Thorn>()));
		thorns.back()->Generate(pos, scale);
	}
	else
	{
		thorns.emplace_back(move(make_unique<Thorn>()));
		thorns.back()->Generate(pos, scale, { 0,0,DegreeToRad(180) });
	}
}
void Stage::GenerateBlock(const Vector3& pos, const bool& haveStar, const Vector3& scale)
{
	blocks.emplace_back(move(make_unique<Block>()));
	blocks.back()->Generate(pos, scale, haveStar);
	blocks.back()->SetisGenerate(true);
	blocks.back()->revival->isCanRevival = true;
	blocks.back()->revival->pos = pos;
	//blockRevivals.emplace_back(move(make_unique<RevivalObject>()));
	//blockRevivals.back()->pos = pos;
}
void Stage::GenerateCannon(const Vector3& pos, const Vector3& rot)
{
	cannons.emplace_back(move(make_unique<Cannon>()));
	if (cannons.size() == 1)
	{
		cannons.back()->Generate(pos, rot, 135);
	}
	else if (cannons.size() == 2)
	{
		cannons.back()->Generate(pos, rot, 45);
	}
}
void Stage::GenerateGoal(const Vector3& pos)
{
	goal = move(make_unique<Goal>());
	goal->Generate(pos);
}

void Stage::PlayerGenerateStar(const Vector3& pos)
{
	for (int i = 0; i < 2; i++)
	{
		stars.emplace_back(move(make_unique<Star>()));
		if (i == 0)
		{
			stars.back()->Generate(
				{
					pos.x,
					ground->GetPos().y + ground->GetScale().y + 1.5f,
					pos.z
				},
				{ -1,0,0 }, 0);
			stars.back()->SetSpeed(1.3);
			stars.back()->SetisGround(true);

		}
		if (i == 1)
		{
			stars.back()->Generate(
				{
					pos.x,
					ground->GetPos().y + ground->GetScale().y + 1.5f,
					pos.z
				},
				{ 1,0,0 }, 0);
			stars.back()->SetSpeed(1.3);
			stars.back()->SetisGround(true);
		}
	}
}
void Stage::CannonGenerateStar(const Vector3& pos, const Vector3& dieVec, const float& angle)
{
	stars.emplace_back(move(make_unique<Star>()));
	stars.back()->Generate(pos, dieVec, 1);

	int random = Random::Range(player->GetPos().x, player->GetPos().x + 30);
	float l = (random - pos.x) / 2;
	float speed = (l * 0.05) / sinf(2 * DegreeToRad(angle));
	stars.back()->SetSpeed(speed);
}
void Stage::BlockGenerateStar(const Vector3& pos, const int& num)
{
	for (int i = 0; i < num; i++)
	{
		float angle = Random::Range(30, 150);
		stars.emplace_back(move(make_unique<Star>()));
		stars.back()->Generate(pos, { cosf(angle),sinf(angle),0 }, 1);
		stars.back()->SetSpeed(Random::RangeF(0.5, 1.5));
	}
}

// 自機
void Stage::PlayerUpdate()
{
	// 星をつぶす処理
	for (const auto& temp : stars)
	{
		if (temp->revival->isRevival == false)
		{
			float tmepRadius = 0;
			if (player->GetisWeakAttack() == true)
			{
				tmepRadius = 2;
			}
			if (player->GetisHeavyAttack() == true)
			{
				tmepRadius = 3;
			}

			if (collision->SphereHitSphere(
				player->GetPos(), tmepRadius, temp->GetPos(), temp->GetRadius() &&
				player->GetisEngulfAttack() == false))
			{
				if (player->GetisGround() == false)
				{
					if (temp->GetisCanHit() == true && temp->GetisDestroy() == false &&
						temp->GetisAttack() == false && temp->GetisGround() == true)
					{
						player->HaveStarNumIncriment();

						if (temp->GetisChangeColor() == false)
						{
							ground->Damage(player->GetHaveStarNum() * 5);
							viewProjection_.SetShakeValue(2, 20, 2);
						}
						else
						{
							ground->Damage(player->GetHaveStarNum() * 10);
							viewProjection_.SetShakeValue(3, 30, 2);
						}
						temp->SetisDestroy(true);
					}
				}
			}
		}
	}

	player->Update();

	//auto text = DebugText::GetInstance();
}

// 床
void Stage::FloorUpdate()
{
	SquareCollider groundCollider =
	{
		{ ground->GetPos().x, ground->GetPos().y },
		{ ground->GetScale().x, ground->GetScale().y },
	};
	if (ground->GetisHit() == 0)
	{
		SquareCollider playerCollider =
		{
			{ player->GetPos().x, player->GetPos().y - player->GetRadius() - player->GetAttackMoveSpeed()},
			{ player->GetScale().x, player->GetScale().y },
		};
		while (collision->SquareHitSquare(playerCollider, groundCollider))
		{
			auto tempPos = player->GetPos();
			tempPos.y -= 0.1f;
			player->SetPos(tempPos);
			player->UpdateMatrix();

			SquareCollider tempCollider =
			{
				{ player->GetPos().x, player->GetPos().y - player->GetRadius() },
				{ player->GetRadius(), player->GetRadius() },
			};
			if (collision->SquareHitSquare(tempCollider, groundCollider))
			{
				player->SetisReverse(true);
				ground->SetisHit(1);
				break;
			}
		}
	}
	if (ground->GetisHit() == 1)
	{
		player->EffectGenerate(
			{
				player->GetPos().x ,
				ground->GetPos().y + ground->GetScale().y,
				player->GetPos().z
			});

		if (player->GetHaveStarNum() > 0)
		{
			ground->LargeDamage(
				player->GetWeakAttackDamage() +
				player->GetHaveStarNum() *
				player->GetStarAttackDamage());
			player->SetHaveStarNum(0);
			ground->SetisHit(2);
		}
		else
		{
			if (player->GetisHeavyAttack())
			{
				PlayerGenerateStar(player->GetPos());
				ground->LargeDamage(player->GetHeavyAttackDamage());
				ground->SetisHit(2);
			}
			else if (player->GetisWeakAttack())
			{
				ground->Damage(player->GetWeakAttackDamage());
				ground->SetisHit(2);
			}
		}
	}
	//if (ground->GetisHit() == 2)
	//{
	//	player->SetisGround(true);
	//}

	if (player->GetPos().y >= 20)
	{
		ground->SetisHit(0);
		player->SetHaveStarNum(0);
	}
	if (player->GetisGround() == true && player->GetisJump() == false &&
		/*player->GetisDown() == false &&*/ player->GetisJumpAddScaleStep() == 0)
	{
		player->SetPos(
			{
				player->GetPos().x,
				ground->GetPos().y + ground->GetScale().y + player->GetRadius() * 2,
				player->GetPos().z
			});
	}
	//if (player->GetisDown() == false)
	//{
	//	player->SetPos(
	//		{
	//			player->GetPos().x,
	//			ground->GetPos().y + ground->GetScale().y + player->GetRadius() * 2,
	//			player->GetPos().z
	//		});
	//}

	//// 大きくなる処理
	//if (stageType != RaceStage)
	//{
	//	const int starSize = 5;
	//	if (stars.size() >= starSize && ground->GetisAddScaleCountDown() == 0)
	//	{
	//		ground->SetisAddScaleCountDown(1);
	//	}
	//	if (ground->GetisAddScaleCountDown() == 1)
	//	{
	//		for (const auto& temp : stars)
	//		{
	//			temp->SetisAngleShacke(true);
	//		}
	//	}
	//	if (stars.size() < starSize)
	//	{
	//		ground->SetisAddScaleCountDown(0);
	//		ground->SetisSuctionStar(false);
	//		for (const auto& temp : stars)
	//		{
	//			temp->SetisAngleShacke(false);
	//		}
	//	}

	//	// 星を吸収する処理
	//	if (ground->GetisSuctionStar() == true)
	//	{
	//		for (const auto& temp : stars)
	//		{
	//			repairEffect->Generate(temp->GetPos());
	//		}
	//		stars.clear();
	//		ground->SetisSuctionStar(false);
	//	}

	//	// 八個集まったか
	//	if (stars.size() >= starSize - 2)
	//	{
	//		ground->SetisDanger(true);
	//	}
	//	else
	//	{
	//		ground->SetisDanger(false);
	//	}
	//}

	ground->Update();
}

// 星
void Stage::StarUpdate()
{
	SquareCollider floorCollider =
	{
		{ ground->GetPos().x,ground->GetPos().y },
		{ ground->GetScale().x,ground->GetScale().y },
	};

	for (const auto& tempStar : stars)
	{
		if (tempStar->revival->isRevival == false)
		{
			SquareCollider starCollider =
			{
				{ tempStar->GetPos().x,tempStar->GetPos().y },
				{ tempStar->GetScale().x,1.5 },
			};
			if (ground->GetHP() > 0)
			{
				if (stageType == RaceStage)
				{
					if (tempStar->GetPos().y - 1.5 <= ground->GetPos().y + ground->GetScale().y)
					{
						tempStar->SetPos(
							{
								tempStar->GetPos().x,
								ground->GetPos().y + ground->GetScale().y + 1.5f,
								tempStar->GetPos().z,
							});
					}
				}

				if (collision->SquareHitSquare(starCollider, floorCollider) && tempStar->GetisCanHit() == true)
				{
					tempStar->SetPos(
						{
							tempStar->GetPos().x,
							ground->GetPos().y + ground->GetScale().y + 1.5f,
							tempStar->GetPos().z,
						});
					tempStar->SetGravity(0);
					if (tempStar->GetGenerateType() == 1)
					{
						tempStar->SetSpeed(0);
						tempStar->SetisGround(true);
					}
					if (tempStar->GetisAttack() == true)
					{
						tempStar->SetisDestroy(true);
						tempStar->SetisAttack(false);
						tempStar->SetSpeed(0);

						//windPressureEffect->Generate(tempStar->GetPos(), tempStar->GetDir());
						if (tempStar->GetisChangeColor() == false)
						{
							ground->Damage(player->GetStarAttackDamage());
							viewProjection_.SetShakeValue(2, 20, 2);
						}
						else
						{
							ground->Damage(player->GetStarAttackDamage() * 2);
							viewProjection_.SetShakeValue(3, 30, 2);
						}

					}
				}
			}

			for (const auto& tempBlock : blocks)
			{
				SquareCollider blockCollider =
				{
					{ tempBlock->GetPos().x,tempBlock->GetPos().y },
					{ tempBlock->GetScale().x,tempBlock->GetScale().y },
				};

				if (collision->SquareHitSquare(starCollider, blockCollider) &&
					tempBlock->revival->isRevival == false)
				{
					tempStar->SetSpeed(0);
				}
			}
		}
	}

	WaveUpdate();

	for (const auto& temp : stars)
	{
		temp->Update();
		temp->UpdateEffect();
	}

	for (const auto& temp : stars)
	{
		if (temp->GetisDestroy() == true)
		{
			if (stageType != RaceStage)
			{
				// 画面外の時
				if (temp->GetPos().x >= 40 || temp->GetPos().x <= -40)
				{
					if (temp->GetisSucked() == false)
					{
						//grainScatterEffect->Generate(temp->GetPos());
						//repairEffect->Generate(temp->GetPos());
						stars.remove(temp);
						break;
					}
				}
				// 普通の時
				else
				{
					if (temp->revival->isRevival == false)
					{
						if (temp->suckedCurve->GetisEnd() == true && ground->GetHP() > 0)
						{
							repairEffect->Generate(temp->GetPos());
						}
						else
						{
							grainScatterEffect->Generate(temp->GetPos(), temp->isChangeColor);
						}
					}

					if (temp->revival->isCanRevival == true)
					{
						temp->revival->isRevival = true;
						temp->SetisDestroy(false);
					}
					else
					{
						stars.remove(temp);
						break;
					}
				}
			}
			// レースステージの時
			else
			{
				grainScatterEffect->Generate(temp->GetPos(), temp->isChangeColor);
				stars.remove(temp);
				break;
			}
		}
	}
}

// トゲ
void Stage::ThornUpdate()
{
	SquareCollider playerCollider =
	{
		{ player->GetPos().x,player->GetPos().y },
		{ player->GetScale().x,player->GetScale().y },
	};

	SquareCollider floorCollider =
	{
		{ ground->GetPos().x,ground->GetPos().y },
		{ ground->GetScale().x,ground->GetScale().y },
	};

	for (const auto& temp : thorns)
	{
		SquareCollider thornCollider =
		{
			{ temp->GetPos().x,temp->GetPos().y },
			{ temp->GetRadius(),temp->GetRadius() },
		};
		if (ground->GetHP() > 0)
		{
			if (collision->SquareHitSquare(thornCollider, floorCollider))
			{
				temp->SetPos(
					{
						temp->GetPos().x,
						ground->GetPos().y + ground->GetScale().y + 1.5f,
						temp->GetPos().z,
					});
			}
		}

		if (collision->SquareHitSquare(playerCollider, thornCollider))
		{
			player->SetisDamage(true);
		}

		for (const auto& tempBlock : blocks)
		{
			if (collision->SphereHitSphere(temp->GetPos(), 0.5, tempBlock->GetPos(), 1.9))
			{
				if (temp->GetPos().y < tempBlock->GetPos().y)
				{
					tempBlock->SetPos(
						{
							tempBlock->GetPos().x,
							temp->GetPos().y + 4,
							tempBlock->GetPos().z,
						});
				}
			}
		}
	}

	for (const auto& temp : thorns)
	{
		temp->Update();
	}
}

// ブロック
void Stage::BlockUpdate()
{
	if (blocks.size() <= 0)
	{
		player->SetSpeed(0.25);
	}

	SquareCollider playerCollider =
	{
		{ player->GetPos().x, player->GetPos().y - player->GetRadius() - player->GetAttackMoveSpeed()},
		{ player->GetRadius(), player->GetRadius() - 0.25f},
	};
	SquareCollider floorCollider =
	{
		{ ground->GetPos().x,ground->GetPos().y },
		{ ground->GetScale().x,ground->GetScale().y },
	};

	// 当たり判定
	for (const auto& temp : blocks)
	{
		SquareCollider blockCollider =
		{
			{ temp->GetPos().x,temp->GetPos().y },
			{ temp->GetScale().x,temp->GetScale().y },
		};

		// プレイヤー
		if (temp->GetisHit() == 0 && player->GetisGround() == false)
		{
			// 上から下の当たり判定
			while (collision->SquareHitSquare(playerCollider, blockCollider))
			{
				auto tempPos = player->GetPos();
				tempPos.y -= 0.1f;
				player->SetPos(tempPos);
				player->UpdateMatrix();

				SquareCollider tempCollider =
				{
					{ player->GetPos().x, player->GetPos().y - player->GetRadius() },
					{ player->GetRadius(), player->GetRadius() },
				};
				if (collision->SquareHitSquare(tempCollider, blockCollider))
				{
					player->SetisReverse(true);
					temp->SetisHit(1);
					break;
				}
			}
		}
		if (temp->GetisHit() == 1)
		{
			if (player->GetisWeakAttack() == true)
			{
				temp->Damage(player->GetWeakAttackDamage());
				temp->SetisHit(2);
			}
			if (player->GetisHeavyAttack() == true)
			{
				temp->Damage(player->GetHeavyAttackDamage());
				temp->SetisHit(2);
			}
		}
		if (player->GetisJump() == true && player->GetisHitBlock() == false)
		{
			SquareCollider playerCollider =
			{
				{ player->GetPos().x, player->GetPos().y - player->GetRadius() + player->GetAttackMoveSpeed()},
				{ player->GetRadius(), player->GetRadius() - 0.25f},
			};

			// 下から上の当たり判定
			while (collision->SquareHitSquare(playerCollider, blockCollider))
			{
				auto tempPos = player->GetPos();
				tempPos.y += 0.1f;
				player->SetPos(tempPos);
				player->UpdateMatrix();

				SquareCollider tempCollider =
				{
					{ player->GetPos().x, player->GetPos().y - player->GetRadius() },
					{ player->GetRadius(), player->GetRadius() },
				};
				if (collision->SquareHitSquare(tempCollider, blockCollider))
				{
					player->SetisHitBlock(true);
					//temp->SetisHit(3);
					break;
				}
			}
		}

		// 地面
		if (collision->SquareHitSquare(floorCollider, blockCollider))
		{
			temp->SetPos(
				{
					temp->GetPos().x,
					ground->GetPos().y + ground->GetScale().y + temp->GetScale().y,
					temp->GetPos().z,
				});
		}

		if (player->GetPos().y >= 20 && temp->GetisHit() == 2)
		{
			temp->SetisHit(0);
		}


		for (const auto& temp2 : blocks)
		{
			if (temp == temp2) break;

			SquareCollider blockCollider2 =
			{
				{ temp2->GetPos().x,temp2->GetPos().y },
				{ temp2->GetScale().x,1.5f },
			};

			if (collision->SquareHitSquare(blockCollider, blockCollider2))
			{
				if (blockCollider.pos.y >= blockCollider2.pos.y)
				{
					temp->SetPos(
						{
							temp->GetPos().x,
							temp2->GetPos().y + 1,
							temp->GetPos().z,
						});
				}
				else if (blockCollider.pos.y < blockCollider2.pos.y)
				{
					temp2->SetPos(
						{
							temp->GetPos().x,
							temp2->GetPos().y + 1,
							temp->GetPos().z,
						});
				}
			}
		}
		if (collision->SphereHitSphere(player->GetPos(), 1.5, temp->GetPos(), 2) ||
			collision->SphereHitSphere({ player->GetPos().x + 3,player->GetPos().y,player->GetPos().z }, 1, temp->GetPos(), 2))
		{
			if (temp->revival->isRevival == false)
			{
				player->SetSpeed(0);
				break;
			}
		}
		else
		{
			player->SetSpeed(0.25);
		}
	}

	// 更新処理
	for (const auto& temp : blocks)
	{
		temp->Update();
	}

	// 削除処理
	for (const auto& temp : blocks)
	{
		if (temp->GetisDestroy() == true)
		{
			if (temp->revival->isCanRevival == true)
			{
				if (temp->GetHaveStar() == true &&
					temp->revival->isRevival == false)
				{
					BlockGenerateStar(temp->GetPos(), 5);
				}

				temp->revival->isRevival = true;
				temp->SetisDestroy(false);
			}
			else
			{
				if (temp->GetHaveStar() == true)
				{
					BlockGenerateStar(temp->GetPos(), 5);
				}

				blocks.remove(temp);
				break;
			}

		}
	}
}

// 大砲
void Stage::CannonUpdate()
{
	int count = 0;
	float speed = 0;
	for (const auto& temp : cannons)
	{
		if (temp->GetisShot() == true)
		{
			if (count == 0)
			{
				CannonGenerateStar(
					{ temp->GetPos().x - 12,temp->GetPos().y + 10,temp->GetPos().z }, temp->GetDirVec(), 135);
			}
			else if (count == 1)
			{
				CannonGenerateStar(
					{ temp->GetPos().x + 12,temp->GetPos().y + 10,temp->GetPos().z }, temp->GetDirVec(), 45);
			}
			temp->SetisShot(false);
			count++;
		}
	}

	for (const auto& temp : cannons)
	{
		temp->Update();
	}
}

// レース
void Stage::RaceUpdate()
{
	ground->SetPos(
		{
			ground->GetPos().x + player->GetSpeed() * SlowMotion::GetInstance()->GetSlowExrate(),
			ground->GetPos().y,
			ground->GetPos().z,
		});

	if (player->GetPos().x >= 92)
	{
		lineTrans->translation_.x = 184;
		lineTrans->UpdateMatrix();
	}
	if (player->GetPos().x >= 184)
	{
		lineTrans2->translation_.x = 276;
		lineTrans2->UpdateMatrix();
	}
	if (player->GetPos().x >= 276)
	{
		lineTrans->translation_.x = 368;
		lineTrans->UpdateMatrix();
	}

	if (player->GetPos().x >= goal->GetPos().x - goal->GetScale().x)
	{
		playerIsHitGoal = true;
	}

	if (goal != nullptr)
	{
		goal->Update();
	}
}

// 耐久戦
void Stage::EnduranceUpdate()
{
	if (isGetTime == 0)
	{
		if (ground->GetPos().y + ground->GetScale().y >= -3.85)
		{
			isGetTime = 1;
			enduranceStartTime = GetNowTime() / 100;
		}
	}
	if (isGetTime == 1 && ground->GetPos().y + ground->GetScale().y <= 0)
	{
		enduranceNowTime = GetNowTime() / 100;
		enduranceEndTime = enduranceNowTime - enduranceStartTime;
		if (enduranceEndTime == enduranceTime)
		{
			isGetTime = 2;
		}
	}

	if (GetDightsNumber(enduranceTime - enduranceEndTime).size() == 2)
	{
		enduranceTimeDightsNumber[0] = GetDightsNumber(enduranceTime - enduranceEndTime)[0];
		enduranceTimeDightsNumber[1] = GetDightsNumber(enduranceTime - enduranceEndTime)[1];
	}
	else if (GetDightsNumber(enduranceTime - enduranceEndTime).size() == 1)
	{
		enduranceTimeDightsNumber[0] = 0;
		enduranceTimeDightsNumber[1] = GetDightsNumber(enduranceTime - enduranceEndTime)[0];
	}
	else if (GetDightsNumber(enduranceTime - enduranceEndTime).size() == 0)
	{
		enduranceTimeDightsNumber[0] = 0;
		enduranceTimeDightsNumber[1] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		enduranceTimeSprites[i]->SetTextureHandle(numberSheet[enduranceTimeDightsNumber[i]]);
	}
}

// 波動
void Stage::WaveUpdate()
{
	for (int i = 0; i < windPressureEffect->waves.size(); i++)
	{
		for (const auto& temp : stars)
		{
			if (collision->SphereHitSphere(
				windPressureEffect->waves[i]->GetPos(), 0.75, temp->GetPos(), 1.5))
			{
				if (/*temp->GetisCanHit() == true &&  */
					temp->GetisGround() == true &&
					temp->GetisDestroy() == false)
				{
					temp->SetGravity(1);
					temp->SetisAttack(true);
					temp->SetSpeed(0);
					int dir = 0;
					if (temp->GetPos().x - windPressureEffect->waves[i]->GetPos().x >= 0)
					{
						dir = 1;
					}
					else
					{
						dir = -1;
					}

					temp->SetDir(dir);
					temp->SetDirVec(
						{
							dir * cosf(DegreeToRad(75)),
							cosf(DegreeToRad(75)),
							0
						});
				}
			}
		}
	}

}