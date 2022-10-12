#include "Particle.h"
#include "GameScene.h"
#include "SlowMotion.h"
using namespace std;

Model* Particle::breakGroundModel = {};

Particle::Particle() :
	activeTimer(0), maxActiveTimer(120),
	vec(0, 0, 0), speed(0)
{
	trans = move(make_unique<WorldTransform>());
	trans->Initialize();
	model = Model::Create();
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

void Particle::Draw(const int& type)
{
	model->Draw(*trans, viewProjection_);
	if (type == 0)
	{
	}
	else
	{
		breakGroundModel->Draw(*trans, viewProjection_);
	}
}

void Particle::Load()
{
	breakGroundModel = Model::CreateFromOBJ("remnants", true);
}
void Particle::UnLoad()
{
	delete breakGroundModel;
}
