#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"

class Player
{
private:
	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	Model* playerModel = nullptr;		// ���f��
	uint32_t playerTexture = 0;			// �e�N�X�`���[
	uint32_t redPixel = 0;				// �e�N�X�`���[

	Input* input_ = nullptr;

	bool isAttack;	// �U�����Ă��邩�ǂ���

	bool isWeakAttack;	// ��U��
	float weakAttackMoveSpeed;	// ��U�����̑��x
	bool isHeavyAttack;	// ���U��
	float heavyAttackMoveSpeed;	// ���U�����̑��x

	int pushKeyFream;

public:
	Player();
	~Player();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);
};

