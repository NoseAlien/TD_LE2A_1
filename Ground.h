#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"

class Ground
{
private:
	WorldTransform* trans = nullptr;	// トランスフォーム
	Model* enemyModel = nullptr;		// モデル
	uint32_t enemyTexture = 0;			// テクスチャー
	uint32_t enemyTexture2 = 0;			// テクスチャー

	int hp;
	int maxhp;
	bool isAddScale;		// 拡大のフラグ
	float collisionRadius;	// 当たり判定の半径
	int isAddScaleCountDown;
	int addScaleCount;
	float addScale;
	float preScaleY;


	int maxRecoveryTimer;	// 最大値

	bool isDanger;	// 星が八個なったいるか
	bool isSuctionStar;	// 星を吸収するフラグ

public:
	void Load();
	void Init(const int& maxhp);
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline void Damage(const int subhp) { hp -= subhp; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline int GetHP() { return hp; }
	inline int GetAddScaleCount() { return addScaleCount; }
	inline int GetisAddScaleCountDown() { return isAddScaleCountDown; }
	inline bool GetisAddScale() { return isAddScale; }
	inline bool GetisSuctionStar() { return isSuctionStar; }

	inline void SetisDanger(const bool& isDanger) { this->isDanger = isDanger; }
	inline void SetisAddScale(const bool& isAddScale) { this->isAddScale = isAddScale; }
	inline void SetisAddScaleCountDown(const int& isAddScaleCountDown) { this->isAddScaleCountDown = isAddScaleCountDown; }
	inline void SetAddScaleCount(const int& addScaleCount) { this->addScaleCount = addScaleCount; }
	inline void SetisSuctionStar(const bool& isSuctionStar) { this->isSuctionStar = isSuctionStar; }

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