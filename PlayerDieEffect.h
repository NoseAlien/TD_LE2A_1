#pragma once
#include "Particle.h"
#include <vector>

class PlayerDieEffect
{
private:
	std::vector<std::unique_ptr<Particle>> particles;
	int maxParticle;
	int count;
	bool isEnd;

public:
	PlayerDieEffect();
	~PlayerDieEffect();
	void Generate(const Vector3& pos);
	void Update();
	void Draw();
	void Clear();

	inline bool GetisEnd() { return	isEnd; }
	inline void SetisEnd(const bool& isEnd)
	{
		this->isEnd = isEnd;
		count = 0;
	}
};

