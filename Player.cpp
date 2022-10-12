#include "Player.h"
#include "DebugText.h"
#include "Stage.h"
#include "GameScene.h"
using namespace std;

Audio* Player::audio = nullptr;

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

	jumpSE = audio->LoadWave("se/jump.wav");
	damageSE = audio->LoadWave("se/damage.wav");

	playerTexture = TextureManager::Load("white1x1.png");
	redPixel = TextureManager::Load("red1x1.png");
	playerModel = Model::Create();
	trans = new WorldTransform();
	trans->Initialize();
}

static int tempTimer = 0; // ゲーム開始と同時に攻撃しないため
void Player::Init()
{
	tempTimer = 0;
	trans->translation_ = { 0,20,0 };
	trans->scale_ = { 1,1,1 };
	trans->UpdateMatrix();
	slowMotion = SlowMotion::GetInstance();

	isReverse = false;
	isAttack = false;
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
		viewProjection_.SetShakeValue(-2, 2, 10);
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

	// 移動処理
	trans->translation_.x += speed * slowMotion->GetSlowExrate();

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
	// 攻撃の種類を判断する処理
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
				viewProjection_.SetShakeValue(-0.5, 0.5, 5);
			}

			audio->PlayWave(jumpSE);
		}
	}

	// 攻撃処理
	if (isAttack == true)
	{
		trans->translation_.y -= attackMoveSpeed * slowMotion->GetSlowExrate();

		if (isReverse == false)
		{
			trans->translation_.y -= attackMoveSpeed * slowMotion->GetSlowExrate();
		}
		else
		{
			if (addScaleStep == 1)
			{
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
				stopTimer++;

				if (stopTimer >= 1)
				{
					trans->translation_.y += attackMoveSpeed * slowMotion->GetSlowExrate();
					trans->scale_.x += 0.1 * slowMotion->GetSlowExrate();
					trans->scale_.y += 0.1 * slowMotion->GetSlowExrate();
					trans->scale_.z += 0.1 * slowMotion->GetSlowExrate();

					if (trans->scale_.x >= 1)
					{
						isReverse = false;		// 反転フラグ
						isWeakAttack = false;	// 弱攻撃
						isHeavyAttack = false;	// 強攻撃
						isEngulfAttack = false;	// 巻き込み攻撃
						isAttack = false;		// 攻撃フラグ

						stopTimer = 0;			// 止まるタイマー
						pushKeyFream = 0;		// 押した時のフレーム
						trans->scale_ = { 1,1,1 };
					}
				}
			}
		}

		// まわりの星を壊す処理
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