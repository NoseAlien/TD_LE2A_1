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

void Thorn::Generate(const Vector3& pos, const Vector3& scale)
{
	trans = new WorldTransform();
	trans->Initialize();
	trans->translation_ = pos;
	trans->scale_ = scale;
	trans->scale_ = { 0.5,0.5,0.5 };
	trans->UpdateMatrix();
	collisionRadius = 1;
}

void Thorn::Update()
{
	trans->UpdateMatrix();
}

void Thorn::Draw(const ViewProjection& viewProjection_, const uint32_t& thornTexture)
{
	thornModel->Draw(*trans, viewProjection_);
}
