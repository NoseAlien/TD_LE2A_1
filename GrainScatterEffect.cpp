#include "GrainScatterEffect.h"
#include "MathUtility.h"
#include "Random.h"
#include "SlowMotion.h"
using namespace std;

GrainScatterEffect::GrainScatterEffect() :
	maxParticle(7)
{
}

GrainScatterEffect::~GrainScatterEffect()
{
	particles.clear();
}

void GrainScatterEffect::Generate(const Vector3& pos, const bool& colorType)
{
	float startRad = DegreeToRad(Random::Range(0, 360 / maxParticle));
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = startRad + DegreeToRad(360 / maxParticle * i);

		float particleScale = Random::RangeF(0.6, 0.2);

		particles.emplace_back(move(make_unique<Particle>(1)));
		particles.back()->SetPos(pos);
		particles.back()->SetSpeed(0.7);
		particles.back()->SetVec(Vector3(cosf(radian), sinf(radian), 1).Normalized());
		particles.back()->SetScale({ particleScale,particleScale,particleScale });

		if (colorType == false)
		{
			particles.back()->SetSpriteColor({ 0.248f,0.839f,0.396f,1 });
		}
		else
		{
			particles.back()->SetSpriteColor({ 1,0.46f,0.46f,1 });
		}
	}
	//else
	//{
	//switch (Random::Range(1, 3))
	//{
	//case 1:
	//	break;
	//case 2:
	//	particles.back()->SetSpriteColor({ 0.82f,0.27f,0.27f,1 });
	//	break;
	//case 3:
	//	particles.back()->SetSpriteColor({ 1,0.56f,0.56f,1 });
	//	break;
	//default:
	//	break;
	//}
	//}

}

void GrainScatterEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.015;
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

		particles[i]->SetSpriteSize({ 100 * tempScale.x,100 * tempScale.y });

		particles[i]->SetSpeed(particles[i]->GetSpeed() * 0.9);

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

void GrainScatterEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}
