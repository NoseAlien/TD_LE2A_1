#include "Player.h"

Player::Player() :
	isAttack(false),
	isWeakAttack(false), weakAttackMoveSpeed(5),
	isHeavyAttack(false), heavyAttackMoveSpeed(5)
{
	trans = new WorldTransform();
	trans->Initialize();

	playerModel = Model::Create();

	playerTexture = TextureManager::Load("white1x1.png");
	redPixel = TextureManager::Load("red1x1.png");

	trans->translation_ = { 0,20,0 };

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

static bool tempReverseFlag = false;
void Player::Update()
{
	Vector3 pos = trans->translation_;

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
				weakAttackMoveSpeed = 8;
			}
			else if (pushKeyFream >= 30)
			{
				isHeavyAttack = true;
				heavyAttackMoveSpeed = 8;
			}
		}
	}

	if (isAttack == true)
	{
		// ŽãUŒ‚
		if (isWeakAttack == true)
		{
			if (tempReverseFlag == false)
			{
				trans->translation_.y -= weakAttackMoveSpeed;
				//weakAttackMoveSpeed += 0.25;

				if (trans->translation_.y <= -10)
				{
					tempReverseFlag = true;
				}
			}
			else
			{
				trans->translation_.y += weakAttackMoveSpeed;
				//weakAttackMoveSpeed += 0.25;

				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					tempReverseFlag = false;
					isWeakAttack = false;
					pushKeyFream = 0;
					isAttack = false;
				}
			}
		}

		// ‹­UŒ‚
		if (isHeavyAttack == true)
		{
			if (tempReverseFlag == false)
			{
				trans->translation_.y -= heavyAttackMoveSpeed;
				if (trans->translation_.y <= -10)
				{
					tempReverseFlag = true;
				}
			}
			else
			{
				trans->translation_.y += heavyAttackMoveSpeed;
				//weakAttackMoveSpeed += 0.25;

				if (trans->translation_.y >= 20)
				{
					trans->translation_.y = 20;
					tempReverseFlag = false;
					isHeavyAttack = false;
					pushKeyFream = 0;
					isAttack = false;
				}
			}
		}
	}
	//player->translation_ = pos;
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
