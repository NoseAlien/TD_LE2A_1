#include "SlowMotion.h"

void SlowMotion::Init()
{
	slowExrate = 1;
	slowTimer = 0;
	slowMaxTimer = 0;
	addSlowExrateSpeed = 0.1;
	isAddSlowExrate = false;
}

void SlowMotion::Update()
{
	if (slowExrate < 1)
	{
		slowTimer++;
		if (slowTimer >= slowMaxTimer)
		{
			slowTimer = 0;
			isAddSlowExrate = true;
		}
	}

	if (isAddSlowExrate == true)
	{
		slowExrate += addSlowExrateSpeed;
		if (slowExrate >= 1)
		{
			slowExrate = 1;
			isAddSlowExrate = false;
		}
	}
}

SlowMotion* SlowMotion::GetInstance()
{
	static SlowMotion* slowMotion = new SlowMotion;
	return slowMotion;
}

void SlowMotion::DestroyInstance()
{
	delete SlowMotion::GetInstance();
}
