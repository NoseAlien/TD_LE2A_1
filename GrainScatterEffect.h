#pragma once
#pragma once
#include "Particle.h"
#include <vector>
class GrainScatterEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	int maxParticle;

public:
	GrainScatterEffect();
	~GrainScatterEffect();
	void Generate(const Vector3& pos, const bool& colorType);
	void Update();
	void Draw();
};
