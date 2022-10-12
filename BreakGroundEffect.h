#pragma once
#include "Particle.h"
#include "SlowMotion.h"
#include <vector>
class BreakGroundEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	SlowMotion* slowMotion;
	int maxParticle;

public:
	BreakGroundEffect();
	~BreakGroundEffect();
	void Generate(const Vector3& pos, const Vector3& scale);
	void Update();
	void Draw();

};

