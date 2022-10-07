#include "Enemy.h"

Enemy::Enemy() :
	collisionRadius(10)
{
}

Enemy::~Enemy()
{
	delete trans;
	delete enemyModel;
}

void Enemy::Load()
{
	enemyTexture = TextureManager::Load("red1x1.png");
	enemyTexture2 = TextureManager::Load("blue1x1.png");
	enemyModel = Model::Create();
	trans = new WorldTransform();
	trans->Initialize();

}

void Enemy::Init()
{
	trans->translation_ = { 0,-22.5,0 };
	trans->scale_ = { 50,10,5 };
	trans->UpdateMatrix();

	hp = 50;
	isAddScale = false;
	recoveryTimer = 0;
	maxRecoveryTimer = 60;
	addScaleCount = 240;
	isAddScaleCountDown = false;
	addScale = 0;
	isSuctionStar = false;
}

void Enemy::Update()
{
	if (isAddScaleCountDown == 1)
	{
		addScaleCount--;
		if (addScaleCount <= 0)
		{
			addScaleCount = 0;
			preScaleY = trans->scale_.y;
			isAddScale = true;
			isSuctionStar = true;
			isAddScaleCountDown = 2;
		}
	}

	if (isAddScale == true)
	{
		const float difference = 4;
		addScale += 0.1;
		trans->scale_.y += addScale;
		if (trans->scale_.y - preScaleY >= difference)
		{
			trans->scale_.y = preScaleY + difference;
			addScale = 0;
			addScaleCount = 240;
			isAddScaleCountDown = 0;
			isAddScale = false;

		}

		recoveryTimer++;
		if (recoveryTimer >= maxRecoveryTimer)
		{
			hp++;
			if (hp >= 50)
			{
				hp = 50;
			}
			recoveryTimer = 0;
		}
	}
	else
	{
		recoveryTimer = 0;
	}

	trans->UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection_)
{
	if (isAddScale == true)
	{
		enemyModel->Draw(*trans, viewProjection_, enemyTexture);
	}
	else
	{
		enemyModel->Draw(*trans, viewProjection_, enemyTexture2);
	}
}

Enemy* Enemy::GetInstance()
{
	static Enemy* enemy = new Enemy;
	return enemy;
}

void Enemy::DestroyInstance()
{
	delete GetInstance();
}

Enemy* enemy = Enemy::GetInstance();