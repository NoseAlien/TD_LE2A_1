#include "Ground.h"
#include "GameScene.h"
using namespace std;

Audio* Ground::audio = nullptr;
Model* Ground::enemyModel = nullptr;

uint32_t Ground::idleTexture = 0;
vector<uint32_t> Ground::blinkTexture = {};
vector<uint32_t> Ground::surprisedTexture = {};
vector<uint32_t> Ground::groundCrackTexture = {};
vector<uint32_t> Ground::cryTexture = {};

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
	recoverySE = audio->LoadWave("se/groundRecovery.wav");

	enemyDangerTexture = TextureManager::Load("SpriteTexture/GroundCrack/groundDengerColor1x1.png");
	enemyModel = Model::CreateFromOBJ("ground", false);
	trans = new WorldTransform();
	trans->Initialize();

	idleTexture = TextureManager::Load("SpriteTexture/ground_face/ground_face_rest.png");	// 待機状態
	faceSprite.reset(Sprite::Create(idleTexture, { 0,0 }));
	faceSprite->SetAnchorPoint({ 0.5f,0.5f });
	faceSprite->SetSize({ 128,128 });

	// まばたき
	for (int i = 1; i <= 4; i++)
	{
		blinkTexture.push_back(
			TextureManager::Load(
				"SpriteTexture/ground_face/ground_face_blink/ground_face_blink" + to_string(i) + ".png"));
	}

	// 驚き
	for (int i = 1; i <= 4; i++)
	{
		surprisedTexture.push_back(
			TextureManager::Load(
				"SpriteTexture/ground_face/ground_face_surprised/ground_face_surprised" + to_string(i) + ".png"));
	}

	// 泣き
	for (int i = 1; i <= 2; i++)
	{
		cryTexture.push_back(
			TextureManager::Load(
				"SpriteTexture/ground_face/ground_face_cry/ground_face_cry" + to_string(i) + ".png"));
	}

	// ひび表現
	for (int i = 0; i < 15; i++)
	{
		groundCrackTexture.push_back(
			TextureManager::Load(
				"SpriteTexture/GroundCrack/ground_Hp_" + to_string(i) + ".png"));
	}
}

void Ground::Init(const int& maxhp)
{
	trans->translation_ = { 0,-22.5,0 };
	//trans->scale_ = { 46,10,5 };
	trans->scale_ = { 1,1,1 };
	trans->rotation_ = { 0,DegreeToRad(-90),0 };
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
			trans->translation_.x + 32,
			trans->translation_.y + 10 * trans->scale_.y,
			trans->translation_.z,
		}, viewProjection_);
	faceSprite->SetPosition({ tempPos.x,tempPos.y + 96 });

	//auto tempPos = WorldToScreen(
	//	{
	//		trans->translation_.x + 32,
	//		trans->translation_.y + 10 * trans->scale_.y,
	//		trans->translation_.z,
	//	}, viewProjection_);
	//faceSprite->SetPosition(tempPos);

	// まばたき関連
	timer = 0;
	maxTimer = 180;
	isBlink = false;
	blinkAnimeTimer = 0;
	blinkAnimeMaxTimer = 3;
	blinkAnimeIndex = 0;
	faceSprite->SetTextureHandle(idleTexture);

	// 驚き関連
	isSurprised = false;
	surprisedAnimeTimer = 0;
	surprisedAnimeMaxTimer = 3;
	surprisedAnimeIndex = 0;
	surprisedCount = 0;

	// 泣き関連
	isCry = false;
	cryAnimeTimer = 0;
	cryAnimeMaxTimer = 3;
	cryAnimeIndex = 0;
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

	//if (isAddScale == true)
	//{
	//	//const float difference = (fabs(trans->translation_.y) - 9.5) / 3;
	//	const float difference = 1.3 / 3;
	//	addScale += 0.01;
	//	trans->scale_.y += addScale;
	//	if (trans->scale_.y - preScaleY >= difference)
	//	{
	//		hp += 10;
	//		if (hp >= maxhp)
	//		{
	//			hp = maxhp;
	//		}

	//		trans->scale_.y = preScaleY + difference;
	//		addScale = 0;
	//		addScaleCount = 240;
	//		isAddScaleCountDown = 0;
	//		isAddScale = false;
	//	}
	//}

	if (hp <= 0)
	{
		hp = 0;
		breakGroundEffect->Generate(trans->translation_, { 46,10,5 });
		audio->PlayWave(defeatSE);
		isAlive = false;
	}
	if (hp >= maxhp)
	{
		hp = maxhp;
	}

	// まばたきする間隔を決めるタイマー
	timer++;
	if (timer >= maxTimer)
	{
		isBlink = true;
		timer = maxTimer;
	}

	// float tempY = 5 - trans->scale_.y;

	// スプライトの座標を求める
	auto tempPos = WorldToScreen(
		{
			trans->translation_.x + 32,
			trans->translation_.y + 10 * trans->scale_.y,
			trans->translation_.z,
		}, viewProjection_);
	faceSprite->SetPosition({ tempPos.x,tempPos.y + 96 });

	// まばたきの時
	if (isBlink == true && isCry == false)
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
		// まばたき関連初期化
		isBlink = false;
		blinkAnimeTimer = 0;
		blinkAnimeIndex = 0;
		timer = 0;

		// 泣き関連初期化
		isCry = false;
		cryAnimeTimer = 0;
		cryAnimeIndex = 0;

		surprisedAnimeTimer++;
		if (surprisedAnimeTimer > surprisedAnimeMaxTimer)
		{
			faceSprite->SetTextureHandle(surprisedTexture[surprisedAnimeIndex]);

			surprisedAnimeIndex++;
			if (surprisedAnimeIndex > 3)
			{
				surprisedCount++;
				surprisedAnimeIndex = 0;
				if (surprisedCount == 2)
				{
					isSurprised = false;
					faceSprite->SetTextureHandle(idleTexture);
					surprisedCount = 0;
				}
			}
			surprisedAnimeTimer = 0;
		}
	}

	if (hp <= maxhp * 0.35)
	{
		isCry = true;
		if (isCry == true)
		{
			cryAnimeTimer++;
			if (cryAnimeTimer >= cryAnimeMaxTimer)
			{
				faceSprite->SetTextureHandle(cryTexture[cryAnimeIndex]);

				cryAnimeIndex++;
				if (cryAnimeIndex > 1)
				{
					cryAnimeIndex = 0;
				}
				cryAnimeTimer = 0;
			}
		}
	}

	if (trans->scale_.y <= 0.5)
	{
		trans->scale_.y = 0.5;
	}

	trans->UpdateMatrix();
}
void Ground::Draw(const ViewProjection& viewProjection_)
{
	if (isAlive == false) return;

	if (isDanger == true)
	{
		if (hp <= maxhp * 0.125f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[15]);
		}
		else if (hp <= maxhp * 0.25f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[14]);
		}
		else if (hp <= maxhp * 0.375f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[13]);
		}
		else if (hp <= maxhp * 0.5f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[12]);
		}
		else if (hp <= maxhp * 0.625f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[11]);
		}
		else if (hp <= maxhp * 0.75f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[10]);
		}
		else if (hp <= maxhp * 0.875f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[9]);
		}
		else if (hp <= maxhp - 1)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[8]);
		}
		else
		{
			enemyModel->Draw(*trans, viewProjection_, enemyDangerTexture);
		}
	}
	else
	{
		if (hp <= maxhp * 0.125f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[7]);
		}
		else if (hp <= maxhp * 0.25f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[6]);
		}
		else if (hp <= maxhp * 0.375f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[5]);
		}
		else if (hp <= maxhp * 0.5f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[4]);
		}
		else if (hp <= maxhp * 0.625f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[3]);
		}
		else if (hp <= maxhp * 0.75f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[2]);
		}
		else if (hp <= maxhp * 0.875f)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[1]);
		}
		else if (hp <= maxhp - 1)
		{
			enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[0]);
		}
		else
		{
			enemyModel->Draw(*trans, viewProjection_);
		}

		//if (hp <= maxhp * 0.25)
		//{
		//	enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[2]);
		//}
		//else if (hp <= maxhp * 0.5)
		//{
		//	enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[1]);
		//}
		//else if (hp <= maxhp * 0.75)
		//{
		//	enemyModel->Draw(*trans, viewProjection_, groundCrackTexture[0]);
		//}
		//else
		//{
		//	enemyModel->Draw(*trans, viewProjection_);
		//}
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
	if (isAlive == false) return;

	faceSprite->Draw2();
}

void Ground::SetThickness(const int& num)
{
	trans->scale_.y += 1.2f / 15 * num;
	auto tempPos = WorldToScreen(
		{
			trans->translation_.x + 32,
			trans->translation_.y + 10 * trans->scale_.y,
			trans->translation_.z,
		}, viewProjection_);
	faceSprite->SetPosition({ tempPos.x,tempPos.y + 96 });
	trans->UpdateMatrix();
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
