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
}

void Enemy::Update()
{
	if (isAddScale == true)
	{
		trans->scale_.y += 0.01;

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