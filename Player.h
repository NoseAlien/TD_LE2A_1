#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Audio.h"
#include "Input.h"
#include "Vector3.h"

#include "WeakAttackEffect.h"
#include "HeavyAttackEffect.h"
#include "PlayerDieEffect.h"
#include "PlayerMoveEffect.h"
#include "SlowMotion.h"

class Player
{
public:
	static Audio* audio;

private:
	std::unique_ptr<WeakAttackEffect> weakAttackEffect;
	std::unique_ptr<HeavyAttackEffect> heavyAttackEffect;
	std::unique_ptr<PlayerDieEffect> playerDieEffect;
	std::unique_ptr<PlayerMoveEffect> playerMoveEffect;

public:	// テクスチャー
	static uint32_t playerTexAnime[9];		// テクスチャーアニメション
	int animeIndex;
	int fream;
	int maxFream;
	static uint32_t redPixel;		// テクスチャー

private:
	Input* input_ = nullptr;

	WorldTransform* trans = nullptr;	// トランスフォーム
	Model* playerModel = nullptr;		// モデル
	std::unique_ptr<Model> collapseModel = nullptr;
	std::unique_ptr<WorldTransform> collapseTrans = nullptr;

private:
	// SE関連	
	uint32_t jumpSE = 0;
	uint32_t damageSE = 0;

	SlowMotion* slowMotion;

private:
	// ライフ関連
	static uint32_t heartTexture;
	std::vector<Sprite*> heartSprites;
	int life;
	bool isAlive;

	float maxSpeed;	// 移動速度
	float speed;	// 移動速度
	float radius;	// 半径
	float bigRadius; // 溜めてる時の半径

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
	bool isGround;	// 地面にいるかどうかのフラグ
	bool isJump;	// 地面から天井に戻るフラグ

private:
	// 攻撃関連
	const int starAttackDamage;
	const int weakAttackDamage;
	const int heavyAttackDamage;

	const float addScaleValue = 0.5;
	int addScaleStep = 0;
	float maxSize = 2.5;

	int isJumpAddScaleStep = 0;
	bool isHitGround;

private:
	// ブロックの当たった関連
	bool isHitBlock;
	int hitedBlockStep;

	bool isAttackBlock;	// 上から攻撃したら

private:
	// 自機のスポーンモデル関連
	static std::unique_ptr<Model> spawnModel;
	std::unique_ptr<WorldTransform> spawnTrans = nullptr;	// トランスフォーム

private:
	void MoveUpdate();
	void AttackUpdate();
	void DamageUpdate();
	void HitBlockUpdate();

public:
	void Load();

	void Init();
	void Update();
	void SelectSceneUpdate();
	void Draw(const ViewProjection& viewProjection_);
	void DrawSpriteBack();
	void DrawSpriteFront();

	void EffectGenerate(const Vector3& pos);
	void EffectUpdate();
	void DieEffectGenerate();
	inline bool GetDieEffectisEnd() { return playerDieEffect->GetisEnd(); }
	inline void SetDieEffectisEnd(const bool& isEnd) { playerDieEffect->SetisEnd(isEnd); }

	void SetisDamage(const bool& isDamage);

	inline void UpdateMatrix() { trans->UpdateMatrix(); }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { /*return { 1,1,1 };*/return trans->scale_; }
	inline float GetSpeed() { return speed; }
	inline float GetRadius() { return radius; }
	inline bool GetisWeakAttack() { return isWeakAttack; }
	inline bool GetisHeavyAttack() { return isHeavyAttack; }
	inline bool GetisEngulfAttack() { return isEngulfAttack; }
	inline int GetHaveStarNum() { return haveStarNum; }
	// 攻撃のダメージ
	inline int GetStarAttackDamage() { return starAttackDamage; }
	inline int GetWeakAttackDamage() { return weakAttackDamage; }
	inline int GetHeavyAttackDamage() { return heavyAttackDamage; }
	inline float GetAttackMoveSpeed() { return attackMoveSpeed; }
	inline bool GetisReverse() { return isReverse && addScaleStep == 2; }
	inline bool GetisReverse2() { return isReverse; }

	inline int GetStopTimer() { return stopTimer; }
	inline int GetLife() { return life; }
	inline bool GetisAlive() { return isAlive; }
	inline bool GetisGround() { return isGround; }
	inline bool GetisJump() { return isJump; }
	inline int GetisJumpAddScaleStep() { return isJumpAddScaleStep; }
	inline bool GetisHitBlock() { return isHitBlock; }
	inline bool GetisAttackBlock() { return isAttackBlock; }

	inline void HaveStarNumIncriment() { haveStarNum++; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }
	inline void SetisReverse(const bool& isReverse) { this->isReverse = isReverse; }
	inline void SetisEngulfAttack(const bool& isEngulfAttack) { this->isEngulfAttack = isEngulfAttack; }
	inline void SetHaveStarNum(const int& haveStarNum) { this->haveStarNum = haveStarNum; }
	inline void SetStageType(const int& stageType) { this->stageType = stageType; }
	inline void SetLife(const int& life) { this->life = life; }
	inline void SetisHitBlock(const bool& isHitBlock) { this->isHitBlock = isHitBlock; }
	inline void SetSpeed(const float& speed) { this->speed = speed; }
	inline void SetisGround(const bool& isGround) { this->isGround = isGround; }
	inline void SetAttackMoveSpeed(const float& attackMoveSpeed) { this->attackMoveSpeed = attackMoveSpeed; }
	inline void SetisHitGround(const bool& isHitGround) { this->isHitGround = isHitGround; }
	inline void SetisAttackBlock(const bool& isAttackBlock) { this->isAttackBlock = isAttackBlock; }

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