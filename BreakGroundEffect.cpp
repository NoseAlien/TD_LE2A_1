#include "BreakGroundEffect.h"
using namespace std;

BreakGroundEffect::BreakGroundEffect() :
	maxParticle(128)
{
}

BreakGroundEffect::~BreakGroundEffect()
{
}

void BreakGroundEffect::Generate(const Vector3& pos, const Vector3& scale)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));

		particles.emplace_back(move(make_unique<Particle>()));
		particles.back()->SetPos(
			{
				Random::RangeF(pos.x - scale.x,pos.x + scale.x),
				Random::RangeF(pos.y,pos.y + scale.y),
				pos.z
			});
		particles.back()->SetScale(
			{
				Random::RangeF(0.3,3),
				Random::RangeF(0.3,3),
				Random::RangeF(0.3,3),
			});
		particles.back()->SetRot(
			{
				DegreeToRad(Random::RangeF(0,360)),
				DegreeToRad(Random::RangeF(0,360)),
				DegreeToRad(Random::RangeF(0,360)),
			});

		particles.back()->SetSpeed(1);
		particles.back()->SetVec({ cosf(radian) / 5,Random::RangeF(2,4),sinf(radian) / 5 });
	}
}

void BreakGroundEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.01 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

		auto tempVec = particles[i]->GetVec();
		Vector3 offset = { 0,-0.5,0 };
		particles[i]->SetVec(tempVec + offset * slowMotion->GetSlowExrate());

		auto tempRot = particles[i]->GetRot();
		tempRot -= 0.01;
		particles[i]->SetRot(tempRot);
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Update();
	}

	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->GetScale().x <= 0)
		{
			particles.erase(particles.begin() + i);
			break;
		}
	}
}

void BreakGroundEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawModel(1);
	}
}
