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
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(360 / maxParticle * i);

		particles.emplace_back(move(make_unique<Particle>()));
		particles.back()->SetPos(pos);
		particles.back()->SetScale(
			{
				1,1,1
				//Random::RangeF(0.3,3),
				//Random::RangeF(0.3,3),
				//Random::RangeF(0.3,3),
			});
		particles.back()->SetSpeed(0.5);
		particles.back()->SetVec(Vector3(cosf(radian), sinf(radian), 0).Normalized());
	}
}

void PlayerDieEffect::Update()
{
	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.01;
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

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
	}

	if (count >= maxParticle)
	{
		isEnd = true;
	}
}

void PlayerDieEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Draw();
	}
}
