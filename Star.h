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

	float gravity;
	bool isNotGravity;
	float floorPosY;
	int dir;	// 1 ‰EA-1 ¶
	float collisionRadius;

	bool isAngleShake;
	Vector3 angleShakeValue;

public:
	Star();
	~Star();
	void Generate(const Vector3 pos, const int& dir);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);

	WorldTransform* GetTrans() { return trans; }
	inline bool GetisCanHit() { return isCanHit; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisAngleShacke(const bool& isAngleShake) { this->isAngleShake = isAngleShake; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline void SetFloorPosY(const float& floorPosY) { this->floorPosY = floorPosY; }
	inline void SetAngleShakeValue(const Vector3& angleShakeValue) { this->angleShakeValue = angleShakeValue; }
};

