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

uint32_t Stage::starTexture = 0;
uint32_t Stage::thornTexture = 0;
vector<uint32_t> Stage::startTextTextures = {};
vector<uint32_t> Stage::numberSheet = {};
uint32_t Stage::timeStrTexture = 0;

Stage::Stage(const int& stageType) :
	stageType(stageType), playerIsHitGoal(false), stagePcrogress(Start), clearTimeDights(6)
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
	}

	timeStrSprite = Sprite::Create(timeStrTexture, { -128,-128 });
	timeStrSprite->SetAnchorPoint({ 0.5, 0.5 });
	timeStrSprite->SetSize({ 128,64 });
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
	}

	delete timeStrSprite;
}

void Stage::Load()
{
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText1.png"));
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText2.png"));
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText3.png"));
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText4.png"));
	starTexture = TextureManager::Load("star.png");
	thornTexture = TextureManager::Load("thorn.png");
	string filepath;
	for (int i = 0; i < 10; i++)
	{
		filepath = "NumberSheet/number" + to_string(i) + ".png";
		numberSheet.emplace_back(TextureManager::Load(filepath.c_str()));
	}
	timeStrTexture = TextureManager::Load("TimeStr.png");
}
bool isPlayerDieEffectGenerate = false;

void Stage::Init()
{
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

	startTextIndex = 0;
	startTextTimer = 0;
	startTextMaxTimer = 80;
	startTextExrate = 0;
	startTextAngle = -180;
	startTextAlpha = 1;
	isStartTextEnd = false;
	stagePcrogress = Start;

	startTime = 0;
	endTime = 0;
	clearTime = 0;
	isShowClearTime = false;
	clearTimeLastDightPos = { 2500,952 };
	isMoveClearTime = false;

	linePos1 = { -50,0,0 };
	linePos2 = { +50,0,0 };

	isCameraMoveStep = false;
	stagePcrogress = Start;
	cameraMoveVec = { 0,0,0 };

	isPlayerDieEffectGenerate = false;

}

void Stage::Update()
{
	//if (gameClear == true || gameOver == true) return;
	//if (stagePcrogress == End) return;

	CountDownUpdate();

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
		if (player->GetLife() > 0)
		{
			PlayerUpdate();
		}

		// ゲームクリアの時
		if (ground->GetHP() <= 0)
		{
			if (stagePcrogress == Play)
			{
				stagePcrogress = Staging;
				SlowMotion::GetInstance()->StartSlowMotion(0.05, 480);
				endTime = GetNowTime();
				clearTime = endTime - startTime;
				isShowClearTime = true;
				isMoveClearTime = true;
			}

			if (stagePcrogress == Staging && player->GetPos().y >= 20 &&
				SlowMotion::GetInstance()->GetisSlowMotion() == false)
			{
				stagePcrogress = End;
				sceneChange->StartSceneChange();

				gameClear = true;

			}
		}

		// ゲームオーバーの時
		if (ground->GetPos().y + ground->GetScale().y >= 0 ||
			player->GetLife() <= 0 || playerIsHitGoal == true)
		{
			if (stagePcrogress == Play)
			{
				stagePcrogress = Staging;
				isCameraMoveStep = true;

			}
			gameOver = true;
			endTime = GetNowTime();
			clearTime = endTime - startTime;
		}
		if (stageType == RaceStage && !isCameraMoveStep)
		{
			viewProjection_.eyePos = { player->GetPos().x,0,-50 };
			viewProjection_.targetPos = { player->GetPos().x ,0,0 };
		}
	}

	GameOverCameraUpdate();
	ClearTimeUpdate();
	//if (isShowClearTime == true)
	//{

	//}

	//auto text = DebugText::GetInstance();
	//text->SetPos(20, 60);
	//text->Printf("isCameraMoveStep = %d", isCameraMoveStep);

	viewProjection_.ShakeUpdate();
	viewProjection_.UpdateMatrix();
	player->EffectUpdate();
	ground->EffectUpdate();
}
void Stage::Draw()
{
	player->Draw(viewProjection_);
	ground->Draw(viewProjection_);

	for (const auto& temp : stars)
	{
		temp->Draw(viewProjection_, starTexture);
	}

	for (const auto& temp : thorns)
	{
		temp->Draw(viewProjection_, thornTexture);
	}

	for (const auto& temp : blocks)
	{
		temp->Draw(viewProjection_, thornTexture);
	}

	for (const auto& temp : cannons)
	{
		temp->Draw(viewProjection_, thornTexture);
	}

	if (goal != nullptr)
	{
		goal->Draw(viewProjection_, thornTexture);
	}


	player->EffectDraw();
	ground->EffectDraw();
}
void Stage::DrawLine()
{
	PrimitiveDrawer::GetInstance()->DrawLine3d(linePos1, linePos2, { 255,0,0,255 });
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
void Stage::DrawCountDown()
{
	if (startTextIndex < 4)
	{
		startTextSprites[startTextIndex]->Draw();
	}
}

void Stage::GameOverCameraUpdate()
{
	if (isCameraMoveStep)
	{
		Vector3 vec = (player->GetPos() + Vector3{ 0, 0, -8 } - viewProjection_.eyePos);
		viewProjection_.eyePos += vec * 0.4;
		viewProjection_.targetPos = viewProjection_.eyePos + Vector3{ 0, 0, 1 };

		if (vec.Magnitude() <= 0.001 && isPlayerDieEffectGenerate == false)
		{
			//sceneChange->StartSceneChange();
			player->DieEffectGenerate();
			isPlayerDieEffectGenerate = true;
		}
	}
}

void Stage::ClearTimeUpdate()
{
	if (isShowClearTime == true)
	{
		if (isMoveClearTime == true)
		{
			clearTimeLastDightPos.x -= 30;
			if (clearTimeLastDightPos.x <= 1856)
			{
				clearTimeLastDightPos.x = 1856;
			}
		}
		dightsNumber = GetDightsNumber(clearTime);
		for (int i = 0; i < dightsNumber.size(); i++)
		{
			clearTimeSprites[i]->SetTextureHandle(numberSheet[dightsNumber[i]]);
			clearTimeSprites[i]->SetPosition(
				{
					clearTimeLastDightPos.x - (dightsNumber.size() - (float)i) * 48,
					clearTimeLastDightPos.y
				});

		}
		timeStrSprite->SetPosition(
			{
				clearTimeLastDightPos.x - dightsNumber.size() * 48 - 128,
				clearTimeLastDightPos.y

			});
	}
}
void Stage::DrawClearTime()
{
	if (isShowClearTime == true)
	{
		for (int i = 0; i < 4; i++)
		{
			clearTimeSprites[i]->Draw();
		}

		timeStrSprite->Draw();
	}
}

void Stage::GenerateThorn(const Vector3& pos, const Vector3& scale)
{
	thorns.emplace_back(move(make_unique<Thorn>()));
	thorns.back()->Generate(pos, scale);
}
void Stage::GenerateBlock(const Vector3& pos, const Vector3& scale)
{
	blocks.emplace_back(move(make_unique<Block>()));
	blocks.back()->Generate(pos, scale);
}
void Stage::GenerateCannon(const Vector3& pos, const Vector3& rot)
{
	cannons.emplace_back(move(make_unique<Cannon>()));
	cannons.back()->Generate(pos, rot);
}
void Stage::GenerateGoal(const Vector3& pos)
{
	goal = move(make_unique<Goal>());
	goal->Generate(pos);
}

void Stage::PlayerGenerateStar(const Vector3 pos)
{
	for (int i = 0; i < 2; i++)
	{
		stars.emplace_back(move(make_unique<Star>()));
		if (i == 0)
		{
			stars.back()->Generate(pos, { -1,0,0 }, 0);
			stars.back()->SetSpeed(1.3);
		}
		if (i == 1)
		{
			stars.back()->Generate(pos, { 1,0,0 }, 0);
			stars.back()->SetSpeed(1.3);
		}
	}
}

// 自機
void Stage::PlayerUpdate()
{
	// まわりを巻き込む処理
	if (player->GetisEngulfAttack() == true)
	{
		SquareCollider playerCollider =
		{
			{ player->GetPos().x,player->GetPos().y },
			{ 15,4 },
		};

		for (const auto& temp : stars)
		{
			SquareCollider starCollider
			{
				{ temp->GetPos().x,temp->GetPos().y },
				{ temp->GetScale().x,temp->GetScale().y },
			};

			if (collision->SquareHitSquare(playerCollider, starCollider))
			{
				ground->Damage(player->GetStarAttackDamage());
				stars.remove(temp);
				break;
			}
		}
		for (const auto& temp : blocks)
		{
			SquareCollider blockCollider =
			{
				{ temp->GetPos().x,temp->GetPos().y },
				{ temp->GetScale().x,temp->GetScale().y },
			};

			if (collision->SquareHitSquare(playerCollider, blockCollider))
			{
				blocks.remove(temp);
				break;
			}

		}
	}

	// 星を巻き込む処理
	for (const auto& temp : stars)
	{
		if (collision->SphereHitSphere(
			player->GetPos(), player->GetRadius(), temp->GetPos(), temp->GetRadius()))
		{
			if (temp->GetisCanHit() == true)
			{
				player->HaveStarNumIncriment();
				stars.remove(temp);
				break;
			}
		}
	}

	// 更新処理
	player->Update();

	//auto text = DebugText::GetInstance();

}

// 床
void Stage::FloorUpdate()
{
	// 床との当たり判定
	//SquareCollider playerCollider =
	//{
	//	{ player->GetPos().x,player->GetPos().y },
	//	{ player->GetScale().x,player->GetScale().y },
	//};
	//SquareCollider floorCollider =
	//{
	//	{ ground->GetPos().x,ground->GetPos().y },
	//	{ ground->GetScale().x,ground->GetScale().y },
	//};

	if (player->GetPos().y <= ground->GetPos().y + ground->GetScale().y + 1)
	{
		player->SetPos(
			{
				player->GetPos().x,
				ground->GetPos().y + ground->GetScale().y + 1,
				player->GetPos().z
			});
		//player->SetScale({ 0,0,0 });

		player->SetisReverse(true);

		if (ground->GetisHit() == 0)
		{
			ground->SetisHit(1);
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
				ground->LargeDamage(player->GetHaveStarNum() * player->GetStarAttackDamage());
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
	}

	if (player->GetPos().y >= 20)
	{
		ground->SetisHit(0);
	}
	// 大きくなる処理
	if (stars.size() >= 10 && ground->GetisAddScaleCountDown() == 0)
	{
		ground->SetisAddScaleCountDown(1);
	}
	if (ground->GetisAddScaleCountDown() == 1)
	{
		for (const auto& temp : stars)
		{
			temp->SetisAngleShacke(true);
		}
	}
	if (stars.size() < 10)
	{
		for (const auto& temp : stars)
		{
			temp->SetisAngleShacke(false);
		}
	}

	// 星を吸収する処理
	if (ground->GetisSuctionStar() == true)
	{
		stars.clear();
		ground->SetisSuctionStar(false);

		//const int num = 5;
		//if (stars.size() < num)
		//{
		//	stars.clear();
		//	ground->SetisSuctionStar(false);
		//}
		//else
		//{
		//	for (int i = 0; i < num; i++)
		//	{
		//		stars.pop_front();
		//	}
		//	ground->SetisSuctionStar(false);
		//}
	}

	// 八個集まったか
	if (stars.size() >= 8)
	{
		ground->SetisDanger(true);
	}
	else
	{
		ground->SetisDanger(false);
	}

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
		SquareCollider starCollider =
		{
			{ tempStar->GetPos().x,tempStar->GetPos().y },
			{ tempStar->GetScale().x,tempStar->GetScale().y },
		};
		if (ground->GetHP() > 0)
		{
			if (collision->SquareHitSquare(starCollider, floorCollider))
			{
				tempStar->SetPos(
					{
						tempStar->GetPos().x,
						ground->GetPos().y + ground->GetScale().y + tempStar->GetScale().y,
						tempStar->GetPos().z,
					});
				tempStar->SetGravity(0);
				if (tempStar->GetGenerateType() == 1)
				{
					tempStar->SetSpeed(0);
				}
			}
		}
		else
		{

		}

		for (const auto& tempBlock : blocks)
		{
			SquareCollider blockCollider =
			{
				{ tempBlock->GetPos().x,tempBlock->GetPos().y },
				{ tempBlock->GetScale().x,tempBlock->GetScale().y },
			};

			if (collision->SquareHitSquare(starCollider, blockCollider))
			{
				tempStar->SetSpeed(0);
			}
		}
	}

	for (const auto& temp : stars)
	{
		temp->Update();
	}

	for (const auto& temp : stars)
	{
		if (stageType != RaceStage)
		{
			if (temp->GetPos().x >= 44 || temp->GetPos().x <= -44)
			{
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

	for (const auto& temp : thorns)
	{
		SquareCollider thornCollider =
		{
			{ temp->GetPos().x,temp->GetPos().y },
			{ temp->GetScale().x,temp->GetScale().y },
		};

		if (collision->SquareHitSquare(playerCollider, thornCollider))
		{
			player->SetisDamage(true);
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

	// 当たり判定
	for (const auto& temp : blocks)
	{
		SquareCollider blockCollider =
		{
			{ temp->GetPos().x,temp->GetPos().y },
			{ temp->GetScale().x,temp->GetScale().y },
		};

		if (collision->SquareHitSquare(playerCollider, blockCollider))
		{
			player->SetPos(
				{
					player->GetPos().x,
					temp->GetPos().y + temp->GetScale().y + 1,
					player->GetPos().z
				});
			player->SetisReverse(true);

			if (temp->GetisHit() == 0)
			{
				temp->SetisHit(1);
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
		}
		else if (collision->SquareHitSquare(floorCollider, blockCollider))
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

	}
	//if (player->GetPos().y <= 4)
	//{
	//	int a = 10;
	//}

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
			blocks.remove(temp);
			break;
		}
	}
}

// 大砲
void Stage::CannonUpdate()
{
	for (const auto& temp : cannons)
	{
		if (temp->GetisShot() == 1)
		{
			stars.emplace_back(move(make_unique<Star>()));
			stars.back()->Generate(temp->GetPos(), temp->GetDirVec(), 1);
			stars.back()->SetSpeed(Random::RangeF(0.5, 2.2));
			temp->SetisShot(false);
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
	Vector3 tempGroundPos = ground->GetPos();
	ground->SetPos(
		{
			tempGroundPos.x + player->GetSpeed() * SlowMotion::GetInstance()->GetSlowExrate(),
			tempGroundPos.y,
			tempGroundPos.z,
		});

	linePos1.x += player->GetSpeed() * SlowMotion::GetInstance()->GetSlowExrate();
	linePos2.x += player->GetSpeed() * SlowMotion::GetInstance()->GetSlowExrate();

	if (player->GetPos().x >= goal->GetPos().x - goal->GetScale().x)
	{
		playerIsHitGoal = true;
	}

	if (goal != nullptr)
	{
		goal->Update();
	}
}

