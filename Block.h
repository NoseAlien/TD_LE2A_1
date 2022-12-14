#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Easing.h"
#include "Audio.h"

struct RevivalObj
{
	bool isCanRevival = false;
	bool isRevival = false;
	int timer = 0;
	int maxTimer = 180;
	Vector3 pos;
};

class Block
{
public:
	static Audio* audio;
	static uint32_t damageSE;
	static uint32_t breakSE;

private:
	static Model* blockModel;		// モデル
	WorldTransform* trans = nullptr;
	static uint32_t blockBreakTexture;

	int hp;
	int maxhp;
	bool isDestroy;
	int isHit;
	bool haveStar;

public:
	std::unique_ptr<RevivalObj> revival;

private:
	// 生成の見た目関連
	bool isGenerate;
	Easing geneAddScaleEase;
	Easing geneAddRotEase;

public:
	Block();
	~Block();
	static void Load();
	static void UnLoad();
	void GenerateUpdate();
	void Generate(const Vector3& pos, const Vector3& scale, const bool& haveStar);
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	void Damage(const int& subhp);

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline bool GetisDestroy() { return isDestroy; }
	inline int GetisHit() { return isHit; }
	inline bool GetHaveStar() { return haveStar; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisHit(const int& isHit) { this->isHit = isHit; }
	inline void SetisGenerate(const bool& isGenerate) { this->isGenerate = isGenerate; }
	inline void SetisDestroy(const bool& isDestroy) { this->isDestroy = isDestroy; }
};

