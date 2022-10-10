#include "Particle.h"
#include "GameScene.h"
using namespace std;

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
	trans->translation_ += vec * speed;
	trans->UpdateMatrix();
}

void Particle::Draw()
{
	model->Draw(*trans, viewProjection_);
}
