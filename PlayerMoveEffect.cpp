#include "PlayerMoveEffect.h"
using namespace std;

PlayerMoveEffect::PlayerMoveEffect() :
	generateTimer(0), generateMaxTimer(3)

{
}

PlayerMoveEffect::~PlayerMoveEffect()
{
	particles.clear();
}

void PlayerMoveEffect::Generate(const Vector3& pos, const float& radius)
{
	generateTimer++;
	if (generateTimer >= generateMaxTimer)
	{
		particles.emplace_back(move(make_unique<Particle>(2)));
		particles.back()->SetPos(
			{
				Random::RangeF(pos.x - radius,pos.x + radius),
				Random::RangeF(pos.y - radius,pos.y + radius),
				0,
			});
		float scale = Random::RangeF(0.1, 0.5);
		particles.back()->SetScale({ scale,scale,0 });

		switch (Random::Range(0, 1))
		{
		case 0:
			particles.back()->SetSpriteRotAngle(-5);
			break;
		case 1:
			particles.back()->SetSpriteRotAngle(5);
			break;
		}

		switch (Random::Range(1, 3))
		{
		case 1:
			particles.back()->SetSpriteColor({ 0.9,0.3,0.33,1 });
			break;
		case 2:
			particles.back()->SetSpriteColor({ 0.83,0.54,0.33,1 });
			break;
		case 3:
			particles.back()->SetSpriteColor({ 0.93,0.96,0.33,1 });
			break;
		default:
			break;
		}
		generateTimer = 0;
	}
}

void PlayerMoveEffect::Update()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();

	for (int i = 0; i < particles.size(); i++)
	{
		auto tempScale = particles[i]->GetScale();
		tempScale -= 0.005 * slowMotion->GetSlowExrate();
		if (tempScale.x <= 0) tempScale = { 0,0,0 };
		particles[i]->SetScale(tempScale);
		particles[i]->SetSpriteSize({ 96 * tempScale.x,96 * tempScale.y });
	}

	for (size_t i = 0; i < particles.size(); i++)
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

void PlayerMoveEffect::Draw()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		particles[i]->DrawSprite();
	}
}

void PlayerMoveEffect::Clear()
{
	particles.clear();
}
