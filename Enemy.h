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
	uint32_t enemyTexture2 = 0;			// テクスチャー

	int hp;
	bool isAddScale;		// 拡大のフラグ
	float collisionRadius;	// 当たり判定の半径

	int recoveryTimer;		// 回復するタイマー
	int maxRecoveryTimer;	// 最大値


public:
	void Load();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline void Damage(const int subhp) { hp -= subhp; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline int GetHP() { return hp; }

	inline void SetisAddScale(const bool& isAddScale) { this->isAddScale = isAddScale; }

public:
	static Enemy* GetInstance();
	static void DestroyInstance();
private:
	Enemy();
	~Enemy();

	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;
	Enemy(const Enemy&&) = delete;
	Enemy& operator=(const Enemy&&) = delete;
};

extern Enemy* enemy;