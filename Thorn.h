#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Thorn
{
private:
	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	Model* thornModel = nullptr;		// ���f��
	uint32_t thornTexture = 0;			// �e�N�X�`���[

	float collisionRadius;

public:
	Thorn();
	~Thorn();
	void Generate(const Vector3& pos, const Vector3& scale);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& thornTexture);

	Vector3 GetPos() { return trans->translation_; }
	Vector3 GetScale() { return trans->scale_; }
	float GetRadius() { return collisionRadius; }
};

