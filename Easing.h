#pragma once
#include "Vector3.h"

class Easing
{
private:
	int timer;
	int easeTimer;
	float timeRate;
	float powNum;
	bool isEnd;

public:
	Easing();
	Easing(const int& easeTimer);
	Easing(const int& easeTimer, const float& powNum);
	void ReSet();
	void Update();

	inline void SetEaseTimer(const int& easeTimer) { this->easeTimer = easeTimer; }
	inline void SetPowNum(const float& powNum) { this->powNum = powNum; }
	inline bool GetisEnd() { return isEnd; }

	// ラープ
	float Lerp(const float& startPos, const float& endPos);
	Vector3 Lerp(const Vector3& startPos, const Vector3& endPos);

	// イーズグイン
	float In(const float& startPos, const float& endPos);
	//Vec2 In();
	Vector3 In(const Vector3& startPos, const Vector3& endPos);

	// イーズアウト
	float Out(const float& startPos, const float& endPos);
	Vector3 Out(const Vector3& startPos, const Vector3& endPos);
};
