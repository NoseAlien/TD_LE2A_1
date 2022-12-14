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
vector<uint32_t> Stage::ruleTex = {};
vector<uint32_t> Stage::ruleStrTex = {};
uint32_t Stage::tutorial1Tex = 0;
uint32_t Stage::tutorial2Tex = 0;
uint32_t Stage::arrowkeyTutorialTex = 0;

uint32_t Stage::overStrTexture;
uint32_t Stage::gameOverBGM;
uint32_t Stage::gameClearBGM;
uint32_t Stage::grainDiedSE;

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
		enduranceTimeSprites[i] = Sprite::Create(numberSheet[i], { 1820,64 });
		enduranceTimeSprites[i]->SetPosition({ (float)(1792 - ((1 - i) * 96)),128 });
		enduranceTimeSprites[i]->SetAnchorPoint({ 0.5, 0.5 });
		enduranceTimeSprites[i]->SetSize({ 128,128 });
	}

	stageNumberSprite.reset(Sprite::Create(stageNumberTextures[stageNumber], { 960,270 }));
	stageNumberSprite->SetAnchorPoint({ 0.5f,0.5f });

	grainScatterEffect = move(make_unique<GrainScatterEffect>());
	repairEffect = move(make_unique<RepairEffect>());
	breakBlockEffect = move(make_unique<BreakBlockEffect>());

	damageEffect = DamageEffect::GetInstance();
	windPressureEffect = WindPressureEffect::GetInstance();

	// ???[?????A
	ruleSprite.reset(Sprite::Create(ruleTex[0], { 960,675 }));
	ruleSprite->SetAnchorPoint({ 0.5f,0.5f });
	ruleStrSprite.reset(Sprite::Create(ruleStrTex[0], { 214,178 }));
	ruleStrSprite->SetAnchorPoint({ 0.5f,0.5f });

	tutorial1Sprite.reset(Sprite::Create(tutorial1Tex, { 1838,622 }));
	tutorial1Sprite->SetAnchorPoint({ 1,1 });

	tutorial2Sprite.reset(Sprite::Create(tutorial2Tex, { 2200,600 }));
	tutorial2Sprite->SetAnchorPoint({ 0.5f,0.5f });
	tutorial2Sprite->SetSize({ 172,166 });

	//arrowkeyTutorialSprite.reset(Sprite::Create(arrowkeyTutorialTex, { 166,288 }));
	arrowkeyTutorialSprite.reset(Sprite::Create(arrowkeyTutorialTex, { 960,880 }));
	arrowkeyTutorialSprite->SetAnchorPoint({ 0.5f,0.5f });

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
	for (int i = 0; i < 6; i++)
	{
		ruleTex.push_back(TextureManager::Load("SpriteTexture/rule/rule_" + to_string(i) + ".png"));
		ruleStrTex.push_back(TextureManager::Load("SpriteTexture/rule/rule_str_" + to_string(i) + ".png"));
	}

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

	for (int i = 0; i <= 13; i++)
	{
		stageNumberTextures.push_back(
			TextureManager::Load(
				"SpriteTexture/select_nuber/stage_number_" + to_string(i) + ".png"));
	}

	tutorial1Tex = TextureManager::Load("SpriteTexture/tutorial_1.png");
	tutorial2Tex = TextureManager::Load("SpriteTexture/tutorial_2.png");

	arrowkeyTutorialTex = TextureManager::Load("SpriteTexture/arrowkey.png");

	gameOverBGM = Audio::GetInstance()->LoadWave("bgm/gameover.wav");
	gameClearBGM = Audio::GetInstance()->LoadWave("bgm/clear.wav");
	grainDiedSE = Audio::GetInstance()->LoadWave("se/bonn.wav");
}
void Stage::UnLoad()
{
	delete lineModel;
}

void Stage::Init()
{
	// ???????A
	tutorial1Sprite->SetSize({ 0,0 });
	tutorial2Sprite->SetPosition({ 2200,600 });
	tutorialMoveEase.ReSet();
	tutorialMoveEase.SetEaseTimer(60);
	tutorialMoveEase.SetPowNum(5);
	tutorialScaleEase.ReSet();
	tutorialScaleEase.SetEaseTimer(60);
	tutorialScaleEase.SetPowNum(5);

	tutorialRule.ReSet();
	tutorialRule.SetEaseTimer(60);
	tutorialRule.SetPowNum(5);

	tutorialAttackCount = 0;
	changeTutorialTexCount = 0;

	// ?G???h???X???A???J?E???g
	isEndlessCountDown = false;
	endlessCountDownIndex = 0;

	damageEffect->Clear();
	windPressureEffect->Clear();

	// ???C?????A
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

	// ?X?^?[?g?J?E???g???A
	startTextIndex = 0;
	startTextTimer = 0;
	startTextMaxTimer = 80;
	startTextExrate = 0;
	startTextAngle = -180;
	startTextAlpha = 1;
	//isStartTextEnd = false;
	isStartTextEnd = true;
	stagePcrogress = Start;

	// ?N???A???A
	startTime = 0;
	endTime = 0;
	clearTime = 0;
	isShowClearTime = false;
	clearTimeLastDightPos = { 2500,952 };
	isMoveClearTime = false;

	fastTimeLastDightPos = { 2500,952 };

	isCameraMoveStep = false;
	cameraMoveVec = { 0,0,0 };

	isPlayerDieEffectGenerate = false;

	// ???v?????A
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

	// ?X?e?[?W?\?????A
	isShowStageNumber = true;
	sizeExrate = 0;
	rotAngel = 0;
	alpha = 1;
	stageNumberSprite->SetPosition({ 960,270 });
	stageNumberSprite->SetSize({ 0,0 });
	stageNumberSprite->SetRotation(0);
	stageNumberSprite->SetColor({ 1,1,1,1 });

	// ???[?????A
	ruleSprite->SetPosition({ 960,675 });
	ruleSprite->SetSize({ 0,0 });
	ruleSprite->SetRotation(0);
	ruleSprite->SetColor({ 1,1,1,1 });

	ruleStrEase.ReSet();
	ruleStrEase.SetEaseTimer(60);
	ruleStrEase.SetPowNum(5);
	ruleStrSprite->SetSize({ 0,0 });

	arrowkeyTutorialSprite->SetPosition({ 960,880 });
	arrowkeyTutorialSprite->SetSize({ 0,0 });

	// ?????????A
	overStrSprite->SetPosition({ 960,1500 });

	// ???????C?????????????A
	isOverLine = false;
	overLineEase.SetPowNum(5);
	overLineEase.SetEaseTimer(60);
	goundeTempScaleY = 0;
	overLineEase.ReSet();

	// ?G???h???X
	//isEndless = false;
	endlessAttackCount = 0;
	endlessAttackMaxCount = 60;
}
void Stage::Update()
{
	//if (gameClear == true || gameOver == true) return;
	//if (stagePcrogress == End) return;

	// ???[?????A
	if (stagePcrogress == Start)
	{
		if (isEndurance == true)
		{
			ruleSprite->SetTextureHandle(ruleTex[1]);
			ruleStrSprite->SetTextureHandle(ruleStrTex[1]);
		}
		else
		{
			if (stageType == BaseStage || stageType == CannonStage)
			{
				ruleSprite->SetTextureHandle(ruleTex[0]);
				ruleStrSprite->SetTextureHandle(ruleStrTex[0]);
			}
			if (stageType == RaceStage)
			{
				ruleSprite->SetTextureHandle(ruleTex[2]);
				ruleStrSprite->SetTextureHandle(ruleStrTex[2]);
			}
			if (stageType == TutorialStage)
			{
				ruleSprite->SetTextureHandle(ruleTex[changeTutorialTexCount + 3]);
				ruleStrSprite->SetTextureHandle(ruleStrTex[changeTutorialTexCount + 3]);

				//if (changeTutorialTexCount == 0)
				//{
				//	ruleSprite->SetTextureHandle(ruleTex[3]);
				//	ruleStrSprite->SetTextureHandle(ruleStrTex[3]);
				//}
				//else if (changeTutorialTexCount == 1)
				//{
				//	ruleSprite->SetTextureHandle(ruleTex[4]);
				//	ruleStrSprite->SetTextureHandle(ruleStrTex[4]);
				//}
				//else if (changeTutorialTexCount == 2)
				//{
				//	ruleSprite->SetTextureHandle(ruleTex[5]);
				//	ruleStrSprite->SetTextureHandle(ruleStrTex[5]);
				//}
			}
		}
	}

	if (sceneChange->GetisSceneChangeNow() == false)
	{
		// ?X?e?[?W?\??
		ShowStageNumberUpdate();

		// ?J?E???g?_?E??????
		CountDownUpdate();
	}

	if (stagePcrogress == Play || stagePcrogress == Staging)
	{
		//// ?????????[??
		//if (ruleStrEase.GetisEnd() == false)
		//{
		//	ruleStrEase.Update();
		//	ruleStrSprite->SetSize(ruleStrEase.Out({ 0,0 }, { 300,45 }));
		//}

		// ?E?[???????R?c?u
		if (isEndurance == true)
		{
			if (tutorialMoveEase.GetisEnd() == false)
			{
				tutorialMoveEase.Update();
				tutorial2Sprite->SetPosition(tutorialMoveEase.Out({ 2200,600 }, { 1880, 600 }));
			}
			if (tutorialMoveEase.GetisEnd() == true && tutorialScaleEase.GetisEnd() == false)
			{
				tutorialScaleEase.Update();
				tutorial1Sprite->SetSize(tutorialScaleEase.Out({ 0,0 }, { 190,150 }));
			}
		}

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

		// ?Q?[???N???A????
		if (ground->GetHP() <= 0 || isGetTime == 2)
		{
			if (stagePcrogress == Play)
			{
				if (Audio::GetInstance()->IsPlaying(GameScene::bgmPlaying) == true)
				{
					Audio::GetInstance()->StopWave(GameScene::bgmPlaying);
					Audio::GetInstance()->PlayWave(gameClearBGM);
				}
				stagePcrogress = Staging;
				SlowMotion::GetInstance()->StartSlowMotion(0.05, 280);
				if (isEndurance == false)
				{
					endTime = GetNowTime();
					clearTime = endTime - startTime;
					if (fastClearTime > clearTime)
					{
						fastClearTime = clearTime;
					}
				}
				isMoveClearTime = true;
				gameClear = true;
			}

			// ???X
			if (stagePcrogress == Staging &&
				SlowMotion::GetInstance()->GetisSlowMotion() == false)
			{
				stagePcrogress = End;
				sceneChange->StartSceneChange();
			}
		}

		// ?Q?[???I?[?o?[????
		if (ground->GetPos().y + ground->GetScale().y >= 0 ||
			player->GetLife() <= 0 || playerIsHitGoal == true)
		{
			if (stagePcrogress == Play)
			{
				if (Audio::GetInstance()->IsPlaying(GameScene::bgmPlaying) == true)
				{
					Audio::GetInstance()->StopWave(GameScene::bgmPlaying);
					Audio::GetInstance()->PlayWave(gameOverBGM);
				}
				stagePcrogress = Staging;
				if (player->GetLife() <= 0 || playerIsHitGoal == true)
				{
					player->SetLife(0);
					isCameraMoveStep = true;
				}
				if (ground->GetPos().y + ground->GetScale().y >= 0 && isOverLine == false)
				{
					isOverLine = true;
					goundeTempScaleY = ground->GetScale().y / 10;
				}
				//endTime = GetNowTime();
				//clearTime = endTime - startTime;
				//if (isEndurance == true)
				//{
				//	endTime = GetNowTime();
				//	clearTime = endTime - startTime;
				//	if (fastClearTime > clearTime)
				//	{
				//		fastClearTime = clearTime;
				//	}
				//	isMoveClearTime = true;
				//	gameClear = true;
				//}
				//else
				//{
				//}
				gameOver = true;
			}
			//if (player->GetLife() <= 0 || playerIsHitGoal == true)
			//{
			//}
		}
		if (stageType == RaceStage && !isCameraMoveStep)
		{
			viewProjection_.eyePos = { player->GetPos().x,0,-50 };
			viewProjection_.targetPos = { player->GetPos().x ,0,0 };
		}
		if (player->GetDieEffectisEnd() == true ||
			overLineEase.GetisEnd() == true)
		{
			overScreenClock++;

			float clearStrPosY = lerp(1500.0f, 400, pow((overScreenClock - 50) / 30.0, 0.2));

			overStrSprite->SetPosition({ 960,clearStrPosY });
			overStrSprite->SetSize({ 768,768 });

			//// ?G???h???X????
			//if (isEndurance == true)
			//{
			//	clearTimeLastDightPos.x = 960 + ((dightsNumber.size() + 1) * 48 + 128) / 2;
			//	clearTimeLastDightPos.y = lerp(1700, 480, pow((clearScreenClock - 125) / 30.0, 0.2));
			//	// ?N???A?^?C??
			//	for (int i = 0; i < dightsNumber.size(); i++)
			//	{
			//		clearTimeSprites[i]->SetTextureHandle(numberSheet[dightsNumber[i]]);
			//		clearTimeSprites[i]->SetPosition(
			//			{
			//				clearTimeLastDightPos.x - (dightsNumber.size() - (float)i) * 48,
			//				clearTimeLastDightPos.y
			//			});
			//		if (i == dightsNumber.size() - 2)
			//		{
			//			dotStrSprite->SetPosition(
			//				{
			//					clearTimeLastDightPos.x - (dightsNumber.size() - (float)i) * 48,
			//					clearTimeLastDightPos.y
			//				});
			//		}
			//	}
			//	timeStrSprite->SetPosition(
			//		{
			//			clearTimeLastDightPos.x - dightsNumber.size() * 48 - 128,
			//			clearTimeLastDightPos.y
			//		});

			//	if (overScreenClock >= 260)
			//	{
			//		stagePcrogress = End;
			//		sceneChange->StartSceneChange();
			//		player->SetDieEffectisEnd(false);
			//		overScreenClock = 0;
			//	}
			//}
			//else
			//{
			if (overScreenClock >= 160)
			{
				stagePcrogress = End;
				sceneChange->StartSceneChange();
				player->SetDieEffectisEnd(false);
				overScreenClock = 0;
			}
			//}
		}
	}

	GameOverCameraUpdate();
	GroundOverLineUpdate();
	ClearTimeUpdate();

	viewProjection_.ShakeUpdate();
	viewProjection_.UpdateMatrix();
	player->EffectUpdate();
	ground->EffectUpdate();
	damageEffect->Update();
	windPressureEffect->Update();
	grainScatterEffect->Update();
	repairEffect->Update();
	breakBlockEffect->Update();
}
void Stage::Draw()
{
	lineModel->Draw(*lineTrans, viewProjection_);
	lineModel->Draw(*lineTrans2, viewProjection_);

	//if (stageType == RaceStage)
	//{
	//	lineModel->Draw(*goalLineTrans, viewProjection_);
	//}

	//if (isEndurance == true)
	//{
	//	lineModel->Draw(*enduranceLineTrans, viewProjection_, lineModelTexture);
	//}

	if (!isPlayerDieEffectGenerate)
	{
		player->Draw(viewProjection_);
	}

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

	//if (goal != nullptr)
	//{
	//	goal->Draw(viewProjection_);
	//}

	ground->EffectDraw();

	breakBlockEffect->Draw();
}
void Stage::DrawSprite()
{
	ground->DrawSprite();

	// ???v???^?C?}?[
	if (isEndurance == true)
	{
		for (int i = 0; i < 2; i++)
		{
			enduranceTimeSprites[i]->Draw2();
		}
	}

	// ?X?e?[?W?\??
	if (isShowStageNumber == true)
	{
		if (stageType == TutorialStage)
		{
			if (changeTutorialTexCount == 0)
			{
				stageNumberSprite->Draw2();
			}
		}
		else
		{
			stageNumberSprite->Draw2();
		}
	}
	// ???[??
	ruleStrSprite->Draw2();
	ruleSprite->Draw2();

	// ????
	if (isEndurance == true)
	{
		tutorial2Sprite->Draw();
		tutorial1Sprite->Draw();
		arrowkeyTutorialSprite->Draw();
	}

	// ?N???A?`??
	if (gameClear)
	{
		clearStrSprite->Draw2();
		if (isEndurance == false && stageType != TutorialStage)
		{
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
	}

	if (gameOver)
	{
		overStrSprite->Draw2();
	}
}
void Stage::DrawEffectFront()
{
	// ?J?E???g?_?E??
	if (isShowStageNumber == false)
	{
		startTextSprites[3]->Draw2();
	}
	//if (enduranceTimeDightsNumber.back() - 1 < 3 &&
	//	enduranceTimeDightsNumber.back() - 1 >= 0)
	//{
	//	startTextSprites[enduranceTimeDightsNumber.back() - 1]->Draw2();
	//}

	grainScatterEffect->Draw();
	repairEffect->Draw();
	windPressureEffect->Draw();
	player->DrawSpriteFront();
	damageEffect->Draw();
}
void Stage::DrawEffectBack()
{
	player->DrawSpriteBack();
	for (const auto& temp : stars)
	{
		temp->DrawEffectBack();
	}
}
void Stage::DrawBackLight()
{
	for (const auto& temp : stars)
	{
		temp->DrawBackLight();
	}
	player->DrawBackLight();
}

void Stage::ShowStageNumberUpdate()
{
	static Vector2 numberNowSize = {};
	static Vector2 strNowSize = {};
	static Vector2 arrowkeyNowSize = {};
	if (isShowStageNumber == true)
	{
		arrowkeyTutorialSprite->SetTextureRect({ 0,0 }, { 490,80 });
		//arrowkeyTutorialSprite->SetPosition({ 960,880 });

		// ?X?e?[?W?i???o?[
		if (sizeExrate < 2)
		{
			sizeExrate += 0.04f;
		}
		if (sizeExrate >= 2)
		{
			sizeExrate += 0.001f;
		}

		stageNumberSprite->SetSize({ 600 * sizeExrate,140 * sizeExrate });
		ruleSprite->SetSize({ 320 * sizeExrate,250 * sizeExrate });
		arrowkeyTutorialSprite->SetSize({ 245 * sizeExrate,40 * sizeExrate });

		rotAngel += 7.2f;
		if (rotAngel >= 360)
		{
			rotAngel = 360;
		}
		stageNumberSprite->SetRotation(DegreeToRad(rotAngel));
		ruleSprite->SetRotation(DegreeToRad(rotAngel));
		arrowkeyTutorialSprite->SetRotation(DegreeToRad(rotAngel));

		numberNowSize = stageNumberSprite->GetSize();
		strNowSize = ruleSprite->GetSize();
		arrowkeyNowSize = arrowkeyTutorialSprite->GetSize();

		if (sizeExrate >= 2.00005 && rotAngel == 360)
		{
			static int timer = 0;
			timer++;
			if (timer >= 60)
			{
				tutorialRule.Update();
				stageNumberSprite->SetPosition(tutorialRule.Out({ 960,270 }, { 214,178 }));
				stageNumberSprite->SetSize(tutorialRule.Out(numberNowSize, { 0,0 }));
				ruleSprite->SetPosition(tutorialRule.Out({ 960,675 }, { 214,178 }));
				ruleSprite->SetSize(tutorialRule.Out(strNowSize, { 0,0 }));
				arrowkeyTutorialSprite->SetPosition(tutorialRule.Out({ 960,880 }, { 214,178 }));
				arrowkeyTutorialSprite->SetSize(tutorialRule.Out(arrowkeyNowSize, { 0,0 }));

				if (tutorialRule.GetisEnd())
				{
					isShowStageNumber = false;
					isStartTextEnd = false;
					timer = 0;
				}

				//alpha -= 0.05f;
				//if (alpha <= 0)
				//{
				//	alpha = 0;
				//	isShowStageNumber = false;
				//	isStartTextEnd = false;
				//	timer = 0;
				//}
				//stageNumberSprite->SetColor({ 1,1,1,alpha });
				//ruleSprite->SetColor({ 1,1,1,alpha });
			}
		}
	}
	else
	{
		// ?????????[??
		if (ruleStrEase.GetisEnd() == false)
		{
			ruleStrEase.Update();
			ruleStrSprite->SetSize(ruleStrEase.Out({ 0,0 }, { 300,45 }));
			if (isEndurance == true)
			{
				arrowkeyTutorialSprite->SetTextureRect({ 0,0 }, { 490,200 });
				arrowkeyTutorialSprite->SetPosition({ 166,288 });
				arrowkeyTutorialSprite->SetSize(ruleStrEase.Out({ 0,0 }, { 245,100 }));
			}
		}
	}
}
void Stage::CountDownUpdate()
{
	if (isStartTextEnd == true) return;

	// ?J?E???g
	const float fream = 30;
	if (startTextIndex < 3)
	{
		startTextIndex = 3;
	}

	if (startTextIndex == 3)
	{
		stagePcrogress = Play;
		// ?g????
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

		// ?A???t?@
		startTextAlpha -= 1 / (float)startTextMaxTimer;
		startTextSprites[startTextIndex]->SetColor({ 1,1,1,startTextAlpha });


		if (stageType == TutorialStage)
		{
			if (changeTutorialTexCount == 0 || changeTutorialTexCount == 1)
			{
				startTextTimer++;
				if (startTextTimer >= startTextMaxTimer)
				{
					startTextIndex = 3;
					isStartTextEnd = true;
					startTime = GetNowTime();
				}
			}
			else
			{
				startTextTimer++;
				if (startTextTimer >= startTextMaxTimer)
				{
					startTextIndex = 4;
					isStartTextEnd = true;
					startTime = GetNowTime();
				}
			}
		}
		else
		{
			startTextTimer++;
			if (startTextTimer >= startTextMaxTimer)
			{
				startTextIndex = 4;
				isStartTextEnd = true;
				startTime = GetNowTime();
			}
		}

	}
}
void Stage::ClearTimeUpdate()
{
	if (gameClear)
	{
		clearScreenClock++;

		float clearStrPosY = lerp(1700.0f, 270, pow((clearScreenClock - 50) / 30.0, 0.2));

		clearStrSprite->SetPosition({ 960,clearStrPosY });
		clearStrSprite->SetSize({ 768,768 });

		dightsNumber = GetDightsNumber(clearTime);

		clearTimeLastDightPos.x = 960 + ((dightsNumber.size() + 1) * 48 + 128) / 2;
		clearTimeLastDightPos.y = lerp(1700, 480, pow((clearScreenClock - 125) / 30.0, 0.2));

		// ?N???A?^?C??
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

		// ?????^?C??
		fastTimedightsNumber = GetDightsNumber(fastClearTime);

		fastTimeLastDightPos.x = 960 + ((fastTimedightsNumber.size() + 1) * 48 + 128) / 2;
		fastTimeLastDightPos.y = lerp(1700, 544, pow((clearScreenClock - 150) / 30.0, 0.2));

		// ???????W????
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
void Stage::GroundOverLineUpdate()
{
	if (isOverLine)
	{
		overLineEase.Update();
		ground->SetScale({ 1,overLineEase.In(goundeTempScaleY, 5),1 });
		if (ground->GetScale().y / 10 > 4.95f && overLineEase.GetisEnd() == false)
		{
			viewProjection_.SetShakeValue(5, 10, 3);
		}
	}
}

void Stage::GenerateStar(const Vector3& pos, const bool& alwaysChangeColor)
{
	stars.emplace_back(move(make_unique<Star>()));
	stars.back()->SetAlwaysChangeColor(alwaysChangeColor);
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
void Stage::GenerateThorn(const Vector3& pos, const bool& isReverseVertical,
	const Vector3& rot, const Vector3& scale)
{
	if (isReverseVertical == false)
	{
		thorns.emplace_back(move(make_unique<Thorn>()));
		thorns.back()->Generate(pos, scale, rot);
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
	goalLineTrans = move(make_unique<WorldTransform>());
	goalLineTrans->Initialize();
	goalLineTrans->translation_ = { pos.x,pos.y - 10,pos.z };
	goalLineTrans->rotation_ = { 0,0,DegreeToRad(90) };
	goalLineTrans->UpdateMatrix();
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

// ???@
void Stage::PlayerUpdate()
{
	// ??????????????
	for (const auto& temp : stars)
	{
		if (temp->revival->isRevival == false)
		{
			float tmepRadius = 0;
			if (player->GetisWeakAttack() == true)
			{
				tmepRadius = 3;
			}
			if (player->GetisHeavyAttack() == true)
			{
				tmepRadius = 4;
			}

			if (collision->SphereHitSphere(
				player->GetPos(), tmepRadius, temp->GetPos(), temp->GetRadius() &&
				player->GetisEngulfAttack() == false))
			{
				//if (player->GetisGround() == false)
				//{
				if (temp->GetisCanHit() == true && temp->GetisDestroy() == false &&
					temp->GetisAttack() == false && temp->GetisGround() == true)
				{
					player->HaveStarNumIncriment();

					if (temp->GetisChangeColor() == false)
					{
						if (isEndurance == true)
						{
							endlessAttackCount += player->GetHaveStarNum() * 5;
							//ground->Damage(0);
						}
						else
						{
							if (stageType != TutorialStage)
							{
								ground->Damage(player->GetHaveStarNum() * 5);
							}
						}

						if (stageType != TutorialStage)
						{
							damageEffect->Generate(
								player->GetPos(), GetDightsNumber(player->GetHaveStarNum() * 5));
						}

						viewProjection_.SetShakeValue(1.75f, 20, 2);
					}
					else
					{
						if (isEndurance == true)
						{
							endlessAttackCount += player->GetHaveStarNum() * 10;
							ground->Damage(0);
						}
						else
						{
							if (stageType != TutorialStage)
							{
								ground->Damage(player->GetHaveStarNum() * 10);
							}
						}

						if (stageType != TutorialStage)
						{
							damageEffect->Generate(
								player->GetPos(), GetDightsNumber(player->GetHaveStarNum() * 10));
						}
						viewProjection_.SetShakeValue(2.25f, 30, 2);
					}
					temp->SetisDestroy(true);
				}
				//}
			}
		}
	}

	player->Update();
}

// ??
void Stage::FloorUpdate()
{
	// ??????????????????
	if (stageType == TutorialStage)
	{
		if (ground->GetScale().y >= 13)
		{
			ground->SetisCanAddHP(false);
		}
		else
		{
			ground->SetisCanAddHP(true);
		}
	}
	else
	{
		ground->SetisCanAddHP(true);
	}

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
				player->SetisHitGround(true);
				ground->SetisHit(1);
				if (player->GetisHeavyAttack() == true)
				{
					player->SetisGround(true);
				}
				//if (player->GetisWeakAttack() == true)
				//{
				//	player->SetPos(
				//		{
				//			player->GetPos().x,
				//			ground->GetPos().y + ground->GetScale().y + player->GetRadius(),
				//			player->GetPos().z
				//		});
				//}

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
			if (isEndurance == true)
			{
				endlessAttackCount += player->GetHaveStarNum();
			}
			else
			{
				if (stageType == TutorialStage)
				{
					ground->LargeDamage(0);
				}
				else
				{
					ground->LargeDamage(
						player->GetWeakAttackDamage() +
						player->GetHaveStarNum() *
						player->GetStarAttackDamage());
				}
			}
			player->SetHaveStarNum(0);
			ground->SetisHit(2);
		}
		else
		{
			if (player->GetisHeavyAttack())
			{
				if (stageType == TutorialStage)
				{
					if (changeTutorialTexCount == 1)
					{
						tutorialAttackCount++;
						if (tutorialAttackCount == 6)
						{
							ruleSprite->SetPosition({ 960,675 });
							ruleStrSprite->SetSize({ 0,0 });
							isShowStageNumber = true;
							sizeExrate = 0;
							rotAngel = 0;
							changeTutorialTexCount = 2;
							tutorialRule.ReSet();
							ruleStrEase.ReSet();

							//player->Init();
							stagePcrogress = Start;
							startTextExrate = 0;
							startTextAlpha = 1;
							startTextTimer = 0;
							startTextAngle = -180;
						}
					}
					if (changeTutorialTexCount == 1 || changeTutorialTexCount == 2)
					{
						damageEffect->Generate(player->GetPos(), GetDightsNumber(player->GetHeavyAttackDamage()));
						PlayerGenerateStar(player->GetPos());
						ground->LargeDamage(5);
					}
					else
					{
						ground->LargeDamage(0);
					}
				}
				else
				{
					if (isEndurance == true)
					{
						endlessAttackCount++;
						ground->LargeDamage(0);
					}
					else
					{
						ground->LargeDamage(player->GetHeavyAttackDamage());
					}
					damageEffect->Generate(player->GetPos(), GetDightsNumber(player->GetHeavyAttackDamage()));
					PlayerGenerateStar(player->GetPos());
				}
				ground->SetisHit(2);

			}
			else if (player->GetisWeakAttack())
			{
				if (stageType == TutorialStage)
				{
					// ?`???[?g???A?????A
					if (changeTutorialTexCount == 0)
					{
						tutorialAttackCount++;
						if (tutorialAttackCount == 3)
						{
							ruleSprite->SetPosition({ 960,675 });
							ruleStrSprite->SetSize({ 0,0 });
							isShowStageNumber = true;
							sizeExrate = 0;
							rotAngel = 0;
							changeTutorialTexCount = 1;
							tutorialRule.ReSet();
							ruleStrEase.ReSet();

							player->Init();
							stagePcrogress = Start;
							startTextExrate = 0;
							startTextAlpha = 1;
							startTextTimer = 0;
							startTextAngle = -180;
						}
					}

					if (changeTutorialTexCount == 0)
					{
						ground->Damage(5);
						damageEffect->Generate(player->GetPos(), GetDightsNumber(player->GetWeakAttackDamage()));
					}
					else
					{
						ground->Damage(0);
					}
				}
				else
				{
					if (isEndurance == true)
					{
						ground->Damage(0);
					}
					else
					{
						ground->Damage(player->GetWeakAttackDamage());
					}
					damageEffect->Generate(player->GetPos(), GetDightsNumber(player->GetWeakAttackDamage()));
				}
				ground->SetisHit(2);
			}
		}
	}

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

	if (isEndurance == true)
	{
		if (endlessAttackCount >= endlessAttackMaxCount)
		{
			ground->SubThickness(2);
			endlessAttackMaxCount += addEndlessAttackCount;
			if (endlessAttackMaxCount >= endlessAttackLimitBreaking)
			{
				endlessAttackMaxCount = endlessAttackLimitBreaking;
			}
			endlessAttackCount = 0;
		}
	}

	ground->Update();
}

// ??
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
							// ?`???[?g???A?????A
							if (stageType == TutorialStage)
							{
								if (changeTutorialTexCount == 2)
								{
									tutorialAttackCount++;
									if (tutorialAttackCount == 9)
									{
										ground->SetHP(0);
									}
									ground->Damage(5);
									damageEffect->Generate(
										tempStar->GetPos(), GetDightsNumber(player->GetStarAttackDamage()));
									viewProjection_.SetShakeValue(1.75f, 20, 2);
								}
								else
								{
									ground->Damage(0);
								}

							}
							else
							{
								if (isEndurance == true)
								{
									endlessAttackCount += 5;
									ground->Damage(0);
								}
								else
								{
									ground->Damage(player->GetStarAttackDamage());
								}
								damageEffect->Generate(
									tempStar->GetPos(), GetDightsNumber(player->GetStarAttackDamage()));
								viewProjection_.SetShakeValue(1.75f, 20, 2);
							}

						}
						else
						{
							// ?`???[?g???A?????A
							if (stageType == TutorialStage)
							{
								if (changeTutorialTexCount == 2)
								{
									tutorialAttackCount++;
									if (tutorialAttackCount == 9)
									{
										ground->SetHP(0);
									}
									ground->Damage(5);
									damageEffect->Generate(
										tempStar->GetPos(), GetDightsNumber(player->GetStarAttackDamage()));
									viewProjection_.SetShakeValue(1.75f, 20, 2);
								}
								else
								{
									ground->Damage(0);
								}
							}
							else
							{
								if (isEndurance == true)
								{
									endlessAttackCount += 10;
									ground->Damage(0);
								}
								else
								{
									ground->Damage(player->GetStarAttackDamage() * 2);
								}
								damageEffect->Generate(
									tempStar->GetPos(), GetDightsNumber(player->GetStarAttackDamage() * 2));
								viewProjection_.SetShakeValue(2.25f, 30, 2);
							}
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
				// ?????O????
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
				// ????????
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
							//Audio::GetInstance()->PlayWave(grainDiedSE);
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
			// ???[?X?X?e?[?W????
			else
			{
				//Audio::GetInstance()->PlayWave(grainDiedSE);
				grainScatterEffect->Generate(temp->GetPos(), temp->isChangeColor);
				stars.remove(temp);
				break;
			}
		}
	}
}

// ?g?Q
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
				if (stageType != RaceStage)
				{
					temp->SetPos(
						{
							temp->GetPos().x,
							ground->GetPos().y + ground->GetScale().y + 1.5f,
							temp->GetPos().z,
						});
				}
				else
				{
					temp->SetisDestroy(true);
				}
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

	for (const auto& temp : thorns)
	{
		if (temp->GetisDestroy() == true)
		{
			thorns.remove(temp);
			break;
		}
	}
}

// ?u???b?N
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

	// ??????????
	for (const auto& temp : blocks)
	{
		SquareCollider blockCollider =
		{
			{ temp->GetPos().x,temp->GetPos().y },
			{ temp->GetScale().x,temp->GetScale().y },
		};

		if (temp->revival->isRevival == false)
		{
			// ?v???C???[
			if (temp->GetisHit() == 0 && player->GetisGround() == false)
			{
				// ????????????????????
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
						if (player->GetisWeakAttack() == true &&
							player->GetisReverse2() == false)
						{
							player->SetisReverse(true);
							player->SetAttackMoveSpeed(0);
							/*player->SetPos(
													{
														temp->GetPos().x,
														temp->GetPos().y + 6.5f,
														temp->GetPos().z,
													});*/
							temp->SetisHit(1);
						}
						if (player->GetisHeavyAttack() == true)
						{
							temp->SetisHit(1);
						}
						break;
					}
				}
			}
			if (temp->GetisHit() == 1/* && player->GetisReverse2() == false*/)
			{
				if (player->GetisWeakAttack() == true)
				{
					temp->Damage(player->GetWeakAttackDamage());
					temp->SetisHit(2);
					break;
				}
				if (player->GetisHeavyAttack() == true)
				{
					temp->Damage(player->GetHeavyAttackDamage());
					temp->SetisHit(2);
					break;
				}
			}
			if (player->GetisJump() == true && player->GetisHitBlock() == false)
			{
				SquareCollider playerCollider =
				{
					{ player->GetPos().x, player->GetPos().y - player->GetRadius() + player->GetAttackMoveSpeed()},
					{ player->GetRadius(), player->GetRadius() - 0.25f},
				};

				// ????????????????????
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
		}

		// ?n??
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

	// ?X?V????
	for (const auto& temp : blocks)
	{
		temp->Update();
	}

	// ????????
	for (const auto& temp : blocks)
	{
		if (temp->GetisDestroy() == true)
		{
			if (temp->revival->isCanRevival == true)
			{
				if (temp->GetHaveStar() == true &&
					temp->revival->isRevival == false)
				{
					breakBlockEffect->Generate(temp->GetPos(), temp->GetScale());
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

// ???C
void Stage::CannonUpdate()
{
	static int shotTimer = 0;
	static int maxShotTimer = 240;
	shotTimer++;
	if (shotTimer >= maxShotTimer)
	{
		for (const auto& temp : cannons)
		{
			if (temp->GetisAddScale() == false)
			{
				temp->SetisAddScale(true);
				maxShotTimer = Random::Range(180, 240);
			}
		}
		shotTimer = 0;
	}


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

// ???[?X
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

// ???v??
void Stage::EnduranceUpdate()
{
	// ?J?E???g
	//const float fream = 30;
	//if (endlessCountDownIndex < 2 && isEndlessCountDown == true)
	//{
	//	startTextTimer++;
	//	if (startTextTimer >= startTextMaxTimer)
	//	{
	//		startTextExrate = 0;
	//		startTextAlpha = 1;
	//		startTextAngle = -180;
	//		startTextTimer = 0;
	//		endlessCountDownIndex++;
	//	}

	//	// ?g????
	//	if (startTextExrate >= 1)
	//	{
	//		startTextExrate += 0.005;
	//	}
	//	else if (startTextExrate >= 0)
	//	{
	//		startTextExrate += 1 / fream;
	//	}
	//	startTextSprites[endlessCountDownIndex]->SetSize({ 448 * startTextExrate,448 * startTextExrate });

	//	// ?p?x
	//	if (startTextAngle >= 0)
	//	{
	//		startTextAngle += 0.25;
	//	}
	//	else if (startTextAngle >= -180)
	//	{
	//		startTextAngle += 180 / fream;
	//	}
	//	startTextSprites[endlessCountDownIndex]->SetRotation(DegreeToRad(startTextAngle));

	//	// ?A???t?@
	//	startTextAlpha -= 1 / (float)startTextMaxTimer;
	//	startTextSprites[endlessCountDownIndex]->SetColor({ 1,1,1,startTextAlpha });
	//}
	//if (endlessCountDownIndex >= 3)
	//{
	//	isEndlessCountDown = false;
	//}

	//static DWORD preTimer = 0;

	//if (startTextExrate == 0)
	//{
	//	preTimer = GetNowTime() / 100;
	//}

	//static int countTime = 0;
	//static DWORD minuteFrameTime = 0;
	//static DWORD minuteFrameNowTime = 0;
	//static DWORD minuteFrameStartTime = 0;

	//static int preNumber = 0;

	// ????
	if (isGetTime == 0)
	{
		isGetTime = 1;
		enduranceStartTime = GetNowTime() / 100;
		//preNumber = enduranceTimeDightsNumber.back();
	}
	if (isGetTime == 1 && gameOver == false)
	{
		//if (preNumber != enduranceTimeDightsNumber.back())
		//{
		//	preNumber = enduranceTimeDightsNumber.back();
		//	startTextExrate = 0;
		//}

		enduranceNowTime = GetNowTime() / 100;
		enduranceEndTime = enduranceNowTime - enduranceStartTime;
		if (enduranceEndTime == enduranceTime)
		{
			isGetTime = 2;
		}

		if (enduranceEndTime <= 3)
		{
			isEndlessCountDown = true;
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


	//if (enduranceTimeDightsNumber.back() - 1 < 3 &&
	//	enduranceTimeDightsNumber.back() - 1 >= 0)
	//{

	//	// ?g????
	//	if (startTextExrate >= 1)
	//	{
	//		startTextExrate += 0.005;
	//	}
	//	else if (startTextExrate >= 0)
	//	{
	//		startTextExrate += 1 / 60;
	//		//startTextExrate += 1;
	//	}

	//	startTextSprites[enduranceTimeDightsNumber.back() - 1]->SetSize({ 448 * startTextExrate,448 * startTextExrate });

	//	//// ?p?x
	//	//if (startTextAngle >= 0)
	//	//{
	//	//	startTextAngle += 0.25;
	//	//}
	//	//else if (startTextAngle >= -180)
	//	//{
	//	//	startTextAngle += 5 / 60;
	//	//}
	//	//startTextSprites[enduranceTimeDightsNumber.back() - 1]->SetRotation(DegreeToRad(startTextAngle));

	//	//// ?A???t?@
	//	//startTextAlpha -= 1 / 60;
	//	//startTextSprites[enduranceTimeDightsNumber.back() - 1]->SetColor({ 1,1,1,startTextAlpha });
	//}
}

// ?g??
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