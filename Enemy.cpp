#include "Enemy.h"

Enemy::Enemy() :
	collisionRadius(10), hp(20)
{
	trans = new WorldTransform();
	trans->Initialize();

	enemyModel = Model::Create();
	enemyTexture = TextureManager::Load("red1x1.png");

	trans->translation_ = { 0,-22.5,0 };
	trans->scale_ = { 10,10,10 };
	trans->UpdateMatrix();
}

Enemy::~Enemy()
{
	delete trans;
	delete enemyModel;
}

void Enemy::Init()
{
}

void Enemy::Update()
{
}

void Enemy::Draw(const ViewProjection& viewProjection_)
{
	enemyModel->Draw(*trans, viewProjection_, enemyTexture);
}
