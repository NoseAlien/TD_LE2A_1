#include "Particle.h"
#include "GameScene.h"
#include "SlowMotion.h"
using namespace std;

Model* Particle::breakGroundModel = {};
uint32_t Particle::texture = {};
uint32_t Particle::starTexture = {};
uint32_t Particle::healTexture = {};
uint32_t Particle::repairTexture = {};

Particle::Particle() :
	activeTimer(0), maxActiveTimer(120),
	vec(0, 0, 0), speed(0)
{
	trans = move(make_unique<WorldTransform>());
	trans->Initialize();
	model = Model::Create();

	//sprite.reset(Sprite::Create(texture, { 0,0 }));
	//sprite->SetAnchorPoint({ 0.5,0.5 });

	//outLineSprite.reset(Sprite::Create(texture, { 0,0 }));
	//outLineSprite->SetAnchorPoint({ 0.5,0.5 });
}

Particle::Particle(const int& spriteType) :
	activeTimer(0), maxActiveTimer(120),
	vec(0, 0, 0), speed(0), spriteType(spriteType)
{
	trans = move(make_unique<WorldTransform>());
	trans->Initialize();
	model = Model::Create();

	switch (spriteType)
	{
	case 1:
		sprite.reset(Sprite::Create(texture, { 0,0 }));
		sprite->SetAnchorPoint({ 0.5,0.5 });
		outLineSprite.reset(Sprite::Create(texture, { 0,0 }));
		outLineSprite->SetAnchorPoint({ 0.5,0.5 });
		break;
	case 2:
		sprite.reset(Sprite::Create(starTexture, { 0,0 }));
		sprite->SetAnchorPoint({ 0.5,0.5 });
		break;
	case 3:
		sprite.reset(Sprite::Create(healTexture, { 0,0 }));
		sprite->SetAnchorPoint({ 0.5,0.5 });
		break;
	case 4:
		sprite.reset(Sprite::Create(repairTexture, { 0,0 }));
		sprite->SetAnchorPoint({ 0.5,0.5 });
		break;
	default:
		break;
	}
}

Particle::~Particle()
{
	delete model;
}

void Particle::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();
	trans->translation_ += vec * (speed * slowMotion->GetSlowExrate());
	trans->UpdateMatrix();
}

void Particle::UpdateSprite()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();
	Vector2 tempPos =
		WorldToScreen(trans->translation_ += vec * (speed * slowMotion->GetSlowExrate()), viewProjection_);

	switch (spriteType)
	{
	case 1:
		sprite->SetPosition(tempPos);
		sprite->SetSize(size);
		sprite->SetColor(color);
		outLineSprite->SetPosition(tempPos);
		outLineSprite->SetSize(outLineSize);
		break;
	case 2:
		sprite->SetPosition(tempPos);
		sprite->SetSize(size);
		sprite->SetColor(color);
		sprite->SetRotation(sprite->GetRotation() + DegreeToRad(rotAngle));
		break;
	default:
		break;
	}
}

void Particle::DrawModel(const int& type)
{
	if (type == 0)
	{
		model->Draw(*trans, viewProjection_);
	}
	else
	{
		breakGroundModel->Draw(*trans, viewProjection_);
	}
}

void Particle::DrawSprite()
{
	switch (spriteType)
	{
	case 1:
		outLineSprite->Draw();
		sprite->Draw();
		break;
	case 2:
		sprite->Draw();
		break;
	default:
		break;
	}
}

void Particle::Load()
{
	breakGroundModel = Model::CreateFromOBJ("remnants", true);

	//texture = TextureManager::Load("SpriteTexture/particle.png");
	//texture = TextureManager::Load("SpriteTexture/particle2.png");
	texture = TextureManager::Load("SpriteTexture/Particle/particle3.png");
	starTexture = TextureManager::Load("SpriteTexture/Particle/starParticle.png");
	healTexture = TextureManager::Load("SpriteTexture/Particle/heal.png");
	repairTexture = TextureManager::Load("SpriteTexture/Particle/repair.png");
}

void Particle::UnLoad()
{
	delete breakGroundModel;
}

Vector2 WorldToScreen(const Vector3& pos, ViewProjection& viewProjection_)
{
	Matrix4 matViewport =
	{
		1920 / 2,0,0,0,
		0,-1080 / 2,0,0,
		0,0,1,0,
		1920 / 2 + 0,1080 / 2 + 0,0,1
	};

	Matrix4 matViewProjectionViewport =
		viewProjection_.matView * viewProjection_.matProjection * matViewport;

	Vector3 tempPos = MathUtility::Vector3TransformCoord(pos, matViewProjectionViewport);

	return { tempPos.x,tempPos.y };
}