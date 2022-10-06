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

	float gravity;
	bool isNotGravity;
	float floorPosY;
	int dir;	// 1 ‰EA-1 ¶
	bool isMove;
	float collisionRadius;

public:
	Star();
	~Star();
	void Init();
	void Generate(const Vector3 pos, const float& floorPosY, const int& dir);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);

	WorldTransform* GetTrans() { return trans; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisMove() { return isMove; }
};

