#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"
#include "BreakGroundEffect.h"
#include "Audio.h"
#include <memory>

class Ground
{
public:
	static Audio* audio;

private:
	// SE関連
	uint32_t damageSE;
	uint32_t largeDamageSE;
	uint32_t defeatSE;
	uint32_t recoverySE;

	static Model* enemyModel;			// モデル
	WorldTransform* trans = nullptr;	// トランスフォーム
	uint32_t enemyTexture = 0;			// テクスチャー
	uint32_t enemyDangerTexture = 0;	// テクスチャー

	bool isAlive;
	int hp;
	int maxhp;
	bool isAddScale;		// 拡大のフラグ
	float collisionRadius;	// 当たり判定の半径
	int isAddScaleCountDown;
	int addScaleCount;
	float addScale;
	float preScaleY;

	int maxRecoveryTimer;	// 最大値

	bool isDanger;	// 星が八個になっているか
	bool isSuctionStar;	// 星を吸収するフラグ
	int isHit;

private:
	// 表情関連
	std::unique_ptr<Sprite> faceSprite;

	// アイドル状態
	static uint32_t idleTexture;

	// まばたき関連
	static std::vector<uint32_t> blinkTexture;
	int timer;
	int maxTimer;	// 瞬きをする間隔
	bool isBlink;
	int blinkAnimeTimer;
	int blinkAnimeMaxTimer;
	int blinkAnimeIndex;

	// 驚き関連
	static std::vector<uint32_t> surprisedTexture;
	bool isSurprised;
	int surprisedAnimeTimer;
	int surprisedAnimeMaxTimer;
	int surprisedAnimeIndex;
	int surprisedCount;	// 一回につきアニメションを回す回数

	// 泣き関連
	static std::vector<uint32_t> cryTexture;
	bool isCry;
	int cryAnimeTimer;
	int cryAnimeMaxTimer;
	int cryAnimeIndex;

	// ひび表現関連
	static std::vector<uint32_t> groundCrackTexture;

private:
	// エフェクト関連
	std::unique_ptr<BreakGroundEffect> breakGroundEffect;
	bool isGeneEffect;

public:
	void Load();
	void Init(const int& maxhp);
	void Update();
	void Draw(const ViewProjection& viewProjection_);
	void DrawSprite();
	void EffectUpdate();
	void EffectDraw();
	void SetThickness(const int& num);

	inline void Damage(const int subhp)
	{
		hp -= subhp;
		isSurprised = true;
		audio->PlayWave(damageSE);
	}

	inline void LargeDamage(const int subhp)
	{
		hp -= subhp;
		isSurprised = true;
		audio->PlayWave(largeDamageSE);
	}

	inline void AddHP(const int& addhp)
	{
		if (isAlive == true)
		{
			hp += addhp;
			trans->scale_.y += 1.2f / 15;
			audio->PlayWave(recoverySE);
		}
	}

	inline Vector3 GetMouthPos()
	{
		return
		{
			trans->translation_.x + 32,
			trans->translation_.y + 6 * trans->scale_.y,
			trans->translation_.z
		};
	}

	inline Vector3 GetPos() { return trans->translation_; }
	//inline Vector3 GetScale() { return trans->scale_; }
	inline Vector3 GetScale() { return { 46,10 * trans->scale_.y,5 }; }
	inline Vector3 GetScaleExrate() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline int GetHP() { return hp; }
	inline int GetAddScaleCount() { return addScaleCount; }
	inline int GetisAddScaleCountDown() { return isAddScaleCountDown; }
	inline bool GetisAddScale() { return isAddScale; }
	inline bool GetisSuctionStar() { return isSuctionStar; }
	inline int GetisHit() { return isHit; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisDanger(const bool& isDanger) { this->isDanger = isDanger; }
	inline void SetisAddScale(const bool& isAddScale) { this->isAddScale = isAddScale; }
	inline void SetisAddScaleCountDown(const int& isAddScaleCountDown) { this->isAddScaleCountDown = isAddScaleCountDown; }
	inline void SetAddScaleCount(const int& addScaleCount) { this->addScaleCount = addScaleCount; }
	inline void SetisSuctionStar(const bool& isSuctionStar) { this->isSuctionStar = isSuctionStar; }
	inline void SetisHit(const int& isHit) { this->isHit = isHit; }
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }

public:
	static Ground* GetInstance();
	static void DestroyInstance();
private:
	Ground();
	~Ground();

	Ground(const Ground&) = delete;
	Ground& operator=(const Ground&) = delete;
	Ground(const Ground&&) = delete;
	Ground& operator=(const Ground&&) = delete;
};

extern Ground* ground;