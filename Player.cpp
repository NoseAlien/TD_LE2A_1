#include "Player.h"

Player::Player() :
	isAttack(false), speed(0.25),
	isWeakAttack(false), isHeavyAttack(false),
	collisionRadius(1)
{

	//trans->scale_ = { 1.5,1.5,1.5 };
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

void Player::Init()
{
	trans->translation_ = { 0,20,0 };
	trans->UpdateMatrix();

	isWeakAttack = false;
	isHeavyAttack = false;
	attackMoveSpeed = 3;
	isHaveStar = false;

	isDamage = false;

	damegeTimer = 0;
}

void Player::Update()
{
	MoveUpdate();
	AttackUpdate();
	DamageUpdate();
	trans->UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection_)
{
	if (damegeTimer % 10 < 5)
	{
		if (pushKeyFream >= 30)
		{
			playerModel->Draw(*trans, viewProjection_, redPixel);
		}
		else
		{
			playerModel->Draw(*trans, viewProjection_, playerTexture);
		}
	}
}

void Player::MoveUpdate()
{
	//if (input_->PushKey(DIK_UP)) trans->translation_.y += 0.5;
	//if (input_->PushKey(DIK_DOWN)) trans->translation_.y -= 0.5;
	//if (input_->PushKey(DIK_RIGHT)) trans->translation_.x += 0.5;
	//if (input_->PushKey(DIK_LEFT)) trans->translation_.x -= 0.5;

	// 移動処理
	trans->translation_.x += speed;
	if (trans->translation_.x >= 43)
	{
		trans->translation_.x = -43;
	}
}
void Player::AttackUpdate()
{
	// 攻撃の種類を判断する処理
	if (isAttack == false && isDamage == false)
	{
		if (input_->PushKey(DIK_SPACE))
		{
			pushKeyFream++;
		}

		if (input_->ReleasedKey(DIK_SPACE))
		{
			isAttack = true;
			if (pushKeyFream < 30)
			{
				isWeakAttack = true;
			}
			else if (pushKeyFream >= 30)
			{
				isHeavyAttack = true;
			}
		}
	}

	// 攻撃処理
	if (isAttack == true)
	{
		if (trans->translation_.y <= floorPosY)
		{
			trans->translation_.y = floorPosY;
			isReverse = true;
		}

		if (isReverse == false)
		{
			trans->translation_.y -= attackMoveSpeed;
		}
		else
		{
			stopTimer++;
			if (stopTimer >= 5)
			{
				trans->translation_.y += attackMoveSpeed;
				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					isReverse = false;		// 反転フラグ
					isWeakAttack = false;	// 弱攻撃
					isHeavyAttack = false;	// 強攻撃
					isAttack = false;		// 攻撃フラグ
					isHaveStar = false;		// 星持ちか?

					isBreak = false;		// まわりの星を壊すフラグ
					stopTimer = 0;			// 止まるタイマー
					pushKeyFream = 0;		// 押した時のフレーム
				}
			}
		}
	}

	// まわりの星を壊す処理
	if (trans->translation_.y <= -9)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			isBreak = true;
		}
	}
}
void Player::DamageUpdate()
{
	if (isDamage == true)
	{
		pushKeyFream = 0;

		damegeTimer++;
		if (damegeTimer >= 100)
		{
			damegeTimer = 0;
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