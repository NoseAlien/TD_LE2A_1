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

	// ˆÚ“®ˆ—
	trans->translation_.x += speed;
	if (trans->translation_.x >= 43)
	{
		trans->translation_.x = -43;
	}
}
void Player::AttackUpdate()
{
	// UŒ‚‚ÌŽí—Þ‚ð”»’f‚·‚éˆ—
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

	// UŒ‚ˆ—
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
					isReverse = false;		// ”½“]ƒtƒ‰ƒO
					isWeakAttack = false;	// ŽãUŒ‚
					isHeavyAttack = false;	// ‹­UŒ‚
					isAttack = false;		// UŒ‚ƒtƒ‰ƒO
					isHaveStar = false;		// ¯Ž‚¿‚©?

					isBreak = false;		// ‚Ü‚í‚è‚Ì¯‚ð‰ó‚·ƒtƒ‰ƒO
					stopTimer = 0;			// Ž~‚Ü‚éƒ^ƒCƒ}[
					pushKeyFream = 0;		// ‰Ÿ‚µ‚½Žž‚ÌƒtƒŒ[ƒ€
				}
			}
		}
	}

	// ‚Ü‚í‚è‚Ì¯‚ð‰ó‚·ˆ—
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