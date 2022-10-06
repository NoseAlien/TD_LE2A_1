#include "Player.h"

Player::Player() :
	isAttack(false),
	isWeakAttack(false), weakAttackMoveSpeed(5),
	isHeavyAttack(false), heavyAttackMoveSpeed(5),
	collisionRadius(1)
{
	trans = new WorldTransform();
	trans->Initialize();

	playerModel = Model::Create();

	playerTexture = TextureManager::Load("white1x1.png");
	redPixel = TextureManager::Load("red1x1.png");

	trans->translation_ = { 0,20,0 };
	//trans->scale_ = { 2,2,2 };
}

Player::~Player()
{
	delete trans;
	delete playerModel;
}

void Player::Init()
{
	input_ = Input::GetInstance();

	isWeakAttack = false;
	weakAttackMoveSpeed = 5;

	isHeavyAttack = false;
	heavyAttackMoveSpeed = 5;
}

void Player::Update()
{
	//if (input_->PushKey(DIK_UP)) trans->translation_.y += 0.5;
	//if (input_->PushKey(DIK_DOWN)) trans->translation_.y -= 0.5;
	//if (input_->PushKey(DIK_RIGHT)) trans->translation_.x += 0.5;
	//if (input_->PushKey(DIK_LEFT)) trans->translation_.x -= 0.5;

	if (isAttack == false)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			pushKeyFream = 0;
		}

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

	if (isAttack == true)
	{
		// ŽãUŒ‚
		if (isWeakAttack == true)
		{
			if (isReverse == false)
			{
				trans->translation_.y -= weakAttackMoveSpeed;
			}
			else
			{
				trans->translation_.y += weakAttackMoveSpeed;

				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					isReverse = false;
					isWeakAttack = false;
					pushKeyFream = 0;
					isAttack = false;
					isHaveStar = false;
				}
			}
		}

		// ‹­UŒ‚
		if (isHeavyAttack == true)
		{
			if (isReverse == false)
			{
				trans->translation_.y -= heavyAttackMoveSpeed;
			}
			else
			{
				trans->translation_.y += heavyAttackMoveSpeed;

				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					isReverse = false;
					isHeavyAttack = false;
					pushKeyFream = 0;
					isAttack = false;
					isHaveStar = false;

				}
			}
		}
	}
	trans->UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection_)
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
