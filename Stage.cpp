#include "Stage.h"
#include "Collision.h"
#include "Player.h"
#include "DebugText.h"
#include "Random.h"
#include "GameScene.h"
#include "HitStop.h"
#include "PrimitiveDrawer.h"

using namespace std;

uint32_t Stage::starTexture = 0;
uint32_t Stage::thornTexture = 0;
vector<uint32_t> Stage::startTextTextures = {};

Stage::Stage(const int& stageType) :
	stageType(stageType), playerIsHitGoal(false), stagePcrogress(Start)
{
	goal = nullptr;
	//startTextSprites[0] = Sprite::Create(startTextTextures[0], { 960,540 });

	for (int i = 0; i < startTextTextures.size(); i++)
	{
		startTextSprites[i] = Sprite::Create(startTextTextures[i], { 960,540 });
		startTextSprites[i]->SetAnchorPoint({ 0.5, 0.5 });
	}


}
Stage::~Stage()
{
	for (int i = 0; i < startTextTextures.size(); i++)
	{
		delete startTextSprites[i];
	}

}

void Stage::Load()
{
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText1.png"));
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText2.png"));
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText3.png"));
	startTextTextures.emplace_back(TextureManager::Load("Text/StartText4.png"));
	starTexture = TextureManager::Load("star.png");
	thornTexture = TextureManager::Load("thorn.png");
}
void Stage::Init()
{
	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 0,0,0 };
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

	linePos1 = { -50,0,0 };
	linePos2 = { +50,0,0 };

	isCameraMoveStep = 0;
	cameraMoveVec = { 0,0,0 };
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
				SlowMotion::GetInstance()->StartSlowMotion(0.05, 180);
			}
			gameClear = true;
			endTime = GetNowTime();
			clearTime = endTime - startTime;

			if (stagePcrogress == Staging && player->GetPos().y >= 20 &&
				SlowMotion::GetInstance()->GetisSlowMotion() == false)
			{
				stagePcrogress = End;
			}
		}

		// ゲームオーバーの時
		if (ground->GetPos().y + ground->GetScale().y >= 0 ||
			player->GetLife() <= 0 || playerIsHitGoal == true)
		{
			if (stagePcrogress == Play)
			{
				stagePcrogress = Staging;
				isCameraMoveStep = 1;

			}
			gameOver = true;
			endTime = GetNowTime();
			clearTime = endTime - startTime;
		}
		if (stageType == RaceStage && isCameraMoveStep == 0)
		{
			viewProjection_.eye = { player->GetPos().x,0,-50 };
			viewProjection_.target = { player->GetPos().x ,0,0 };
		}

	}

	GameOverCameraUpdate();

	auto text = DebugText::GetInstance();
	text->SetPos(20, 60);
	text->Printf("isCameraMoveStep = %d", isCameraMoveStep);

	viewProjection_.ShackUpdate();
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

	//PrimitiveDrawer::GetInstance()->DrawLine3d(linePos1, linePos2, { 255,0,0,255 });

	player->EffectDraw();
	ground->EffectDraw();
}

void Stage::DrawLine()
{
	PrimitiveDrawer::GetInstance()->DrawLine3d(linePos1, linePos2, { 255,0,0,255 });
	//PrimitiveDrawer::GetInstance()->DrawLine3d({ -50,0,0 }, { 50,0,0 }, { 255,0,0,255 });
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
	if (isCameraMoveStep == 1)
	{
		Vector3 vec = player->GetPos() - viewProjection_.target;
		viewProjection_.target += vec.Normalized() * 2;
		if (vec.Magnitude() <= 2)
		{
			viewProjection_.target = player->GetPos();
			//isCameraMoveTarget = false;
			isCameraMoveStep = 2;
			//isCameraMovePos = true;
		}
	}
	if (isCameraMoveStep == 2)
	{
		cameraMoveVec = viewProjection_.target - viewProjection_.eye;

		if (cameraMoveVec.Magnitude() <= 15)
		{
			SlowMotion::GetInstance()->StartSlowMotion(0.01, 60);

			isCameraMoveStep = 3;
		}
	}
	if (isCameraMoveStep == 3)
	{
		if (stagePcrogress == Staging && SlowMotion::GetInstance()->GetisSlowMotion() == false)
		{
			isCameraMoveStep = 4;
			stagePcrogress = End;
		}
	}
	if (isCameraMoveStep == 2 || isCameraMoveStep == 3)
	{
		viewProjection_.eye += cameraMoveVec.Normalized() * 2 *
			SlowMotion::GetInstance()->GetSlowExrate();
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

	if (player->GetPos().y <= ground->GetPos().y + ground->GetScale().y + player->GetScale().y)
	{
		player->SetisReverse(true);
		// タイマ－にした、瞬間的なフラグが欲しかったため
		if (player->GetStopTimer() == 0)
		{
			player->EffectGenerate(
				{
					player->GetPos().x ,
					ground->GetPos().y + ground->GetScale().y,
					player->GetPos().z
				});

			if (player->GetHaveStarNum() > 0)
			{
				ground->Damage(player->GetHaveStarNum() * player->GetStarAttackDamage());
				player->SetHaveStarNum(0);
			}
			else
			{
				if (player->GetisHeavyAttack() == true)
				{
					PlayerGenerateStar(player->GetPos());
					ground->Damage(player->GetHeavyAttackDamage());
				}
				else if (player->GetisWeakAttack() == true)
				{
					ground->Damage(player->GetWeakAttackDamage());
				}
			}
		}
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
					temp->GetPos().y + temp->GetScale().y + player->GetScale().y,
					player->GetPos().z
				});
			player->SetisReverse(true);

			if (player->GetisWeakAttack() == true)
			{
				temp->Damage(player->GetWeakAttackDamage());
			}
			if (player->GetisHeavyAttack() == true)
			{
				temp->Damage(player->GetHeavyAttackDamage());
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

