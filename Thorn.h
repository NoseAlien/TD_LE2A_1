#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

class Thorn
{
private:
	static Model* thornModel;	// モデル
	WorldTransform* trans = nullptr;	// トランスフォーム
	uint32_t thornTexture = 0;			// テクスチャー

	float collisionRadius;
	bool isDestroy;

public:
	Thorn();
	~Thorn();
	static void Load();
	static void UnLoad();
	void Generate(const Vector3& pos, const Vector3& scale, const Vector3& rot = { 0,0,0 });
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisDestroy(const bool& isDestroy) { this->isDestroy = isDestroy; }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisDestroy() { return isDestroy; }

};

