#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Vector3.h"

class Player
{
private:
	WorldTransform* trans = nullptr;	// トランスフォーム
	Model* playerModel = nullptr;		// モデル
	uint32_t playerTexture = 0;			// テクスチャー
	uint32_t redPixel = 0;				// テクスチャー

	Input* input_ = nullptr;

	bool isAttack;	// 攻撃しているかどうか

	bool isWeakAttack;	// 弱攻撃
	float weakAttackMoveSpeed;	// 弱攻撃時の速度
	bool isHeavyAttack;	// 強攻撃
	float heavyAttackMoveSpeed;	// 強攻撃時の速度
	float collisionRadius;
	bool isReverse;

	int pushKeyFream;
	bool isHaveStar;

public:
	Player();
	~Player();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline Vector3 GetPos() { return trans->translation_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisHeavyAttack() { return isHeavyAttack; }
	inline bool GetisHaveStar() { return isHaveStar; }

	inline void SetisReverse(bool isReverse) { this->isReverse = isReverse; }
	inline void SetisHaveStar(bool isHaveStar) { this->isHaveStar = isHaveStar; }


};

