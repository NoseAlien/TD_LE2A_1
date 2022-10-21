#pragma once
#include "Particle.h"
#include <vector>
class WindPressureEffect
{
public:
	std::vector<std::unique_ptr<Particle>> waves;

public:
	void Generate(const Vector3& pos, const int& dir);
	void Update();
	void Draw();
	void Clear();

public:
	static WindPressureEffect* GetInstance();
	static void DestroyInstance();
private:
	WindPressureEffect();
	~WindPressureEffect();

	WindPressureEffect(const WindPressureEffect&) = delete;
	WindPressureEffect& operator=(const WindPressureEffect&) = delete;
	WindPressureEffect(const WindPressureEffect&&) = delete;
	WindPressureEffect& operator=(const WindPressureEffect&&) = delete;

};