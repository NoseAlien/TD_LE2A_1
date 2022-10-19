#include "Ground.h"
#include "GameScene.h"
using namespace std;

Audio* Ground::audio = nullptr;
Model* Ground::enemyModel = nullptr;

uint32_t Ground::idleTexture = 0;
vector<uint32_t> Ground::blinkTexture = {};
vector<uint32_t> Ground::surprisedTexture = {};

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
	//enemyTexture = TextureManager::Load("groundColor1x1.png");
	enemyDangerTexture = TextureManager::Load("red1x1.png");
	//enemyModel = Model::Create();
	enemyModel = Model::CreateFromOBJ("ground", true);
	trans = new WorldTransform();
	trans->Initialize();

	idleTexture = TextureManager::Load("SpriteTexture/ground_face/ground_face_rest.png");	// 待機状態
	faceSprite.reset(Sprite::Create(idleTexture, { 0,0 }));
	faceSprite->SetAnchorPoint({ 0.5f,0.5f });

	for (int i = 1; i <= 4; i++)
	{
		blinkTexture.push_back(
			TextureManager::Load(
				"SpriteTexture/ground_face/ground_face_blink/ground_face_blink" + to_string(i) + ".png"));	// まばたき
	}

	for (int i = 1; i <= 4; i++)
	{
		surprisedTexture.push_back(
			TextureManager::Load(
				"SpriteTexture/ground_face/ground_face_surprised/ground_face_surprised" + to_string(i) + ".png"));	// まばたき
	}
}

void Ground::Init(const int& maxhp)
{
	trans->translation_ = { 0,-22.5,0 };
	//trans->scale_ = { 46,10,5 };
	trans->scale_ = { 1,1,1 };
	trans->rotation_ = { 0,DegreeToRad(90),0 };
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

	auto tempPos = WorldToScreen(
		{
			trans->translation_.x + 35,
			trans->translation_.y + 5,
			trans->translation_.z,
		}, viewProjection_);
	faceSprite->SetPosition(tempPos);

	// まばたき関連
	timer = 0;
	maxTimer = 180;
	isBlink = false;
	blinkAnimeTimer = 0;
	blinkAnimeMaxTimer = 3;
	blinkAnimeIndex = 0;

	// 驚き関連
	isSurprised = false;
	surprisedAnimeTimer = 0;
	surprisedAnimeMaxTimer = 3;
	surprisedAnimeIndex = 0;
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
		//const float difference = (fabs(trans->translation_.y) - 9.5) / 3;
		const float difference = 1.3 / 3;
		addScale += 0.01;
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

	//static int timer = 0;
	//static int maxTimer = 180;
	//static bool isBlink = false;
	//static int blinkAnimeTimer = 0;
	//static int blinkAnimeMaxTimer = 3;
	//static int blinkAnimeIndex = 0;

	timer++;
	if (timer >= maxTimer)
	{
		isBlink = true;
		timer = maxTimer;
	}

	// スプライトの座標を求める
	auto tempPos = WorldToScreen(
		{
			trans->translation_.x + 35,
			trans->translation_.y + trans->scale_.y + 4,
			trans->translation_.z,
		}, viewProjection_);
	faceSprite->SetPosition(tempPos);

	// まばたきの時
	if (isBlink == true)
	{
		blinkAnimeTimer++;
		if (blinkAnimeTimer > blinkAnimeMaxTimer)
		{
			faceSprite->SetTextureHandle(blinkTexture[blinkAnimeIndex]);

			blinkAnimeIndex++;
			if (blinkAnimeIndex > 3)
			{
				blinkAnimeIndex = 0;
				isBlink = false;
				timer = 0;
				maxTimer = Random::Range(120, 240);
				faceSprite->SetTextureHandle(idleTexture);
			}
			blinkAnimeTimer = 0;
		}
	}

	// 驚きの時
	if (isSurprised == true)
	{
		surprisedAnimeTimer++;
		if (surprisedAnimeTimer > surprisedAnimeMaxTimer)
		{
			faceSprite->SetTextureHandle(surprisedTexture[surprisedAnimeIndex]);

			surprisedAnimeIndex++;
			if (surprisedAnimeIndex > 3)
			{
				surprisedAnimeIndex = 0;
				isSurprised = false;
				faceSprite->SetTextureHandle(idleTexture);
			}
			surprisedAnimeTimer = 0;
		}
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
		//enemyModel->Draw(*trans, viewProjection_, enemyTexture);
		enemyModel->Draw(*trans, viewProjection_);
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

void Ground::DrawSprite()
{
	faceSprite->Draw();
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
