#pragma once
class HitStop
{
private:
	bool isHitStop;
	int hitstopTimer;
	int hitstopMaxTimer;

public:
	void Init();
	void Update();

	inline void StartHitStop(const int& hitstopMaxTimer)
	{
		if (isHitStop == false)
		{
			hitstopTimer = 0;
			this->hitstopMaxTimer = hitstopMaxTimer;

			isHitStop = true;
		}
	};

	inline bool GetisHitStop() { return isHitStop; }

public:
	static HitStop* GetInstance();
	static void DestroyInstance();
private:
	HitStop() {};
	~HitStop() {};

	HitStop(const HitStop&) = delete;
	HitStop& operator=(const HitStop&) = delete;
	HitStop(const HitStop&&) = delete;
	HitStop& operator=(const HitStop&&) = delete;
};

