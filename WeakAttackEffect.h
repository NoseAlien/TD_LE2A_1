#pragma once
#include "Particle.h"
#include <vector>
class WeakAttackEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	int maxParticle;

public:
	WeakAttackEffect();
	~WeakAttackEffect();
	void Generate(const Vector3 pos);
	void Update();
	void Draw();
};

