#include "Stage.h"
#include "Collision.h"
#include "Player.h"
#include "DebugText.h"
#include "Random.h"
#include "GameScene.h"
using namespace std;

uint32_t Stage::starTexture = 0;
uint32_t Stage::thornTexture = 0;

Stage::Stage(const int& stageType) : stageType(stageType), playerIsHitGoal(false)
{
	goal = nullptr;
}
Stage::~Stage()
{

}

void Stage::Load()
{
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
	//GenerateThorn(player->GetPos());
}
void Stage::Update()
{
	if (gameClear == true || gameOver == true) return;

	if (ground->GetHP() > 0)
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
		PlayerUpdate();
	}

	if (ground->GetHP() <= 0 || playerIsHitGoal == true)
	{
		gameClear = true;
	}

	if (ground->GetPos().y + ground->GetScale().y >= 0 ||
		player->GetLife() <= 0)
	{
		gameOver = true;
	}

	viewProjection_.ShackUpdate();
	viewProjection_.UpdateMatrix();
	player->EffectUpdate();
}
void Stage::Draw()
{
	player->Draw(viewProjection_);
	if (ground->GetHP() > 0)
	{
		ground->Draw(viewProjection_);
	}

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

	while (player->GetPos().y - player->GetAttackMoveSpeed() <=
		ground->GetPos().y + ground->GetScale().y + player->GetScale().y &&
		player->GetisReverse() == false)
	{
		SquareCollider tempCollider =
		{
			{ player->GetPos().x,player->GetPos().y },
			{ player->GetScale().x,player->GetScale().y },
		};

		float y = player->GetPos().y;
		y -= 0.5;
		player->SetPos({ player->GetPos().x ,y,player->GetPos().z });

		if (collision->SquareHitSquare(tempCollider, floorCollider))
		{
			player->SetPos(
				{
					player->GetPos().x,
					ground->GetPos().y + ground->GetScale().y /*- 0.5f*/,
					player->GetPos().z
				});
			player->UpdateMatrix();
			break;
		}
	}
	if (collision->SquareHitSquare(playerCollider, floorCollider))
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
			tempGroundPos.x + player->GetSpeed(),
			tempGroundPos.y,
			tempGroundPos.z,
		});

	viewProjection_.eye.x += player->GetSpeed();
	viewProjection_.target.x += player->GetSpeed();

	if (player->GetPos().x >= goal->GetPos().x - goal->GetScale().x)
	{
		playerIsHitGoal = true;
	}

	if (goal != nullptr)
	{
		goal->Update();
	}
}

