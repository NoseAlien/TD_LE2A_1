#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Goal
{
private:
	WorldTransform* trans = nullptr;
	Model* goalModel = nullptr;		// ƒ‚ƒfƒ‹

public:
	Goal();
	~Goal();
	void Generate(const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
};

