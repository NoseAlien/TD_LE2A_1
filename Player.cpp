#include "Player.h"
#include "DebugText.h"
#include "Stage.h"
#include "GameScene.h"
#include "Ground.h"
#include "WindPressureEffect.h"
#include <string>
using namespace std;

Audio* Player::audio = nullptr;
uint32_t Player::playerTexAnime[9] = {};
uint32_t Player::heartTexture = {};
unique_ptr<Model> Player::spawnModel = nullptr;
uint32_t Player::redPixel = 0;

Player::Player() :
	isAttack(false), maxSpeed(0.25), maxPushKeyFream(60),// maxPushKeyFream(120),
	isWeakAttack(false), isHeavyAttack(false),
	collisionRadius(1), maxDamageTimer(180),
	//starAttackDamage(5), weakAttackDamage(5), heavyAttackDamage(10),
	starAttackDamage(5), weakAttackDamage(1), heavyAttackDamage(2),
	stageType(BaseStage), radius(2), bigRadius(3)
{
	weakAttackEffect = move(make_unique<WeakAttackEffect>());
	heavyAttackEffect = move(make_unique<HeavyAttackEffect>());
	playerDieEffect = move(make_unique<PlayerDieEffect>());
	playerMoveEffect = move(make_unique<PlayerMoveEffect>());
}
Player::~Player()
{
	delete playerModel;
	delete trans;
	for (int i = 0; i < 3; i++)
	{
		delete heartSprites[i];
	}
}

void Player::Load()
{
	input_ = Input::GetInstance();

	jumpSE = audio->LoadWave("se/jump.wav");
	damageSE = audio->LoadWave("se/damage.wav");

	string str;
	for (int i = 0; i < 9; i++)
	{
		str = "PlayerTexAnime/texAnime" + to_string(i + 1) + ".png";
		playerTexAnime[i] = TextureManager::Load(str.c_str());
	}
	redPixel = TextureManager::Load("red1x1.png");

	playerModel = Model::CreateFromOBJ("player", true);
	trans = new WorldTransform();
	trans->Initialize();

	collapseModel.reset(Model::CreateFromOBJ("player_collapse", true));
	collapseTrans = move(make_unique<WorldTransform>());
	collapseTrans->Initialize();

	heartTexture = TextureManager::Load("SpriteTexture/Heart.png");

	for (int i = 0; i < 3; i++)
	{
		heartSprites.push_back(Sprite::Create(heartTexture, { (float)(96 + i * 80),96 }));
		heartSprites.back()->SetAnchorPoint({ 0.5f,0.5f });
		heartSprites.back()->SetSize({ 64,64 });
	}

	spawnModel.reset(Model::CreateFromOBJ("player_spawn", true));
	spawnTrans = move(make_unique<WorldTransform>());
	spawnTrans->Initialize();
}

static int tempTimer = 0; // ƒQ[ƒ€ŠJŽn‚Æ“¯Žž‚ÉUŒ‚‚µ‚È‚¢‚½‚ß
void Player::Init()
{
	// ƒAƒjƒ[ƒVƒ‡ƒ“ŠÖ˜A
	animeIndex = 0;
	fream = 0;
	maxFream = 5;
	radius = 2;

	tempTimer = 0;

	trans->translation_ = { 0,20,0 };
	trans->scale_ = { 2,2,2 };
	trans->rotation_ = { DegreeToRad(180),0,0 };
	trans->UpdateMatrix();
	spawnTrans->translation_ =
	{
		trans->translation_.x,
		23,
		trans->translation_.z,
	};
	spawnTrans->UpdateMatrix();

	collapseTrans->translation_ = { 0,20,0 };
	collapseTrans->scale_ = { 4,4,4 };
	collapseTrans->rotation_ = { 0,0,0 };
	collapseTrans->UpdateMatrix();

	slowMotion = SlowMotion::GetInstance();

	// UŒ‚ŠÖ˜A
	isReverse = false;
	isAttack = false;
	isWeakAttack = false;
	isHeavyAttack = false;
	isEngulfAttack = false;

	//speed = 0;

	attackMoveSpeed = 3;
	haveStarNum = 0;

	isDamage = false;

	damageTimer = 0;

	// ƒ‰ƒCƒtŠÖ˜A
	life = 3;
	isAlive = true;

	isGround = false;
	isJump = false;

	// ƒuƒƒbƒN‚É“–‚½‚Á‚½ŠÖ˜A
	isHitBlock = false;
	hitedBlockStep = 0;

	playerMoveEffect->Clear();
	weakAttackEffect->Clear();
	heavyAttackEffect->Clear();
	playerDieEffect->Clear();

}
void Player::Update()
{
	if (!input_->ReleasedKey(DIK_SPACE))
	{
		tempTimer++;
	}

	MoveUpdate();
	if (tempTimer >= 10)
	{
		tempTimer = 10;
		AttackUpdate();
		DamageUpdate();
		HitBlockUpdate();

		playerMoveEffect->Generate(
			{
				trans->translation_.x,
				trans->translation_.y - radius,
				trans->translation_.z
			}, radius - 0.5);
		playerMoveEffect->Update();
	}

	trans->UpdateMatrix();
	spawnTrans->translation_ =
	{
		trans->translation_.x,
		23,
		trans->translation_.z,
	};
	spawnTrans->UpdateMatrix();
}
void Player::SelectSceneUpdate()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		trans->rotation_ = { DegreeToRad(180),0,0 };

		if (trans->translation_.y == 20)
		{
			isAttack = true;
			addScaleStep = 1;
			isWeakAttack = true;
			maxSize = 1;
			trans->scale_ = { 2,2,2 };
			//speed *= 0.5;
		}
		audio->PlayWave(jumpSE);
	}
	if (isAttack == true && isWeakAttack == true)
	{
		if (isReverse == false)
		{
			trans->translation_.y -= attackMoveSpeed * slowMotion->GetSlowExrate();
		}
		else
		{
			if (addScaleStep == 1)
			{
				stopTimer++;

				trans->translation_.y -= 1;
				trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
				trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
				trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
				if (trans->scale_.y <= 0)
				{
					trans->translation_.y = 20;
					trans->scale_ = { 0,0,0 };
					addScaleStep = 2;
				}
			}
			else if (addScaleStep == 2)
			{
				trans->scale_.x += 0.05 * slowMotion->GetSlowExrate();
				trans->scale_.y += 0.05 * slowMotion->GetSlowExrate();
				trans->scale_.z -= 0.05 * slowMotion->GetSlowExrate();
				trans->rotation_.z = DegreeToRad(Random::Range(-10, 10));

				if (trans->scale_.x >= radius)
				{
					isReverse = false;		// ”½“]ƒtƒ‰ƒO
					isWeakAttack = false;	// ŽãUŒ‚
					isHeavyAttack = false;	// ‹­UŒ‚
					isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
					isAttack = false;		// UŒ‚ƒtƒ‰ƒO

					stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
					trans->scale_ = { radius,radius,radius };
					trans->rotation_ = { DegreeToRad(180),0,0 };

				}
			}
		}
	}

	trans->UpdateMatrix();
}
void Player::Draw(const ViewProjection& viewProjection_)
{
	fream++;
	if (fream > maxFream)
	{
		animeIndex++;
		if (animeIndex > 8)
		{
			animeIndex = 0;
		}
		fream = 0;
	}

	if (damageTimer % 10 < 5)
	{
		if (isGround == true)
		{
			collapseTrans->translation_ = { trans->translation_.x,trans->translation_.y - 3,trans->translation_.z };
			collapseTrans->scale_ = trans->scale_ + 2;
			collapseTrans->UpdateMatrix();
			collapseModel->Draw(*collapseTrans, viewProjection_);
		}
		else
		{
			if (pushKeyFream >= maxPushKeyFream)
			{
				playerModel->Draw(*trans, viewProjection_, redPixel);
			}
			else
			{
				playerModel->Draw(*trans, viewProjection_, playerTexAnime[animeIndex]);
			}
		}
	}

	spawnModel->Draw(*spawnTrans, viewProjection_);
}
void Player::DrawSpriteFront()
{
	weakAttackEffect->Draw();
	heavyAttackEffect->Draw();
	playerDieEffect->Draw();

	for (int i = 0; i < life; i++)
	{
		heartSprites[i]->Draw2();
	}
}
void Player::DrawSpriteBack()
{
	playerMoveEffect->Draw();
	//if (isGround == false && isAttack == false)
	//{
	//	if (input_->PushKey(DIK_SPACE))
	//	{
	//		pushKeySprite->Draw();
	//	}
	//}
}

void Player::EffectGenerate(const Vector3& pos)
{
	if (isWeakAttack == true)
	{
		viewProjection_.SetShakeValue(1, 20);
		weakAttackEffect->Generate({ pos.x,pos.y + 2 - radius,pos.z });
	}
	if (isHeavyAttack == true)
	{
		viewProjection_.SetShakeValue(2, 40, 2);
		heavyAttackEffect->Generate({ pos.x,pos.y + 2 - radius,pos.z });
	}
}
void Player::EffectUpdate()
{
	weakAttackEffect->Update();
	heavyAttackEffect->Update();
	playerDieEffect->Update();
}

void Player::DieEffectGenerate()
{
	playerDieEffect->Generate(
		{
			trans->translation_.x,
			trans->translation_.y - radius,
			trans->translation_.z
		});
}

void Player::MoveUpdate()
{
	//if (input_->PushKey(DIK_UP)) trans->translation_.y += 0.5;
	//if (input_->PushKey(DIK_DOWN)) trans->translation_.y -= 0.5;
	//if (input_->PushKey(DIK_RIGHT)) trans->translation_.x += 0.5;
	//if (input_->PushKey(DIK_LEFT)) trans->translation_.x -= 0.5;
	//if (trans->translation_.x >= 39.5)
	//{
	//	trans->translation_.x = 39.5;
	//}
	//if (trans->translation_.x <= -39.5)
	//{
	//	trans->translation_.x = -39.5;
	//}

	//if (!isAttack)
	//{
	//	speed += 0.25 / 3;
	//}
	//speed = min(speed, maxSpeed);

	//if (isGround == false)
	//{
	//	speed = maxSpeed;
	//}
	// ˆÚ“®ˆ—
	if (isAlive == true)
	{
		trans->translation_.x += speed * slowMotion->GetSlowExrate();

		if (stageType != RaceStage)
		{
			if (trans->translation_.x >= 43)
			{
				trans->translation_.x = -43;
			}
		}
		if (trans->translation_.y >= 20)
		{
			trans->translation_.y = 20;
		}
	}
}
void Player::AttackUpdate()
{
	// UŒ‚‚ÌŽí—Þ‚ð”»’f‚·‚éˆ—
	if (isAttack == false)
	{
		if (input_->PushKey(DIK_SPACE))
		{
			if (trans->translation_.y == 20)
			{
				pushKeyFream++;
				if (trans->scale_.x >= 3)
				{
					trans->scale_ = { 3,3,3 };
					trans->rotation_.z = 0;
				}
				else
				{
					trans->scale_ += 1 / (float)maxPushKeyFream;
					trans->rotation_.z = DegreeToRad(Random::Range(-10, 10));
				}
				isReverse = false;
			}
		}

		if (input_->ReleasedKey(DIK_SPACE))
		{
			trans->rotation_ = { DegreeToRad(180),0,0 };

			if (trans->translation_.y == 20 && trans->scale_.x >= 2)
			{
				isAttack = true;
				addScaleStep = 1;
				if (pushKeyFream < maxPushKeyFream)
				{
					isWeakAttack = true;
					maxSize = 1;
					trans->scale_ = { 2,2,2 };
					//speed *= 0.5;
				}
				else
				{
					//speed = 0;
					isHeavyAttack = true;
					maxSize = 2;
					weakAttackEffect->Generate(trans->translation_ - Vector3{ 0,2 - radius,0 });
					viewProjection_.SetShakeValue(0.5, 10);
					trans->scale_ = { 3,3,3 };
				}
			}
			audio->PlayWave(jumpSE);
		}
	}

	// UŒ‚ˆ—
	if (isAttack == true)
	{
		pushKeyFream = 0;
		if (isWeakAttack == true)
		{
			if (isReverse == false)
			{
				trans->translation_.y -= attackMoveSpeed * slowMotion->GetSlowExrate();
			}
			else
			{
				if (addScaleStep == 1)
				{
					stopTimer++;

					trans->translation_.y -= 1;
					trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
					trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
					trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
					if (trans->scale_.y <= 0)
					{
						trans->translation_.y = 20;
						trans->scale_ = { 0,0,0 };
						addScaleStep = 2;
					}
				}
				if (addScaleStep == 2)
				{
					trans->scale_ += 0.05 * slowMotion->GetSlowExrate();
					trans->rotation_.z = DegreeToRad(Random::Range(-10, 10));

					if (trans->scale_.x >= radius)
					{
						isReverse = false;		// ”½“]ƒtƒ‰ƒO
						isWeakAttack = false;	// ŽãUŒ‚
						isHeavyAttack = false;	// ‹­UŒ‚
						isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
						isAttack = false;		// UŒ‚ƒtƒ‰ƒO

						stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
						trans->scale_ = { radius,radius,radius };
						trans->rotation_ = { DegreeToRad(180),0,0 };
					}
				}
			}
		}
		if (isHeavyAttack == true)
		{
			if (isReverse == false)
			{
				trans->translation_.y -= attackMoveSpeed * slowMotion->GetSlowExrate();
			}
			else
			{
				stopTimer++;
				isGround = true;

				if (addScaleStep == 1)
				{
					trans->translation_.y -= 0.05 * slowMotion->GetSlowExrate();
					trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
					trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
					trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
					if (trans->scale_.y <= 0.5)
					{
						addScaleStep = 2;
					}
				}
				if (addScaleStep == 2)
				{
					trans->scale_.x -= addScaleValue * slowMotion->GetSlowExrate();
					trans->scale_.y += addScaleValue / maxSize * slowMotion->GetSlowExrate();
					trans->scale_.z -= addScaleValue / 2 * slowMotion->GetSlowExrate();
					if (trans->scale_.y >= radius)
					{
						addScaleStep = 0;
						isReverse = false;		// ”½“]ƒtƒ‰ƒO
						isWeakAttack = false;	// ŽãUŒ‚
						isHeavyAttack = false;	// ‹­UŒ‚
						isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
						isAttack = false;		// UŒ‚ƒtƒ‰ƒO

						stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
						trans->scale_ = { radius,radius,radius };
					}
				}
			}
		}

		/*if (isAttack == false)
		{
			if (input_->PushKey(DIK_SPACE))
			{
				pushKeyFream++;
			}

			if (input_->ReleasedKey(DIK_SPACE))
			{
				isAttack = true;
				addScaleStep = 1;
				if (pushKeyFream < maxPushKeyFream)
				{
					isWeakAttack = true;
					maxSize = 2.5;
				}
				else if (pushKeyFream >= maxPushKeyFream)
				{
					isHeavyAttack = true;
					maxSize = 4;
					viewProjection_.SetShakeValue(0.5, 10);
				}

				audio->PlayWave(jumpSE);
			}
		}*/

		//if (isAttack == true)
		//{
		//	pushKeyFream = 0;
		// 
		//	if (isReverse == false)
		//	{
		//		trans->translation_.y -= attackMoveSpeed * slowMotion->GetSlowExrate();
		//	}
		//	else
		//	{
		//		if (addScaleStep == 1)
		//		{
		//			trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
		//			trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
		//			trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
		//			if (trans->scale_.y <= 0)
		//			{
		//				trans->translation_.y = 20;
		//				trans->scale_ = { 0,0,0 };
		//				addScaleStep = 2;
		//			}
		//		}
		//		else if (addScaleStep == 2)
		//		{
		//			stopTimer++;
		//
		//			if (stopTimer >= 1)
		//			{
		//				//trans->translation_.y += attackMoveSpeed * slowMotion->GetSlowExrate();
		//				trans->scale_.x += 0.05 * slowMotion->GetSlowExrate();
		//				trans->scale_.y += 0.05 * slowMotion->GetSlowExrate();
		//				trans->scale_.z += 0.05 * slowMotion->GetSlowExrate();
		//
		//				if (trans->scale_.x >= 1)
		//				{
		//					isReverse = false;		// ”½“]ƒtƒ‰ƒO
		//					isWeakAttack = false;	// ŽãUŒ‚
		//					isHeavyAttack = false;	// ‹­UŒ‚
		//					isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
		//					isAttack = false;		// UŒ‚ƒtƒ‰ƒO
		//
		//					stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
		//					trans->scale_ = { 1,1,1 };
		//				}
		//			}
		//		}
		//	}

		//// ‚Ü‚í‚è‚ÌƒIƒuƒWƒFƒNƒg‚ð‰ó‚·ˆ—
		//if (stopTimer >= 2 && isWeakAttack == true)
		//{
		//	if (input_->TriggerKey(DIK_SPACE))
		//	{
		//		isEngulfAttack = true;
		//	}
		//}
		//if (stopTimer >= 4 && isHeavyAttack == true)
		//{
		//	if (input_->TriggerKey(DIK_SPACE))
		//	{
		//		isEngulfAttack = true;
		//	}
		//}
	}

	if (isGround == true)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			if (isJumpAddScaleStep == 0)
			{
				addScaleStep = 0;
				isReverse = false;		// ”½“]ƒtƒ‰ƒO
				isWeakAttack = false;	// ŽãUŒ‚
				isHeavyAttack = false;	// ‹­UŒ‚
				isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
				isAttack = false;		// UŒ‚ƒtƒ‰ƒO

				stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
				trans->scale_ = { radius,radius,radius };

				isJumpAddScaleStep = 1;
			}
		}

		const float offset = 0.25f;
		if (isJumpAddScaleStep == 1)
		{
			trans->translation_.y -= offset * slowMotion->GetSlowExrate();

			trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
			trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
			trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
			if (trans->scale_.y <= 0.5)
			{
				Vector3 tempTrans =
				{
					trans->translation_.x,
					trans->translation_.y - 1,
					trans->translation_.z,
				};

				isJumpAddScaleStep = 2;
				WindPressureEffect::GetInstance()->Generate(tempTrans, 1);
				WindPressureEffect::GetInstance()->Generate(tempTrans, -1);
			}

		}
		if (isJumpAddScaleStep == 2)
		{
			isEngulfAttack = true;

			trans->translation_.y += offset * slowMotion->GetSlowExrate();

			trans->scale_.x -= addScaleValue * slowMotion->GetSlowExrate();
			trans->scale_.y += addScaleValue / maxSize * slowMotion->GetSlowExrate();
			trans->scale_.z -= addScaleValue / 2 * slowMotion->GetSlowExrate();

			if (trans->scale_.y >= radius)
			{
				trans->scale_ = { radius,radius,radius };
				isJumpAddScaleStep = 2;
				isJump = true;
				isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
			}

			if (isJump == true)
			{
				trans->translation_.y += attackMoveSpeed * slowMotion->GetSlowExrate();

				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					isGround = false;
					isJump = false;
					isJumpAddScaleStep = 0;
				}
			}
		}
	}
}
void Player::DamageUpdate()
{
	if (isDamage == true)
	{
		damageTimer++;
		if (damageTimer >= maxDamageTimer)
		{
			damageTimer = 0;
			isDamage = false;
		}
	}
}
void Player::HitBlockUpdate()
{
	if (isHitBlock == true)
	{
		if (hitedBlockStep == 0)
		{
			//trans->translation_.y -= 1;
			//trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
			//trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
			//trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
			//if (trans->scale_.y <= 0)
			//{
			trans->translation_.y = 20;
			trans->scale_ = { 0,0,0 };
			hitedBlockStep = 1;
			isJumpAddScaleStep = 0;
			isGround = false;
			isJump = false;
			//}
		}
		if (hitedBlockStep == 1)
		{
			trans->scale_ += 0.05 * slowMotion->GetSlowExrate();
			//trans->scale_.y += 0.05 * slowMotion->GetSlowExrate();
			//trans->scale_.z += 0.05 * slowMotion->GetSlowExrate();
			trans->rotation_.z = DegreeToRad(Random::Range(-10, 10));

			if (trans->scale_.x >= radius)
			{
				isReverse = false;		// ”½“]ƒtƒ‰ƒO
				isWeakAttack = false;	// ŽãUŒ‚
				isHeavyAttack = false;	// ‹­UŒ‚
				isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
				isAttack = false;		// UŒ‚ƒtƒ‰ƒO

				stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
				trans->scale_ = { radius,radius,radius };
				trans->rotation_ = { DegreeToRad(180),0,0 };

				hitedBlockStep = 0;
				isHitBlock = false;
			}
		}

		//if (hitedBlockStep == 0)
		//{
		//	trans->translation_.y -= 1;
		//	trans->scale_.x += addScaleValue * slowMotion->GetSlowExrate();
		//	trans->scale_.y -= addScaleValue / maxSize * slowMotion->GetSlowExrate();
		//	trans->scale_.z += addScaleValue / 2 * slowMotion->GetSlowExrate();
		//	if (trans->scale_.y <= 0)
		//	{
		//		trans->translation_.y = 20;
		//		trans->scale_ = { 0,0,0 };
		//		hitedBlockStep = 1;
		//	}
		//}
		//else if (hitedBlockStep == 1)
		//{
		//	trans->scale_.x += 0.05 * slowMotion->GetSlowExrate();
		//	trans->scale_.y += 0.05 * slowMotion->GetSlowExrate();
		//	trans->scale_.z += 0.05 * slowMotion->GetSlowExrate();
		//	trans->rotation_.z = DegreeToRad(Random::Range(-10, 10));

		//	if (trans->scale_.x >= radius)
		//	{
		//		isReverse = false;		// ”½“]ƒtƒ‰ƒO
		//		isWeakAttack = false;	// ŽãUŒ‚
		//		isHeavyAttack = false;	// ‹­UŒ‚
		//		isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
		//		isAttack = false;		// UŒ‚ƒtƒ‰ƒO

		//		stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
		//		trans->scale_ = { radius,radius,radius };
		//		trans->rotation_ = { DegreeToRad(180),0,0 };

		//		hitedBlockStep = 0;
		//		isHitBlock = false;
		//	}
		//}
	}
}

void Player::SetisDamage(const bool& isDamage)
{
	if (damageTimer == 0 && life > 0)
	{
		this->isDamage = isDamage;
		pushKeyFream = 0;
		life--;

		viewProjection_.SetShakeValue(2, 15, 2);
		audio->PlayWave(damageSE);
	}
}

Player* Player::GetInstance()
{
	static Player* player = new Player;
	return player;
}
void Player::DestroyInstance()
{
	delete GetInstance();
}
Player* player = Player::GetInstance();