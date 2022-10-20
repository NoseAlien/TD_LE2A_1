#include "RepairEffect.h"
#include "MathUtility.h"
#include "Random.h"
#include "SlowMotion.h"
using namespace std;

RepairEffect::RepairEffect() :
	maxParticle(4)
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

		float particleScale = Random::RangeF(0.5, 0.8);

		particles.emplace_back(move(make_unique<Particle>(3)));
		particles.back()->SetPos(pos + Vector3{ Random::RangeF(-1.0f, 1.0f),Random::RangeF(-1.0f, 1.0f),Random::RangeF(-1.0f, 1.0f) }.Normalized() * 4);
		particles.back()->SetScale({ particleScale,particleScale,0 });
		particles.back()->SetSpeed(Random::RangeF(0.1, 0.2));
		particles.back()->SetSpriteColor({ 1,1,1,1 });
		particles.back()->SetVec(Vector3{ 0,1,0 });

		effect.emplace_back(move(make_unique<Particle>(4)));
		effect.back()->SetPos(pos);
		effect.back()->SetScale({ 1,1,0 });
		effect.back()->SetSpeed(0);
		effect.back()->SetSpriteColor({ 1,1,1,1 });
		effect.back()->SetVec(Vector3{ 0,0,0 });
	}
}

void RepairEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.01 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);
		particles[i]->SetSpriteSize({ 128 * tempScale.x,128 * tempScale.y });

		float diffuse = 0.95;
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


	for (int i = 0; i < effect.size(); i++)
	{
		auto tempScale = effect[i]->GetScale();
		tempScale.x -= 0.03 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		effect[i]->SetScale(tempScale);
		effect[i]->SetSpriteSize({ 128 * tempScale.x,128 * tempScale.y });
	}

	for (int i = 0; i < effect.size(); i++)
	{
		effect[i]->UpdateSprite();
	}

	for (int i = 0; i < effect.size(); i++)
	{
		if (effect[i]->GetScale().x <= 0)
		{
			effect.erase(effect.begin() + i);
			break;
		}
	}
}

void RepairEffect::Draw()
{
	for (int i = 0; i < effect.size(); i++)
	{
		effect[i]->DrawSprite();
	}
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}

void RepairEffect::Clear()
{
	particles.clear();
}
