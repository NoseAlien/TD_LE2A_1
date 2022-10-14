#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class Star
{
private:
	WorldTransform* trans = nullptr;
	static Model* starModel; //= nullptr;		// モデル
	uint32_t starTexture;

	Input* input_ = nullptr;

	float speed;
	bool isCanHit;
	int canHitTimer;
	int maxCanHitTimer;
	int stageType;

	float gravity;
	bool isNotGravity;
	Vector3 dirVec;	// 1 右、-1 左
	float collisionRadius;

	bool isAngleShake;

	int generateType;	// ０プレイヤー生成,１大砲生成

private: // アニメーション関連
	int animeIndex;
	int fream;
	int maxFream;

public:
	Star();
	~Star();
	static void Load();
	static void UnLoad();
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
	//inline void SetAngleShakeValue(const Vector3& angleShakeValue) { this->angleShakeValue = angleShakeValue; }
};

