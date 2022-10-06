#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"

class Enemy
{
private:
	WorldTransform* trans = nullptr;	// トランスフォーム
	Model* enemyModel = nullptr;		// モデル
	uint32_t enemyTexture = 0;			// テクスチャー
	float collisionRadius;
	int hp;

	bool isAttack;	// 攻撃しているかどうか

public:
	Enemy();
	~Enemy();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline void Damage(const int subhp) { hp -= subhp; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline float GetRadius() { return collisionRadius; }
	inline float GetHP() { return hp; }
};

