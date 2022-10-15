#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Block
{
private:
	static Model* blockModel;		// ƒ‚ƒfƒ‹
	WorldTransform* trans = nullptr;
	uint32_t blockTexture;

	int hp;
	int maxhp;
	bool isDestroy;
	int isHit;
	bool haveStar;

public:
	Block();
	~Block();
	static void Load();
	static void UnLoad();
	void Generate(const Vector3& pos, const Vector3& scale, const bool& haveStar);
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline void Damage(const int& subhp) { hp -= subhp; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline bool GetisDestroy() { return isDestroy; }
	inline int GetisHit() { return isHit; }
	inline bool GetHaveStar() { return haveStar; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisHit(const int& isHit) { this->isHit = isHit; }
};

