#pragma once
#include "Particle.h"
#include <vector>
class RepairEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	std::vector<std::unique_ptr<Particle>> effect;
	int maxParticle;

public:
	RepairEffect();
	~RepairEffect();
	void Generate(const Vector3& pos);
	void Update();
	void Draw();
	void Clear();
};

