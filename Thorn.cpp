#include "Thorn.h"
using namespace std;

Model* Thorn::thornModel = nullptr;

Thorn::Thorn()
{

}

Thorn::~Thorn()
{
	delete trans;
}

void Thorn::Load()
{
	thornModel = Model::CreateFromOBJ("thorn", true);
}

void Thorn::UnLoad()
{
	delete thornModel;
}

void Thorn::Generate(const Vector3& pos, const Vector3& scale, const Vector3& rot)
{
	trans = new WorldTransform();
	trans->Initialize();
	trans->translation_ = pos;
	trans->scale_ = scale;
	trans->rotation_ = rot;
	trans->UpdateMatrix();
	collisionRadius = 0.5;
	isDestroy = false;
}

void Thorn::Update()
{
	trans->UpdateMatrix();
}

void Thorn::Draw(const ViewProjection& viewProjection_)
{
	thornModel->Draw(*trans, viewProjection_);
}
