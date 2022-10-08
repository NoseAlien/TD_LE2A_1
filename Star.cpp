#include "Star.h"
#include "Random.h"
using namespace MathUtility;

Star::Star() :
	gravity(2), collisionRadius(1),
	isCanHit(false), maxCanHitTimer(10)
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

void Star::Generate(const Vector3& pos, const Vector3& dirVec, const int& generateType)
{
	trans->translation_ = pos;
	trans->scale_ = { 1.5,1.5,1.5 };
	trans->UpdateMatrix();
	gravity = 1;
	collisionRadius = trans->scale_.x;
	isNotGravity = false;
	isAngleShake = false;
	this->dirVec = dirVec;
	this->generateType = generateType;
}

void Star::Update()
{
	if (generateType == 0)
	{
		trans->translation_ += speed * dirVec.Normalized();
		speed -= 0.1;
		if (speed <= 0)
		{
			speed = 0;
		}
	}
	else if (generateType == 1)
	{
		gravity -= 0.05;
		if (gravity <= -1) gravity = -1;
		trans->translation_.x += speed * dirVec.Normalized().x;
		trans->translation_.y += gravity;
	}

	// UŒ‚‰Â”\‚©‚Ç‚¤‚©
	canHitTimer++;
	if (canHitTimer >= maxCanHitTimer)
	{
		canHitTimer = maxCanHitTimer;
		isCanHit = true;
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
