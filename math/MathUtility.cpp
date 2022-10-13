#include "MathUtility.h"

float DegreeToRad(float num)
{
	return num / 180 * MathUtility::PI;
}

float RadToDegree(float num)
{
	return num / MathUtility::PI * 180;
}

DWORD GetNowTime()
{
	return GetTickCount64() / 10;
}

int GetDigit(const int& number)
{
	int digit = 0;
	int num = number;
	while (num != 0)
	{
		num /= 10;
		digit++;
	}
	return digit;
}

std::vector<int> GetDightsNumber(const int& number)
{
	std::vector<int> nums;
	int dight = GetDigit(number);
	int num = number;
	nums.resize(dight);

	for (int i = dight - 1; i > -1; i--)
	{
		nums[i] = num % 10;
		num = num / 10;
	}

	return nums;
}
