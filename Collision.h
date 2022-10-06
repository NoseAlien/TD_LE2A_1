#pragma once
#include "Vector3.h"

class Collision
{
public:
	bool SphereHitSphere(const Vector3& pos1, float radius1, const Vector3& pos2, float radius2);

	static Collision* GetInstance();
	static void DestroyInstance();
private:
	Collision() = default;
	~Collision() = default;

	Collision(const Collision&) = delete;
	Collision& operator=(const Collision&) = delete;
	Collision(const Collision&&) = delete;
	Collision& operator=(const Collision&&) = delete;
};

extern Collision* collision;