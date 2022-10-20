#include "RepairEffect.h"
#include "MathUtility.h"
#include "Random.h"
#include "SlowMotion.h"
using namespace std;

RepairEffect::RepairEffect() :
	maxParticle(16)
{
}

RepairEffect::~RepairEffect()
{
	particles.clear();
}

void RepairEffect::Generate(const Vector3& pos)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));

		//float particleScale = Random::RangeF(0.1, 0.3);
		float particleScale = 1;

		particles.emplace_back(move(make_unique<Particle>(1)));
		particles.back()->SetPos(pos);
		particles.back()->SetScale({ particleScale,particleScale,0 });
		particles.back()->SetSpeed(Random::RangeF(0.8, 1.3));
		particles.back()->SetSpriteColor({ 0,0,0,255 });
		//particles.back()->SetVec(Vector3{ Random::RangeF(-1.0f, 1.0f),Random::RangeF(-1.0f, 1.0f),Random::RangeF(-1.0f, 1.0f) }.Normalized());
	}
}

void RepairEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.013 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);
		particles[i]->SetSpriteSize({ 128 * tempScale.x,128 * tempScale.y });

		float diffuse = 0.85;
		particles[i]->SetVec(particles[i]->GetVec() * diffuse * slowMotion->GetSlowExrate());
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->UpdateSprite();
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

void RepairEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}

void RepairEffect::Clear()
{
	particles.clear();
}
