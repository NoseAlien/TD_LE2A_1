#pragma once
#include "Particle.h"
#include <vector>
class PlayerMoveEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	int generateTimer;
	int generateMaxTimer;

public:
	PlayerMoveEffect();
	~PlayerMoveEffect();
	void Generate(const Vector3& pos, const float& radius);
	void Update();
	void Draw();
};

