#include "WindPressureEffect.h"
using namespace std;

WindPressureEffect::WindPressureEffect()
{
}

WindPressureEffect::~WindPressureEffect()
{
}

void WindPressureEffect::Generate(const Vector3& pos, const int& dir)
{
	waves.emplace_back(move(make_unique<Particle>(5)));
	waves.back()->SetPos(pos);
	waves.back()->SetSpeed(1);
	waves.back()->SetSpriteColor({ 1,1,1,1 });
	waves.back()->SetVec(Vector3{ float(dir),0,0 }.Normalized());
}

void WindPressureEffect::Update()
{
	for (int i = 0; i < waves.size(); i++)
	{
		waves[i]->SetSpeed(waves[i]->GetSpeed() * 0.9);
		if (waves[i]->GetSpeed() <= 0.01)
		{
			auto a = waves[i]->GetSpriteColor().w;
			a -= 0.05;
			waves[i]->SetSpriteColor({ 1,1,1,a });
		}

		waves[i]->UpdateSprite();
	}

	for (int i = 0; i < waves.size(); i++)
	{
		if (waves[i]->GetSpriteColor().w <= 0)
		{
			waves.erase(waves.begin() + i);
		}
	}
}

void WindPressureEffect::Draw()
{
	for (int i = 0; i < waves.size(); i++)
	{
		waves[i]->DrawSprite();
	}
}

void WindPressureEffect::Clear()
{
	waves.clear();
}


WindPressureEffect* WindPressureEffect::GetInstance()
{
	static WindPressureEffect* windPressureEffect = new WindPressureEffect;
	return windPressureEffect;
}

void WindPressureEffect::DestroyInstance()
{
	delete GetInstance();
}