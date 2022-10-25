#include "DamageEffect.h"
#include "Particle.h"
#include "GameScene.h"
using namespace std;

void DamageEffect::Generate(const Vector3& pos, std::vector<int> dightsNumber)
{
	p.push_back(move(make_unique<DamageParticle>()));
	p.back()->Generate(pos, dightsNumber);
}
void DamageEffect::Update()
{
	for (int i = 0; i < p.size(); i++)
	{
		p[i]->Update();
	}

	for (int i = 0; i < p.size(); i++)
	{
		if (p[i]->isDestroy == true)
		{
			p.erase(p.begin() + i);
		}
	}
}
void DamageEffect::Draw()
{
	for (int i = 0; i < p.size(); i++)
	{
		p[i]->Draw();
	}
}
void DamageEffect::Clear()
{
	p.clear();
}

DamageEffect* DamageEffect::GetInstance()
{
	static DamageEffect* damageEffect = new DamageEffect;
	return damageEffect;
}
void DamageEffect::DestroyInstance()
{
	delete GetInstance();
}

void DamageParticle::Generate(const Vector3& pos, std::vector<int> dightsNumber)
{
	auto tempPos = WorldToScreen(pos, viewProjection_);
	dightsSize = dightsNumber.size();
	isDestroy = false;
	alpha = 1;

	if (dightsSize == 1)
	{
		damegeSprite[0].reset(Sprite::Create(
			Stage::numberSheet[dightsNumber[0]], { 128 + tempPos.x,tempPos.y - 64 }));
		damegeSprite[0]->SetSize({ 0,0 });
		damegeSprite[0]->SetAnchorPoint({ 0.5f,0.5f });
	}
	if (dightsSize == 2)
	{
		for (int i = 0; i < 2; i++)
		{
			damegeSprite[i].reset(Sprite::Create(
				Stage::numberSheet[dightsNumber[i]], { 128 + tempPos.x - 24 + i * 48,tempPos.y - 64 }));
			damegeSprite[i]->SetSize({ 0,0 });
			damegeSprite[i]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}

	ease.SetPowNum(5);
	ease.SetEaseTimer(30);
}
void DamageParticle::Update()
{
	for (int i = 0; i < dightsSize; i++)
	{
		if (i == 0)
		{
			ease.Update();
		}
		damegeSprite[i]->SetSize(ease.Out({ 0,0 }, { 70,70 }));
		if (damegeSprite[i]->GetSize().x >= 64)
		{
			if (i == 0)
			{
				alpha -= 0.05;
			}
			damegeSprite[i]->SetColor({ 1,1,1,alpha });
			if (damegeSprite[i]->GetColor().w <= 0)
			{
				isDestroy = true;
			}
		}
	}
}
void DamageParticle::Draw()
{
	for (int i = 0; i < dightsSize; i++)
	{
		damegeSprite[i]->Draw2();
	}
}
