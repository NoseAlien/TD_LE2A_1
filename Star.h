#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class Star
{
private:
	WorldTransform* trans = nullptr;
	Model* starModel = nullptr;		// ƒ‚ƒfƒ‹
	uint32_t starTexture = 0;

	Input* input_ = nullptr;

public:
	Star();
	~Star();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);
};

