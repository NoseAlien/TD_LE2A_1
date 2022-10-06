#include "Collision.h"
#include <memory>
using namespace std;

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