#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "GrainMoveEffect.h"
#include <memory>

class Star
{
private:
	std::unique_ptr<GrainMoveEffect> grainMoveEffect;

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
	bool isDestroy;	// 削除可能なフラグ
	bool isGround;

	int dir;

private:
	// 攻撃関連
	bool isAttack;

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
	void AttackUpdate();
	void Draw(const ViewProjection& viewProjection_);

	void UpdateEffect();
	void DrawEffectBack();

	WorldTransform* GetTrans() { return trans; }
	inline bool GetisCanHit() { return isCanHit; }
	inline bool GetGenerateType() { return generateType; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetDirVec(const Vector3& dirVec) { this->dirVec = dirVec; }
	inline void SetisAngleShacke(const bool& isAngleShake) { this->isAngleShake = isAngleShake; }
	inline void SetSpeed(const float& speed) { this->speed = speed; }
	inline void SetGravity(const float& gravity) { this->gravity = gravity; }
	inline void SetisDestroy(const bool& isDestroy) { this->isDestroy = isDestroy; }
	inline void SetisGround(const bool& isGround) { this->isGround = isGround; }
	inline void SetisCanHit(const bool& isCanHit) { this->isCanHit = isCanHit; }
	inline void SetisAttack(const bool& isAttack) { this->isAttack = isAttack; }
	inline void SetDir(const int& dir) { this->dir = dir; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisDestroy() { return isDestroy; }
	inline bool GetisAttack() { return isAttack; }
	inline bool GetisGround() { return isGround; }
	inline int GetDir() { return dir; }
	//inline void SetAngleShakeValue(const Vector3& angleShakeValue) { this->angleShakeValue = angleShakeValue; }
};

