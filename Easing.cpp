#include "Easing.h"
#include "GameScene.h"
#include <math.h>

Easing::Easing() :
	timer(0), timeRate(0),
	easeTimer(0), powNum(1), isEnd(false)
{
}

Easing::Easing(const int& easeTimer) :
	timer(0), timeRate(0),
	easeTimer(easeTimer), powNum(1), isEnd(false)
{
}

Easing::Easing(const int& easeTimer, const float& powNum) :
	timer(0), timeRate(0),
	easeTimer(easeTimer), powNum(powNum), isEnd(false)
{
}

void Easing::ReSet()
{
	timer = 0;
	timeRate = 0;
	isEnd = false;
}

void Easing::Update()
{
	timer++;
	if (timeRate >= 1)
	{
		isEnd = true;
	}
	else
	{
		timeRate = min((float)timer / easeTimer, 1);
	}
}

float Easing::Lerp(const float& startPos, const float& endPos)
{
	float dis = endPos - startPos;
	return dis * timeRate + startPos;
}

Vector3 Easing::Lerp(const Vector3& startPos, const Vector3& endPos)
{
	Vector3 dis = endPos - startPos;
	return dis * timeRate + startPos;
}

float Easing::In(const float& startPos, const float& endPos)
{
	float dis = endPos - startPos;
	return dis * powf(timeRate, powNum) + startPos;
}

Vector3 Easing::In(const Vector3& startPos, const Vector3& endPos)
{
	Vector3 dis = endPos - startPos;
	return dis * powf(timeRate, powNum) + startPos;
}

float Easing::Out(const float& startPos, const float& endPos)
{
	float dis = endPos - startPos;
	if ((int)powNum % 2 == 1)
	{
		return dis * (powf(timeRate - 1, powNum) + 1) + startPos;
	}
	else if ((int)powNum % 2 == 0)
	{
		return dis * -1 * (powf(timeRate - 1, powNum) - 1) + startPos;
	}
}

Vector3 Easing::Out(const Vector3& startPos, const Vector3& endPos)
{
	Vector3 dis = endPos - startPos;
	if ((int)powNum % 2 == 1)
	{
		return dis * (powf(timeRate - 1, powNum) + 1) + startPos;
	}
	else if ((int)powNum % 2 == 0)
	{
		return dis * -1 * (powf(timeRate - 1, powNum) - 1) + startPos;
	}
}