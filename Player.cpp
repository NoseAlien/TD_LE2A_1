#include "Player.h"
#include "DebugText.h"
#include "Stage.h"
#include "GameScene.h"
using namespace std;

Player::Player() :
	isAttack(false), speed(0.25), maxPushKeyFream(60),// maxPushKeyFream(120),
	isWeakAttack(false), isHeavyAttack(false),
	collisionRadius(1), maxDamageTimer(180),
	starAttackDamage(5), weakAttackDamage(5), heavyAttackDamage(10),
	stageType(BaseStage)
{
	weakAttackEffect = move(make_unique<WeakAttackEffect>());
	heavyAttackEffect = move(make_unique<HeavyAttackEffect>());
}
Player::~Player()
{
	delete playerModel;
	delete trans;
}

void Player::Load()
{
	input_ = Input::GetInstance();
	playerTexture = TextureManager::Load("white1x1.png");
	redPixel = TextureManager::Load("red1x1.png");
	playerModel = Model::Create();
	trans = new WorldTransform();
	trans->Initialize();
}

static int tempTimer = 0; // ƒQ[ƒ€ŠJŽn‚Æ“¯Žž‚ÉUŒ‚‚µ‚È‚¢‚½‚ß
void Player::Init()
{
	tempTimer = 0;
	trans->translation_ = { 0,20,0 };
	trans->UpdateMatrix();

	isWeakAttack = false;
	isHeavyAttack = false;
	isEngulfAttack = false;

	attackMoveSpeed = 3;
	haveStarNum = 0;

	isDamage = false;

	damageTimer = 0;

	life = 3;

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
	}
	trans->UpdateMatrix();
}

void Player::SelectSceneUpdate()
{
	AttackUpdate();
	trans->UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection_)
{
	if (damageTimer % 10 < 5)
	{
		if (pushKeyFream >= maxPushKeyFream)
		{
			playerModel->Draw(*trans, viewProjection_, redPixel);
		}
		else
		{
			playerModel->Draw(*trans, viewProjection_, playerTexture);
		}
	}
}

void Player::EffectGenerate(const Vector3& pos)
{
	if (isWeakAttack == true)
	{
		viewProjection_.SetShakeValue(-1, 1, 5);
		weakAttackEffect->Generate({ pos.x,pos.y - 2,pos.z });
	}
	if (isHeavyAttack == true)
	{
		viewProjection_.SetShakeValue(-2, 2, 5);
		//weakAttackEffect->Generate({ pos.x,pos.y - 2,pos.z });
		heavyAttackEffect->Generate({ pos.x,pos.y - 5,pos.z });
	}
}

void Player::EffectUpdate()
{
	weakAttackEffect->Update();
	heavyAttackEffect->Update();
}

void Player::EffectDraw()
{
	weakAttackEffect->Draw();
	heavyAttackEffect->Draw();
}

void Player::MoveUpdate()
{
	//if (input_->PushKey(DIK_UP)) trans->translation_.y += 0.5;
	//if (input_->PushKey(DIK_DOWN)) trans->translation_.y -= 0.5;
	//if (input_->PushKey(DIK_RIGHT)) trans->translation_.x += 0.5;
	//if (input_->PushKey(DIK_LEFT)) trans->translation_.x -= 0.5;

	// ˆÚ“®ˆ—
	trans->translation_.x += speed;

	if (stageType != RaceStage)
	{
		if (trans->translation_.x >= 43)
		{
			trans->translation_.x = -43;
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
			}
		}
	}

	// UŒ‚ˆ—
	if (isAttack == true)
	{
		if (isReverse == false)
		{
			trans->translation_.y -= attackMoveSpeed;
		}
		else
		{
			stopTimer++;
			if (addScaleStep == 3 && stopTimer >= 1)
			{
				trans->translation_.y += attackMoveSpeed;
				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					isReverse = false;		// ”½“]ƒtƒ‰ƒO
					isWeakAttack = false;	// ŽãUŒ‚
					isHeavyAttack = false;	// ‹­UŒ‚
					isEngulfAttack = false;	// Šª‚«ž‚ÝUŒ‚
					isAttack = false;		// UŒ‚ƒtƒ‰ƒO
					//haveStarNum = 0;

					stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
					pushKeyFream = 0;		// ‰Ÿ‚µ‚½Žž‚ÌƒtƒŒ[ƒ€
					trans->scale_ = { 1,1,1 };

				}
			}
		}

		// ‚Ü‚í‚è‚Ì¯‚ð‰ó‚·ˆ—
		if (input_->TriggerKey(DIK_SPACE))
		{
			if (isWeakAttack == true)
			{
				if (stopTimer <= 8)
				{
					isEngulfAttack = true;
				}
			}
			else if (isHeavyAttack == true)
			{
				if (stopTimer <= 4)
				{
					isEngulfAttack = true;
				}
			}
		}
	}


	if (addScaleStep == 1 && isReverse == true)
	{
		trans->scale_.x += addScaleValue;
		trans->scale_.y -= addScaleValue / maxSize;
		trans->scale_.z += addScaleValue;
		if (trans->scale_.x >= maxSize)
		{
			trans->scale_.x = maxSize;
			trans->scale_.y = 0.5;
			trans->scale_.z = maxSize;
			addScaleStep = 2;
		}
	}
	if (addScaleStep == 2)
	{
		trans->scale_.x -= addScaleValue;
		trans->scale_.y += addScaleValue / maxSize;
		trans->scale_.z -= addScaleValue;
		if (trans->scale_.x <= 1)
		{
			trans->scale_ = { 1,1,1 };
			addScaleStep = 3;
		}
	}

	//trans->scale_ = { 0.5,0.5,0.5 };
	//trans->UpdateMatrix();

	//auto text = DebugText::GetInstance();
	//text->SetPos(20, 60);
	//text->Printf("pushKeyFream = %d", pushKeyFream);

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