#include "BezierCurve.h"
#include "Easing.h"
using namespace std;

BezierCurve::BezierCurve(const float& interpolateTime) :
	interpolateTime(interpolateTime), startIndex(1), isEnd(false)
{
	ease.SetEaseTimer(interpolateTime);
	ease.SetPowNum(3);
}

void BezierCurve::ReSet()
{
	timer = 0;
	timeRate = 0;
	ease.ReSet();
	points.clear();
	isEnd = false;
}

void BezierCurve::Update()
{
	if (points.size() < 2) return;

	ease.Update();
	if (ease.GetisEnd() == true)
	{
		isEnd = true;
	}
}

Vector3 BezierCurve::InterPolation(const int& type)
{
	if (RecursiveLerp(points, type).size() < 2) return { 0,0,0 };

	// “ñ‰ñ
	switch (type)
	{
	case Lerp:
		return ease.Lerp(RecursiveLerp(points, type)[0], RecursiveLerp(points, type)[1]);
		break;
	case EaseIn:
		return ease.In(RecursiveLerp(points, type)[0], RecursiveLerp(points, type)[1]);
		break;
	case EaseOut:
		return ease.Out(RecursiveLerp(points, type)[0], RecursiveLerp(points, type)[1]);
		break;
	}
}

vector<Vector3> BezierCurve::RecursiveLerp(const vector<Vector3>& points, const int& type)
{
	vector<Vector3> tempPoints;

	if (points.size() < 3)
	{
		return points;
	}
	else
	{
		switch (type)
		{
		case Lerp:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(ease.Lerp(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case EaseIn:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(ease.In(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case EaseOut:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(ease.Out(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;
		}
	}
}