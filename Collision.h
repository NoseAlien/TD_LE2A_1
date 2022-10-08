#pragma once
#include "Vector3.h"
#include "Vector2.h"

class SquareCollider
{
public:
	Vector2 pos;
	Vector2 scale;

	Vector2 leftTop;
	Vector2 rightDown;

public:
	SquareCollider();
	SquareCollider(const Vector2& pos, const Vector2& scale);
};

class Collision
{
public:
	bool SphereHitSphere(const Vector3& pos1, float radius1, const Vector3& pos2, float radius2);
	bool SquareHitSquare(const SquareCollider& square1, const SquareCollider& square2);

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