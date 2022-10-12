#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>
class Particle
{
private:
	std::unique_ptr<WorldTransform> trans;
	Model* model;
	Vector3 vec;
	int activeTimer;
	int maxActiveTimer;
	float speed;
private:
	static Model* breakGroundModel;

public:
	Particle();
	~Particle();
	static void Load();
	static void UnLoad();
	void Update();
	void Draw(const int& type = 0);

public:	// セッター
	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }
	inline void SetRot(const Vector3& rot) { trans->rotation_ = rot; }
	inline void SetVec(const Vector3& vec) { this->vec = vec; }
	inline void SetSpeed(const float& speed) { this->speed = speed; }

public:	// ゲッター
	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline Vector3 GetRot() { return trans->rotation_; }
	inline Vector3 GetVec() { return vec; }
	inline float GetSpeed() { return speed; }
};
