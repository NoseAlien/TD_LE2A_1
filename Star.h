#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class Star
{
private:
	WorldTransform* trans = nullptr;
	Model* starModel = nullptr;		// ƒ‚ƒfƒ‹
	uint32_t starTexture;

	Input* input_ = nullptr;

	float speed;
	bool isCanHit;
	int canHitTimer;
	int maxCanHitTimer;

	float gravity;
	bool isNotGravity;
	Vector3 dirVec;	// 1 ‰EA-1 ¶
	float collisionRadius;

	bool isAngleShake;
	Vector3 angleShakeValue;

	int generateType;	// ‚OƒvƒŒƒCƒ„[¶¬,‚P‘å–C¶¬

public:
	Star();
	~Star();
	void Generate(const Vector3& pos, const Vector3& dirVec, const int& generateType);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);

	WorldTransform* GetTrans() { return trans; }
	inline bool GetisCanHit() { return isCanHit; }
	inline bool GetGenerateType() { return generateType; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisAngleShacke(const bool& isAngleShake) { this->isAngleShake = isAngleShake; }
	inline void SetSpeed(const float& speed) { this->speed = speed; }
	inline void SetGravity(const float& gravity) { this->gravity = gravity; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline void SetAngleShakeValue(const Vector3& angleShakeValue) { this->angleShakeValue = angleShakeValue; }
};

