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

	// ���[�v
	float Lerp(const float& startPos, const float& endPos);
	Vector3 Lerp(const Vector3& startPos, const Vector3& endPos);

	// �C�[�Y�O�C��
	float In(const float& startPos, const float& endPos);
	//Vec2 In();
	Vector3 In(const Vector3& startPos, const Vector3& endPos);

	// �C�[�Y�A�E�g
	float Out(const float& startPos, const float& endPos);
	Vector3 Out(const Vector3& startPos, const Vector3& endPos);
};
