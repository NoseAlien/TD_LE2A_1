#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

class Thorn
{
private:
	static Model* thornModel;	// ���f��
	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	uint32_t thornTexture = 0;			// �e�N�X�`���[

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

