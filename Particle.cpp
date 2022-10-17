#include "Particle.h"
#include "GameScene.h"
#include "SlowMotion.h"
using namespace std;

Model* Particle::breakGroundModel = {};
uint32_t Particle::texture = {};

Particle::Particle() :
	activeTimer(0), maxActiveTimer(120),
	vec(0, 0, 0), speed(0)
{
	trans = move(make_unique<WorldTransform>());
	trans->Initialize();
	model = Model::Create();

	sprite.reset(Sprite::Create(texture, { 0,0 }));
	sprite->SetAnchorPoint({ 0.5,0.5 });

	outLineSprite.reset(Sprite::Create(texture, { 0,0 }));
	outLineSprite->SetAnchorPoint({ 0.5,0.5 });

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

void Particle::UpdateSpirte()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();
	Vector2 tempPos =
		WolrdToScreen(trans->translation_ += vec * (speed * slowMotion->GetSlowExrate()), viewProjection_);
	sprite->SetPosition(tempPos);
	sprite->SetSize(size);
	sprite->SetColor(color);

	outLineSprite->SetPosition(tempPos);
	outLineSprite->SetSize(outLineSize);
	//outLineSprite->SetColor(outLineColor);
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
	outLineSprite->Draw();
	sprite->Draw();
}

void Particle::Load()
{
	breakGroundModel = Model::CreateFromOBJ("remnants", true);

	//texture = TextureManager::Load("SpriteTexture/particle.png");
	//texture = TextureManager::Load("SpriteTexture/particle2.png");
	texture = TextureManager::Load("SpriteTexture/particle3.png");
	//texture = TextureManager::Load("SpriteTexture/temp.png");
	//texture = TextureManager::Load("SpriteTexture/temp2.png");
}

void Particle::UnLoad()
{
	delete breakGroundModel;
}

Vector2 WolrdToScreen(const Vector3& pos, ViewProjection& viewProjection_)
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