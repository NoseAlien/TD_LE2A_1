#pragma once
#include "Vector3.h"
#include "Sprite.h"
#include "Easing.h"
#include <memory>
#include <vector>

struct DamageParticle
{
	std::unique_ptr<Sprite> damegeSprite[2];
	int dightsSize;
	Easing ease;
	bool isDestroy;
	Vector4 color;
	void Generate(const Vector3& pos, std::vector<int> dightsNumber);
	void Update();
	void Draw();
};

class DamageEffect
{
private:
	std::vector<std::unique_ptr<DamageParticle>> p;
public:
	void Generate(const Vector3& pos, std::vector<int> dightsNumber);
	void Update();
	void Draw();
	void Clear();

public:
	static DamageEffect* GetInstance();
	static void DestroyInstance();
private:
	DamageEffect() {}
	~DamageEffect() {}

	DamageEffect(const DamageEffect&) = delete;
	DamageEffect& operator=(const DamageEffect&) = delete;
	DamageEffect(const DamageEffect&&) = delete;
	DamageEffect& operator=(const DamageEffect&&) = delete;
};

