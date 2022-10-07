#include "Stage.h"
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
using namespace std;

uint32_t Stage::starTexture = 0;
uint32_t Stage::thornTexture = 0;

Stage::Stage()
{

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
	player->Init();
	enemy->Init();
	stars.clear();
	thorns.clear();
	gameClear = false;
	gameOver = false;
	//GenerateThorn(player->GetPos());
}
void Stage::Update()
{
	if (gameClear == true || gameOver == true) return;

	PlayerUpdate();
	StarUpdate();
	FloorUpdate();
	ThornUpdate();

	if (enemy->GetScale().y >= 36.5)
	{
		gameOver = true;
	}
}
void Stage::Draw(const ViewProjection& viewProjection_)
{
	player->Draw(viewProjection_);
	if (enemy->GetHP() > 0)
	{
		enemy->Draw(viewProjection_);
	}

	for (const auto& temp : stars)
	{
		temp->Draw(viewProjection_, starTexture);
	}

	for (const auto& temp : thorns)
	{
		temp->Draw(viewProjection_, thornTexture);
	}
}

void Stage::GenerateThorn(const Vector3& pos)
{
	thorns.emplace_back(move(make_unique<Thorn>()));
	thorns.back()->Generate(pos);
}

void Stage::HitGenerateStar(const Vector3 pos)
{
	for (int i = 0; i < 2; i++)
	{
		stars.emplace_back(move(make_unique<Star>()));
		if (i == 0)
		{
			stars.back()->Generate(pos, -1);
		}
		if (i == 1)
		{
			stars.back()->Generate(pos, 1);
		}
	}
}

// Ž©‹@
void Stage::PlayerUpdate()
{
	// UŒ‚ˆ—
	if (player->GetPos().y <= player->GetFloorPosY())
	{
		player->SetisReverse(true);

		// ƒ^ƒCƒ}|‚É‚µ‚½
		// uŠÔ“I‚Èƒtƒ‰ƒO‚ª—~‚µ‚©‚Á‚½‚½‚ß
		if (player->GetStopTimer() == 1)
		{
			if (player->GetisHaveStar() == true)
			{
				enemy->Damage(5);
			}
			else
			{
				if (player->GetisHeavyAttack() == true)
				{
					HitGenerateStar(player->GetPos());
					enemy->Damage(1);
				}
			}
		}
	}

	// ¯‚ðŠª‚«ž‚Þˆ—
	for (const auto& temp : stars)
	{
		if (collision->SphereHitSphere(
			player->GetPos(), player->GetRadius(), temp->GetPos(), temp->GetRadius()))
		{
			if (temp->GetisCanHit() == true)
			{
				player->SetisHaveStar(true);
				stars.remove(temp);
				break;
			}
		}
	}

	// XVˆ—
	float offset = enemy->GetPos().y + enemy->GetScale().y;
	player->SetFloorPosY(offset + player->GetScale().y);
	player->Update();
}

// °
void Stage::FloorUpdate()
{
	// ‘å‚«‚­‚È‚éˆ—
	if (stars.size() >= 10 && enemy->GetisAddScaleCountDown() == 0)
	{
		enemy->SetisAddScaleCountDown(1);
	}
	if (enemy->GetisSuctionStar() == true)
	{
		const int num = 5;
		if (stars.size() < num)
		{
			stars.clear();
			enemy->SetisSuctionStar(false);
		}
		else
		{
			for (int i = 0; i < num; i++)
			{
				stars.pop_front();
			}
			enemy->SetisSuctionStar(false);
		}
	}

	//else
	//{
	//	enemy->SetisAddScale(false);
	//}

	enemy->Update();
}

// ¯
void Stage::StarUpdate()
{
	float offset = enemy->GetPos().y + enemy->GetScale().y;
	for (const auto& temp : stars)
	{
		temp->SetFloorPosY(offset + temp->GetScale().y);
		temp->Update();
	}

	for (const auto& temp : stars)
	{
		if (temp->GetPos().x >= 44 || temp->GetPos().x <= -44)
		{
			stars.remove(temp);
			break;
		}
	}
}

// ƒgƒQ
void Stage::ThornUpdate()
{
	for (const auto& temp : thorns)
	{
		if (collision->SphereHitSphere(
			player->GetPos(), player->GetRadius(), temp->GetPos(), temp->GetRadius()))
		{
			player->SetisDamage(true);
			break;
		}
	}

	for (const auto& temp : thorns)
	{
		temp->Update();
	}
}