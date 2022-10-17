#include "GrainScatterEffect.h"
#include "MathUtility.h"
#include "Random.h"
#include "SlowMotion.h"
using namespace std;

GrainScatterEffect::GrainScatterEffect() :
	maxParticle(16)
{
}

GrainScatterEffect::~GrainScatterEffect()
{
	particles.clear();
}

void GrainScatterEffect::Generate(const Vector3& pos)
{
	for (int i = 0; i < maxParticle; i++)
	{
		float radian = DegreeToRad(360 / maxParticle * i);

		particles.emplace_back(move(make_unique<Particle>(1)));
		particles.back()->SetPos(pos);
		particles.back()->SetSpeed(0.5);
		particles.back()->SetVec(Vector3(cosf(radian), sinf(radian), 1).Normalized());
		particles.back()->SetScale({ 0.5,0.5,0.5 });
		particles.back()->SetSpriteColor({ 0.248,0.839,0.396,1 });
	}
}

void GrainScatterEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.02;
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);

		particles[i]->SetSpriteSize({ 100 * tempScale.x,100 * tempScale.y });

		particles[i]->SetSpeed(particles[i]->GetSpeed() * 0.95);

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



	//for (int i = 0; i < particles.size(); i++)
	//{
	//	auto tempScale = particles[i]->GetScale();
	//	tempScale -= 0.025 * slowMotion->GetSlowExrate();
	//	if (tempScale.x <= 0) tempScale = { 0,0,0 };
	//	particles[i]->SetScale(tempScale);
	//	particles[i]->SetSpriteSize({ 100 * tempScale.x,100 * tempScale.y });
	//	particles[i]->SetOutLineSpriteSize({ 110 * tempScale.x,110 * tempScale.y });
	//}

	//for (int i = 0; i < particles.size(); i++)
	//{
	//	particles[i]->UpdateSprite();
	//}

	//for (int i = 0; i < particles.size(); i++)
	//{
	//	if (particles[i]->GetScale().x <= 0)
	//	{
	//		particles.erase(particles.begin() + i);
	//		break;
	//	}
	//}
}

void GrainScatterEffect::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}
