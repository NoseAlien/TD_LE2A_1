#include "HeavyAttackEffect.h"
#include "Random.h"
using namespace std;

HeavyAttackEffect::HeavyAttackEffect() :
	maxParticle(20)
{
}

HeavyAttackEffect::~HeavyAttackEffect()
{
}

void HeavyAttackEffect::Generate(const Vector3& pos)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));
		float particleScale = Random::RangeF(0.5, 0.1);

		particles.emplace_back(move(make_unique<Particle>(1)));
		particles.back()->SetPos(pos);
		particles.back()->SetScale({ particleScale,particleScale,particleScale });
		particles.back()->SetSpeed(Random::RangeF(2.5, 1.4));
		particles.back()->SetVec(Vector3{ Random::RangeF(-1.0f, 1.0f),Random::RangeF(-1.0f, 1.0f),Random::RangeF(-1.0f, 1.0f) }.Normalized());

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

	//for (int i = 0; i < 16; i++)
	//{
	//	float radian = DegreeToRad(Random::Range(0, 360));

	//	particles.emplace_back(move(make_unique<Particle>()));
	//	particles.back()->SetPos(pos);
	//	particles.back()->SetScale({ 1,1,1 });
	//	particles.back()->SetSpeed(0.75);
	//	particles.back()->SetVec(Vector3(cosf(radian), 2, sinf(radian)).Normalized());
	//}
}

void HeavyAttackEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.013 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);
		particles[i]->SetSpriteSize({ 100 * tempScale.x,100 * tempScale.y });

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

void HeavyAttackEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}

void HeavyAttackEffect::Clear()
{
	particles.clear();
}
