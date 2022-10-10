#include "WeakAttackEffect.h"
#include "MathUtility.h"
#include "Random.h"
using namespace std;

WeakAttackEffect::WeakAttackEffect() :
	maxParticle(16)
{
}

WeakAttackEffect::~WeakAttackEffect()
{
}

void WeakAttackEffect::Generate(const Vector3 pos)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));

		particles.emplace_back(move(make_unique<Particle>()));
		particles.back()->SetPos(pos);
		particles.back()->SetScale({ 0.25,0.25,0.25 });
		particles.back()->SetSpeed(0.5);
		particles.back()->SetVec(Vector3(cosf(radian), 1.5, sinf(radian)).Normalized());
	}
}

void WeakAttackEffect::Update()
{
	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.01;
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);
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

void WeakAttackEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Draw();
	}
}
