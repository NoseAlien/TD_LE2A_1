#include "HeavyAttackEffect.h"
#include "Random.h"
using namespace std;

HeavyAttackEffect::HeavyAttackEffect() :
	maxParticle(32)
{
}

HeavyAttackEffect::~HeavyAttackEffect()
{
}

void HeavyAttackEffect::Generate(const Vector3 pos)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(Random::Range(0, 360));

		particles.emplace_back(move(make_unique<Particle>()));
		particles.back()->SetPos(pos);
		particles.back()->SetScale({ 0.5,0.5,0.5 });
		particles.back()->SetSpeed(1);
		particles.back()->SetVec({ cosf(radian) / 5,Random::RangeF(1,1.1),sinf(radian) / 5 });
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
	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.01;
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

		auto tempVec = particles[i]->GetVec();
		Vector3 offset = { 0,-0.05,0 };
		particles[i]->SetVec(tempVec + offset);
	}

	//for (int i = 0; i < particles.size(); i++)
	//{
	//	auto tempScale = particles[i]->GetScale();
	//	tempScale -= 0.04;
	//	if (tempScale.x <= 0) tempScale = { 0,0,0 };
	//	particles[i]->SetScale(tempScale);
	//}

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

void HeavyAttackEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Draw();
	}
}
