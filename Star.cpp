#include "Star.h"
#include "Random.h"
#include "Stage.h"
using namespace MathUtility;

Star::Star() :
	gravity(2), collisionRadius(1),
	isCanHit(false), maxCanHitTimer(10), stageType(BaseStage)
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
	trans->scale_ = { 1.5,1.5,0.0001 };
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

	trans->rotation_ = { 0,0,trans->rotation_.z };

	if (isAngleShake == true)
	{
		/*angleShakeValue =
		{
			Random::RangeF(-10, 10),
			Random::RangeF(-10, 10),
			Random::RangeF(-10, 10),
		};

		trans->rotation_ = { DegreeToRad(angleShakeValue.x),
		DegreeToRad(angleShakeValue.y),
		DegreeToRad(angleShakeValue.z) };*/

		trans->rotation_.z = DegreeToRad(Random::RangeF(-20, 20));

	}
	else
	{
		trans->scale_ = { 1.5f / (1 + speed),1.5f / (1 + speed),0.0001};

		trans->rotation_.z += dirVec.Normalized().x * 0.05;
	}

	trans->UpdateMatrix();
}

void Star::Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture)
{
	starModel->Draw(*trans, viewProjection_, starTexture);
}
