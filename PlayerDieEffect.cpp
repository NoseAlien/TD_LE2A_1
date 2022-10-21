#include "PlayerDieEffect.h"
using namespace std;

PlayerDieEffect::PlayerDieEffect() : maxParticle(12)
{
}

PlayerDieEffect::~PlayerDieEffect()
{
}

void PlayerDieEffect::Generate(const Vector3& pos)
{
	isEnd = false;
	count = 0;
	//for (int i = 0; i < maxParticle; i++)
	//{
	//	float radian = DegreeToRad(360 / maxParticle * i);

	//	particles.emplace_back(move(make_unique<Particle>()));
	//	particles.back()->SetPos(pos);
	//	particles.back()->SetScale({ 1,1,1 });
	//	particles.back()->SetSpeed(0.5);
	//	particles.back()->SetVec(Vector3(cosf(radian), sinf(radian), 0).Normalized());
	//}

	float startRad = DegreeToRad(Random::Range(0, 360 / maxParticle));
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = startRad + DegreeToRad(360 / maxParticle * i);

		float particleScale = Random::RangeF(3, 2);

		particles.emplace_back(move(make_unique<Particle>(1)));
		particles.back()->SetPos(pos);
		particles.back()->SetSpeed(0.7);
		particles.back()->SetVec(Vector3(cosf(radian), sinf(radian), 1).Normalized());
		particles.back()->SetScale({ particleScale,particleScale,particleScale });
		particles.back()->SetSpriteColor({ 0.248,0.839,0.396,1 });
	}
}

void PlayerDieEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.04;
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
			count++;
			break;
		}
	}

	/*for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.02;
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

		particles[i]->SetSpeed(particles[i]->GetSpeed() * 0.9);

		particles[i]->Update();
	}

	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->GetScale().x <= 0)
		{
			particles.erase(particles.begin() + i);
			count++;
			break;
		}
	}*/

	if (count >= maxParticle)
	{
		isEnd = true;
	}
}

void PlayerDieEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}

void PlayerDieEffect::Clear()
{
	particles.clear();
}
