#include "Star.h"

Star::Star() :
	gravity(2), collisionRadius(2)
{
	trans = new WorldTransform();
	trans->Initialize();
	starModel = Model::Create();
}

Star::~Star()
{
	delete trans;
	delete starModel;
}

void Star::Init()
{

}

void Star::Generate(const Vector3 pos, const float& floorPosY, const int& dir)
{
	trans->translation_ = pos;
	trans->scale_ = { 2,2,2 };
	trans->UpdateMatrix();
	gravity = 1;
	isNotGravity = false;
	isMove = false;
	this->floorPosY = floorPosY;
	this->dir = dir;
}
void Star::Update()
{
	if (trans->translation_.y <= floorPosY)
	{
		if (isNotGravity == true)
		{
			trans->translation_.y = floorPosY;
			gravity = 0;
			isMove = true;
		}
	}
	else
	{
		trans->translation_.x += dir * 0.25;
	}

	trans->translation_.y += gravity;
	gravity -= 0.05;
	if (gravity <= 0)
	{
		isNotGravity = true;
	}
	if (gravity <= -2)
	{
		gravity = -2;
	}

	if (isMove == true)
	{
		trans->translation_.x += 0.2;
		if (trans->translation_.x >= 50)
		{
			trans->translation_.x = -50;
		}
	}

	trans->UpdateMatrix();
}

void Star::Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture)
{
	starModel->Draw(*trans, viewProjection_, starTexture);
}
