#include "MathUtility.h"

float DegreeToRad(float num)
{
	return num / 180 * MathUtility::PI;
}

float RadToDegree(float num)
{
	return num / MathUtility::PI * 180;
}