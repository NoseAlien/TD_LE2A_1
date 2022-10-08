#include "Player.h"
#include "DebugText.h"

Player::Player() :
	isAttack(false), speed(0.25), maxPushKeyFream(60),// maxPushKeyFream(120),
	isWeakAttack(false), isHeavyAttack(false),
	collisionRadius(1), maxDamageTimer(180),
	starAttackDamage(5), weakAttackDamage(5), heavyAttackDamage(10)
{
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
	isEngulfAttack = false;

	attackMoveSpeed = 3;
	haveStarNum = 0;

	isDamage = false;

	damageTimer = 0;

	life = 3;
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
	if (isAttack == false)
	{
		if (input_->PushKey(DIK_SPACE))
		{
			pushKeyFream++;
		}

		if (input_->ReleasedKey(DIK_SPACE))
		{
			isAttack = true;
			if (pushKeyFream < maxPushKeyFream)
			{
				isWeakAttack = true;
			}
			else if (pushKeyFream >= maxPushKeyFream)
			{
				isHeavyAttack = true;
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
			if (stopTimer >= 1)
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
				}
			}
		}
	}

	// ‚Ü‚í‚è‚Ì¯‚ð‰ó‚·ˆ—
	if (trans->translation_.y <= -9)
	{
		if (input_->TriggerKey(DIK_SPACE))
		{
			isEngulfAttack = true;
		}
	}

	auto text = DebugText::GetInstance();
	text->SetPos(20, 60);
	text->Printf("pushKeyFream = %d", pushKeyFream);

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