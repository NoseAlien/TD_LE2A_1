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

public:
	Thorn();
	~Thorn();
	static void Load();
	static void UnLoad();
	void Generate(const Vector3& pos, const Vector3& scale);
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	Vector3 GetPos() { return trans->translation_; }
	Vector3 GetScale() { return trans->scale_; }
	float GetRadius() { return collisionRadius; }
};

