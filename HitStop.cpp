#include "HitStop.h"

void HitStop::Init()
{
	isHitStop = 0;
	hitstopTimer = 0;
	hitstopMaxTimer = 0;
}

void HitStop::Update()
{
	if (isHitStop == false) return;

	hitstopTimer++;
	if (hitstopTimer >= hitstopMaxTimer)
	{
		hitstopTimer = 0;
		isHitStop = false;
	}
}

HitStop* HitStop::GetInstance()
{
	static HitStop* hitstop = new HitStop;
	return hitstop;
}

void HitStop::DestroyInstance()
{
	delete  HitStop::GetInstance();
}
