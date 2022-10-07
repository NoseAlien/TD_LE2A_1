#include "Thorn.h"

Thorn::Thorn()
{

}

Thorn::~Thorn()
{
	delete trans;
	delete thornModel;
}

void Thorn::Generate(const Vector3& pos)
{
	trans = new WorldTransform();
	trans->Initialize();
	trans->translation_ = pos;
	trans->scale_ = { 0.5,0.5,0.5 };
	trans->UpdateMatrix();
	collisionRadius = trans->scale_.x;
	thornModel = Model::Create();
}

void Thorn::Update()
{
	trans->UpdateMatrix();
}

void Thorn::Draw(const ViewProjection& viewProjection_, const uint32_t& thornTexture)
{
	thornModel->Draw(*trans, viewProjection_, thornTexture);
}
