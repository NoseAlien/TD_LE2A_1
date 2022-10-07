#include "Star.h"
#include "Random.h"
using namespace MathUtility;

Star::Star() :
	gravity(2), collisionRadius(1), floorPosY(-10.5), isCanHit(false)
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

void Star::Generate(const Vector3 pos, const int& dir)
{
	speed = 1.3;
	trans->translation_ = pos;
	trans->scale_ = { 1.5,1.5,1.5 };
	trans->UpdateMatrix();
	gravity = 1;
	collisionRadius = trans->scale_.x;
	isNotGravity = false;
	this->dir = dir;
	isAngleShake = false;
}

void Star::Update()
{

	trans->translation_.x += dir * speed;
	speed -= 0.1;
	if (speed <= 0)
	{
		speed = 0;
		isCanHit = true;
	}

	if (trans->translation_.y <= floorPosY)
	{
		trans->translation_.y = floorPosY;
	}
	else
	{
		trans->translation_.y -= 0.5;
	}

	if (isAngleShake == true)
	{
		angleShakeValue =
		{
			Random::RangeF(-10, 10),
			Random::RangeF(-10, 10),
			Random::RangeF(-10, 10),
		};

		trans->rotation_.x = DegreeToRad(angleShakeValue.x);
		trans->rotation_.y = DegreeToRad(angleShakeValue.y);
		trans->rotation_.z = DegreeToRad(angleShakeValue.z);
	}
	else
	{
		trans->rotation_ = { 0,0,0 };
	}

	trans->UpdateMatrix();
}

void Star::Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture)
{
	starModel->Draw(*trans, viewProjection_, starTexture);
}
