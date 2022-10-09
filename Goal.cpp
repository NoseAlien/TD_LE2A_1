#include "Goal.h"

Goal::Goal()
{
	trans = new WorldTransform();
	trans->Initialize();
	goalModel = Model::Create();
}

Goal::~Goal()
{
	delete trans;
	delete goalModel;
}

void Goal::Generate(const Vector3& pos)
{
	trans->translation_ = pos;
	trans->scale_ = { 1,2,1 };
	trans->UpdateMatrix();
}

void Goal::Update()
{
	trans->UpdateMatrix();
}

void Goal::Draw(const ViewProjection& viewProjection_, const uint32_t& goalTexture)
{
	goalModel->Draw(*trans, viewProjection_, goalTexture);
}
