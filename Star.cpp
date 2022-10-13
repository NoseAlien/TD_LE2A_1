#include "Star.h"
#include "Random.h"
#include "Stage.h"
#include "SlowMotion.h"
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
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	if (generateType == 0)
	{
		trans->translation_ += slowMotion->GetSlowExrate() * speed * dirVec.Normalized();
		speed -= 0.1 * slowMotion->GetSlowExrate();
		if (speed <= 0)
		{
			speed = 0;
		}
	}
	else if (generateType == 1 || generateType == 2)
	{
		gravity -= 0.05 * slowMotion->GetSlowExrate();
		if (gravity <= -1) gravity = -1;
		trans->translation_.x += speed * dirVec.Normalized().x * slowMotion->GetSlowExrate();
		trans->translation_.y += gravity * slowMotion->GetSlowExrate();
	}

	// UŒ‚‰Â”\‚©‚Ç‚¤‚©
	canHitTimer += 1 * slowMotion->GetSlowExrate();
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
		if (generateType == 0)
		{
			trans->scale_ = { 1.5f / (1 + speed),1.5f / (1 + speed),0.0001 };
			trans->rotation_.z += dirVec.Normalized().x * 0.05 * slowMotion->GetSlowExrate();
		}
		else if (generateType == 1 || generateType == 2)
		{
			trans->scale_ = trans->scale_ = { 1.5,1.5,0.0001 };
			trans->rotation_.z += dirVec.Normalized().x * 0.05 * slowMotion->GetSlowExrate();
		}
	}

	trans->UpdateMatrix();
}

void Star::Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture)
{
	starModel->Draw(*trans, viewProjection_, starTexture);
}
