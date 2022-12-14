#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "GrainMoveEffect.h"
#include "BezierCurve.h"
#include "Easing.h"
#include <memory>

struct RevivalObj1
{
	bool isCanRevival = false;
	bool isRevival = false;
	int timer = 0;
	int maxTimer = 180;
	Vector3 pos;
};

class Star
{

private:
	std::unique_ptr<GrainMoveEffect> grainMoveEffect;

public:
	std::unique_ptr<RevivalObj1> revival;

private:
	WorldTransform* trans = nullptr;
	static Model* starModel; //= nullptr;		// モデル
	static std::unique_ptr<Model> powerUpModel;
	static std::unique_ptr<Model> kotubuLightModel;

	std::unique_ptr<WorldTransform> kotubuLightTrans;

	std::unique_ptr<Sprite> kotubuLightSprite;
public:
	static uint32_t kotubuLightTex;

private:
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
	// 色変わる関連
	int changeColorTimer;
	int changeColorMaxTimer;
	bool alwaysChangeColor;

public:
	bool isChangeColor;

private:
	// 床に吸い込まれる関連
	bool isSucked;
	int suckedMaxTimer;
	int suckedTimer;
public:
	// 床に吸い込まれる関連
	std::unique_ptr<BezierCurve> suckedCurve;

private:
	// 攻撃関連
	bool isAttack;

private: // アニメーション関連
	int animeIndex;
	int fream;
	int maxFream;

private:
	// 生成の見た目関連
	bool isGenerate;
	Easing geneAddScaleEase;

public:
	Star();
	~Star();
	static void Load();
	static void UnLoad();
	void Generate(const Vector3& pos, const Vector3& dirVec, const int& generateType);
	void GenerateUpdate();
	void Update();
	void AttackUpdate();
	void SuckedUpdate();
	void Draw(const ViewProjection& viewProjection_);

	void UpdateEffect();
	void DrawEffectBack();
	void DrawBackLight();

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
	inline void SetisGenerate(const bool& isGenerate) { this->isGenerate = isGenerate; }
	inline void SetAlwaysChangeColor(const bool& alwaysChangeColor) { this->alwaysChangeColor = alwaysChangeColor; }
	//inline void SetisSucked(const bool& isSucked) { this->isSucked = isSucked; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisDestroy() { return isDestroy; }
	inline bool GetisAttack() { return isAttack; }
	inline bool GetisGround() { return isGround; }
	inline int GetDir() { return dir; }
	inline bool GetisSucked() { return isSucked; }
	inline bool GetisChangeColor() { return isChangeColor; }
	//inline void SetAngleShakeValue(const Vector3& angleShakeValue) { this->angleShakeValue = angleShakeValue; }
};

