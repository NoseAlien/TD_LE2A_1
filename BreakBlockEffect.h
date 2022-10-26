#pragma once
#include "Particle.h"
#include "SlowMotion.h"
#include <vector>
class BreakBlockEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles{};
	SlowMotion* slowMotion = nullptr;
	int maxParticle = 0;

public:
	BreakBlockEffect();
	~BreakBlockEffect();
	void Generate(const Vector3& pos, const Vector3& scale);
	void Update();
	void Draw();

};
