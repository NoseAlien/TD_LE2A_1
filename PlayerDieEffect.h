#pragma once
#include "Particle.h"
#include <vector>

class PlayerDieEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	int maxParticle;

public:
	PlayerDieEffect();
	~PlayerDieEffect();
	void Generate(const Vector3& pos);
	void Update();
	void Draw();
};

