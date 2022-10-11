#pragma once
#include "Particle.h"
#include "SlowMotion.h"
#include <vector>
class HeavyAttackEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	int maxParticle;

public:
	HeavyAttackEffect();
	~HeavyAttackEffect();
	void Generate(const Vector3& pos);
	void Update();
	void Draw();
};

