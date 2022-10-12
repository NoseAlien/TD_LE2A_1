#include "Collision.h"
#include <memory>
using namespace std;

SquareCollider::SquareCollider() :
	pos(0, 0), scale(0, 0),
	leftTop(0, 0), rightDown(0, 0)
{
}

SquareCollider::SquareCollider(const Vector2& pos, const Vector2& scale) :
	pos(pos), scale(scale),
	leftTop(pos.x - scale.x, pos.y + scale.y), rightDown(pos.x + scale.x, pos.y - scale.y)
{
}

bool Collision::SphereHitSphere(const Vector3& pos1, float radius1, const Vector3& pos2, float radius2)
{
	return
	{
		(pos2.x - pos1.x) * (pos2.x - pos1.x) +
		(pos2.y - pos1.y) * (pos2.y - pos1.y) +
		(pos2.z - pos1.z) * (pos2.z - pos1.z) <=
		(radius2 + radius1) * (radius2 + radius1)
	};
}

bool Collision::SquareHitSquare(const SquareCollider& square1, const SquareCollider& square2)
{
	return
	{
		square2.leftTop.x < square1.rightDown.x&&
		square1.leftTop.x < square2.rightDown.x&&
		square2.leftTop.y > square1.rightDown.y&&
		square1.leftTop.y > square2.rightDown.y
	};

	//return (bx1 < ax2) && (ax1 < bx2) && (by1 < ay2) && (ay1 < by2);

}

Collision* Collision::GetInstance()
{
	static Collision* collision = new Collision;
	return collision;
}

void Collision::DestroyInstance()
{
	delete GetInstance();
}

Collision* collision = Collision::GetInstance();

