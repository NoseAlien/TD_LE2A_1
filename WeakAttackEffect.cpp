#include "WeakAttackEffect.h"
#include "MathUtility.h"
#include "Random.h"
#include "SlowMotion.h"
using namespace std;

WeakAttackEffect::WeakAttackEffect() :
	maxParticle(16)
{
}

WeakAttackEffect::~WeakAttackEffect()
{
	particles.clear();
}

void WeakAttackEffect::Generate(const Vector3& pos)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));

		particles.emplace_back(move(make_unique<Particle>()));
		particles.back()->SetPos(pos);
		particles.back()->SetSpeed(0.5);
		particles.back()->SetVec(Vector3(cosf(radian), 1.5, sinf(radian)).Normalized());
		particles.back()->SetSpriteColor({ 0.25,0.85,1,1 });
		particles.back()->SetScale({ 0.5,0.5,0.5 });
		//particles.back()->SetOutLineSpriteColor({ 0,1,1,1 });

		switch (Random::Range(1, 3))
		{
		case 1:
			particles.back()->SetSpriteColor({ 0.25,1,1,1 });
			break;
		case 2:
			particles.back()->SetSpriteColor({ 0.25,1,0.5,1 });
			break;
		case 3:
			particles.back()->SetSpriteColor({ 0.24,0.5,1,1 });
			break;
		}
	}
}

void WeakAttackEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.025 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);
		particles[i]->SetSpriteSize({ 100 * tempScale.x,100 * tempScale.y });
		particles[i]->SetOutLineSpriteSize({ 110 * tempScale.x,110 * tempScale.y });
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->UpdateSpirte();
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
		particles[i]->DrawSprite();
	}
}
