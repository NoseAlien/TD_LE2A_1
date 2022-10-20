#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

class Goal
{
private:
	WorldTransform* trans = nullptr;
	static std::unique_ptr<Model> goalModel;		// ƒ‚ƒfƒ‹

public:
	Goal();
	~Goal();
	static void Load();
	static void UnLoad();
	void Generate(const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
};

