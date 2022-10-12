#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Block
{
private:
	WorldTransform* trans = nullptr;
	Model* blockModel = nullptr;		// ƒ‚ƒfƒ‹
	uint32_t blockTexture;

	int hp;
	int maxhp;
	bool isDestroy;
	int isHit;

public:
	Block();
	~Block();
	void Generate(const Vector3& pos, const Vector3& scale);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);

	inline void Damage(const int& subhp) { hp -= subhp; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline bool GetisDestroy() { return isDestroy; }
	inline int GetisHit() { return isHit; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisHit(const int& isHit) { this->isHit = isHit; }
};

