#pragma once
#include "WorldTransform.h"
#include "Sprite.h"
#include "Model.h"
#include <memory>

extern Vector2 WorldToScreen(const Vector3& pos, ViewProjection& viewProjection_);

class Particle
{
private:
	std::unique_ptr<WorldTransform> trans;
	Model* model;
	Vector3 vec;
	int activeTimer;
	int maxActiveTimer;
	float speed;
	int spriteType;

private:
	// スプライト関連
	std::unique_ptr<Sprite> sprite;
	Vector2 size;
	Vector4 color;
	float rotAngle;

public:
	static Model* breakGroundModel;
	static uint32_t texture;
	static uint32_t starTexture;
	static uint32_t healTexture;
	static uint32_t repairTexture;
	static uint32_t windPressureTexture;

public:
	Particle() {};
	Particle(const int& spriteType);
	~Particle();
	static void Load();
	static void UnLoad();
	void Update();
	void UpdateSprite();
	void DrawModel(const int& type = 0);
	void DrawSprite();

public:	// セッター
	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }
	inline void SetRot(const Vector3& rot) { trans->rotation_ = rot; }
	inline void SetVec(const Vector3& vec) { this->vec = vec; }
	inline void SetSpeed(const float& speed) { this->speed = speed; }

	inline void SetSpriteSize(const Vector2& size) { this->size = size; }
	inline void SetSpriteColor(const Vector4& color) { this->color = color; }
	inline void SetSpriteRotAngle(const float& rotAngle) { this->rotAngle = rotAngle; }


public:	// ゲッター
	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline Vector3 GetRot() { return trans->rotation_; }
	inline Vector3 GetVec() { return vec; }
	inline float GetSpeed() { return speed; }
	inline Vector4 GetSpriteColor() { return color; }

	inline Vector2 GetSpriteSize() { return size; }
};
