#pragma once
#include <vector>
#include "Vector3.h"
#include "Easing.h"

class BezierCurve
{
public:
	enum InterPolationType
	{
		Lerp,
		EaseIn,
		EaseOut,
	};
private:
	std::vector<Vector3> points;
	size_t startIndex;
	float interpolateTime;
	float timeRate = 0;
	int timer = 0;
	bool isEnd;

	Easing ease;

public:
	BezierCurve() {}
	BezierCurve(const float& interpolateTime);
	void ReSet();
	void Update();
	Vector3 InterPolation(const int& type);
	inline void AddPoint(const Vector3& pos) { points.push_back(pos); }
	inline void SetBackPoint(const Vector3& pos) { points.back() = pos; }
	inline bool GetisEnd() { return isEnd; }

private:
	std::vector<Vector3> RecursiveLerp(const std::vector<Vector3>& points, const int& type);
};