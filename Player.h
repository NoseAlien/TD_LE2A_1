#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Vector3.h"

class Player
{
private:
	Input* input_ = nullptr;

	WorldTransform* trans = nullptr;	// トランスフォーム
	Model* playerModel = nullptr;		// モデル

	uint32_t playerTexture = 0;		// テクスチャー
	uint32_t redPixel = 0;			// テクスチャー

	float speed;	// 移動速度
	bool isBreak;	// まわりの星を壊すか

	bool isAttack;	// 攻撃しているかどうか
	float attackMoveSpeed;	// 攻撃時の移動速度
	bool isReverse;			// 攻撃時の反転フラグ

	bool isWeakAttack;	// 弱攻撃
	bool isHeavyAttack;	// 強攻撃

	float collisionRadius;	// 当たり判定の半径

	int pushKeyFream;	// キーを押したときのフレーム
	bool isHaveStar;	// スターを巻き込めたか?

	int stopTimer = 0;	// 床で少し止まるタイマー
	float floorPosY;	// 床の高さ

	bool isDamage;
	int damegeTimer;

private:
	void MoveUpdate();
	void AttackUpdate();
	void DamageUpdate();

public:
	void Load();

	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisHeavyAttack() { return isHeavyAttack; }
	inline bool GetisHaveStar() { return isHaveStar; }
	inline bool GetisBreak() { return isBreak; }
	inline int GetStopTimer() { return stopTimer; }
	inline float GetFloorPosY() { return floorPosY; }

	inline void SetisReverse(bool isReverse) { this->isReverse = isReverse; }
	inline void SetisHaveStar(bool isHaveStar) { this->isHaveStar = isHaveStar; }
	inline void SetFloorPosY(const float& floorPosY) { this->floorPosY = floorPosY; }
	inline void SetisDamage(const bool& isDamage) { if (damegeTimer == 0) this->isDamage = isDamage; }

public:
	static Player* GetInstance();
	static void DestroyInstance();
private:
	Player();
	~Player();

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(const Player&&) = delete;
	Player& operator=(const Player&&) = delete;

};

extern Player* player;