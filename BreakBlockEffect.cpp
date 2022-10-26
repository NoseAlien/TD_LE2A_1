#include "BreakBlockEffect.h"
using namespace std;

BreakBlockEffect::BreakBlockEffect() :
	maxParticle(16)
{
}

BreakBlockEffect::~BreakBlockEffect()
{
}

void BreakBlockEffect::Generate(const Vector3& pos, const Vector3& scale)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));

		particles.emplace_back(move(make_unique<Particle>(0)));
		particles.back()->SetPos(
			{
				Random::RangeF(pos.x - scale.x,pos.x + scale.x),
				Random::RangeF(pos.y,pos.y + scale.y),
				pos.z
			});
		particles.back()->SetScale(
			{
				Random::RangeF(0.2,3),
				Random::RangeF(0.2,3),
				Random::RangeF(0.2,3),
			});
		particles.back()->SetRot(
			{
				DegreeToRad(Random::RangeF(0,360)),
				DegreeToRad(Random::RangeF(0,360)),
				DegreeToRad(Random::RangeF(0,360)),
			});

		particles.back()->SetSpeed(1);
		particles.back()->SetVec(Vector3{ Random::RangeF(-1,1),Random::RangeF(-1,1),Random::RangeF(-1,1) }.Normalized());
	}
}

void BreakBlockEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.03 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

		auto tempVec = particles[i]->GetVec();
		Vector3 offset = { 0.9,0.9,0.9 };
		particles[i]->SetVec(tempVec * offset * slowMotion->GetSlowExrate());

		auto tempRot = particles[i]->GetRot();
		tempRot -= 0.04;
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

void BreakBlockEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawModel(1);
	}
}
