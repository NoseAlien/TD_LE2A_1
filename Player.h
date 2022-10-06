#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

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

	int pushKeyFream;

public:
	Player();
	~Player();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);
};

