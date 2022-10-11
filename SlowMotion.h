#pragma once

class SlowMotion
{
private:
	float slowExrate;
	float addSlowExrateSpeed;
	int slowTimer;
	int slowMaxTimer;
	bool isAddSlowExrate;

public:
	void Init();
	void Update();
	void StartSlowMotion(const float& slowExrate, const int& slowMaxTimer)
	{
		this->slowExrate = slowExrate;
		this->slowMaxTimer = slowMaxTimer;
	}

	inline float GetSlowExrate() { return slowExrate; }

public:
	static SlowMotion* GetInstance();
	static void DestroyInstance();
private:
	SlowMotion() {};
	~SlowMotion() {};

	SlowMotion(const SlowMotion&) = delete;
	SlowMotion& operator=(const SlowMotion&) = delete;
	SlowMotion(const SlowMotion&&) = delete;
	SlowMotion& operator=(const SlowMotion&&) = delete;
};

