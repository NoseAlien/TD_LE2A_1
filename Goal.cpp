#include "Goal.h"
using namespace std;

unique_ptr<Model> Goal::goalModel = nullptr;

Goal::Goal()
{
	trans = new WorldTransform();
	trans->Initialize();
	//goalModel = Model::Create();
}

Goal::~Goal()
{
	delete trans;
}

void Goal::Load()
{
	goalModel.reset(Model::CreateFromOBJ("goal", true));
}

void Goal::UnLoad()
{
}

void Goal::Generate(const Vector3& pos)
{
	trans->translation_ = pos;
	trans->scale_ = { 1,1,1 };
	trans->rotation_ = { 0,0,DegreeToRad(180) };
	trans->UpdateMatrix();
}

void Goal::Update()
{
	trans->UpdateMatrix();
}

void Goal::Draw(const ViewProjection& viewProjection_)
{
	goalModel->Draw(*trans, viewProjection_);
}
