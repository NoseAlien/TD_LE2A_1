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

	int life;

	float speed;	// 移動速度

	bool isAttack;	// 攻撃しているかどうか
	float attackMoveSpeed;	// 攻撃時の移動速度
	bool isReverse;			// 攻撃時の反転フラグ

	bool isWeakAttack;		// 弱攻撃
	bool isHeavyAttack;		// 強攻撃
	bool isEngulfAttack;	// 巻き込み攻撃

	float collisionRadius;	// 当たり判定の半径

	int pushKeyFream;		// キーを押したときのフレーム
	int maxPushKeyFream;	// 最大値

	int haveStarNum;	// スターを巻き込めたか?

	int stopTimer = 0;	// 床で少し止まるタイマー
	int stageType;

	bool isDamage;
	int damageTimer;
	int maxDamageTimer;
	bool isHit;

	const int starAttackDamage;
	const int weakAttackDamage;
	const int heavyAttackDamage;

private:
	void MoveUpdate();
	void AttackUpdate();
	void DamageUpdate();

public:
	void Load();

	void Init(const int& stageType);
	void Update();
	inline void UpdateMatrix() { trans->UpdateMatrix(); }
	void Draw(const ViewProjection& viewProjection_);

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return { 1,1,1 };/*return trans->scale_;*/ }
	inline float GetSpeed() { return speed; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisWeakAttack() { return isWeakAttack; }
	inline bool GetisHeavyAttack() { return isHeavyAttack; }
	inline bool GetisEngulfAttack() { return isEngulfAttack; }
	inline int GetHaveStarNum() { return haveStarNum; }
	// 攻撃のダメージ
	inline int GetStarAttackDamage() { return starAttackDamage; }
	inline int GetWeakAttackDamage() { return weakAttackDamage; }
	inline int GetHeavyAttackDamage() { return heavyAttackDamage; }
	inline float GetAttackMoveSpeed() { return attackMoveSpeed; }
	inline bool GetisReverse() { return isReverse; }

	inline int GetStopTimer() { return stopTimer; }
	inline int GetLife() { return life; }

	inline void HaveStarNumIncriment() { haveStarNum++; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisReverse(bool isReverse) { this->isReverse = isReverse; }
	inline void SetisEngulfAttack(const bool& isEngulfAttack) { this->isEngulfAttack = isEngulfAttack; }
	inline void SetHaveStarNum(const int& haveStarNum) { this->haveStarNum = haveStarNum; }

	inline void SetisDamage(const bool& isDamage)
	{
		if (damageTimer == 0)
		{
			this->isDamage = isDamage;
			pushKeyFream = 0;
			life--;
		}
	}

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