#include "Ground.h"

using namespace std;

Audio* Ground::audio = nullptr;
Model* Ground::enemyModel = nullptr;

Ground::Ground() :
	collisionRadius(10)
{
}

Ground::~Ground()
{
	delete trans;
	delete enemyModel;
}

void Ground::Load()
{
	damageSE = audio->LoadWave("se/floor_damage.wav");
	largeDamageSE = audio->LoadWave("se/floor_damage_L.wav");
	defeatSE = audio->LoadWave("se/floor_break.wav");
	enemyTexture = TextureManager::Load("groundColor1x1.png");
	enemyDangerTexture = TextureManager::Load("red1x1.png");
	enemyModel = Model::Create();
	trans = new WorldTransform();
	trans->Initialize();
}

void Ground::Init(const int& maxhp)
{
	trans->translation_ = { 0,-22.5,0 };
	trans->scale_ = { 46,10,5 };
	trans->UpdateMatrix();

	isAlive = true;
	hp = maxhp;
	this->maxhp = maxhp;
	isAddScale = false;
	maxRecoveryTimer = 60;
	addScaleCount = 240;
	isAddScaleCountDown = false;
	addScale = 0;
	isSuctionStar = false;
	isDanger = false;

	breakGroundEffect = move(make_unique<BreakGroundEffect>());
	isGeneEffect = false;
	isHit = 0;
}

void Ground::Update()
{
	if (isAlive == false) return;

	if (isAddScaleCountDown == 0)
	{
		addScaleCount = 240;
	}

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
		const float difference = (fabs(trans->translation_.y) - 9.5) / 3;
		addScale += 0.1;
		trans->scale_.y += addScale;
		if (trans->scale_.y - preScaleY >= difference)
		{
			hp += 10;
			if (hp >= maxhp)
			{
				hp = maxhp;
			}

			trans->scale_.y = preScaleY + difference;
			addScale = 0;
			addScaleCount = 240;
			isAddScaleCountDown = 0;
			isAddScale = false;
		}
	}

	if (hp <= 0)
	{
		hp = 0;
		breakGroundEffect->Generate(trans->translation_, trans->scale_);
		audio->PlayWave(defeatSE);
		isAlive = false;
	}

	trans->UpdateMatrix();
}

void Ground::Draw(const ViewProjection& viewProjection_)
{
	if (isAlive == false) return;

	if (isDanger == true)
	{
		enemyModel->Draw(*trans, viewProjection_, enemyDangerTexture);
	}
	else
	{
		enemyModel->Draw(*trans, viewProjection_, enemyTexture);
	}
}

void Ground::EffectUpdate()
{
	breakGroundEffect->Update();
}

void Ground::EffectDraw()
{
	breakGroundEffect->Draw();
}

Ground* Ground::GetInstance()
{
	static Ground* floor = new Ground;
	return floor;
}

void Ground::DestroyInstance()
{
	delete GetInstance();
}

Ground* ground = Ground::GetInstance();
