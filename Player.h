#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Vector3.h"

class Player
{
private:
	Input* input_ = nullptr;

	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	Model* playerModel = nullptr;		// ���f��

	uint32_t playerTexture = 0;		// �e�N�X�`���[
	uint32_t redPixel = 0;			// �e�N�X�`���[

	float speed;	// �ړ����x
	bool isBreak;	// �܂��̐����󂷂�

	bool isAttack;	// �U�����Ă��邩�ǂ���
	float attackMoveSpeed;	// �U�����̈ړ����x
	bool isReverse;			// �U�����̔��]�t���O

	bool isWeakAttack;	// ��U��
	bool isHeavyAttack;	// ���U��

	float collisionRadius;	// �����蔻��̔��a

	int pushKeyFream;	// �L�[���������Ƃ��̃t���[��
	bool isHaveStar;	// �X�^�[���������߂���?

	int stopTimer = 0;	// ���ŏ����~�܂�^�C�}�[
	float floorPosY;	// ���̍���

	bool isDamage;
	int damegeTimer;

private:
	void MoveUpdate();
	void AttackUpdate();
	void DamageUpdate();

public:
	void Load();

	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline bool GetisHeavyAttack() { return isHeavyAttack; }
	inline bool GetisHaveStar() { return isHaveStar; }
	inline bool GetisBreak() { return isBreak; }
	inline int GetStopTimer() { return stopTimer; }
	inline float GetFloorPosY() { return floorPosY; }

	inline void SetisReverse(bool isReverse) { this->isReverse = isReverse; }
	inline void SetisHaveStar(bool isHaveStar) { this->isHaveStar = isHaveStar; }
	inline void SetFloorPosY(const float& floorPosY) { this->floorPosY = floorPosY; }
	inline void SetisDamage(const bool& isDamage) { if (damegeTimer == 0) this->isDamage = isDamage; }

public:
	static Player* GetInstance();
	static void DestroyInstance();
private:
	Player();
	~Player();

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(const Player&&) = delete;
	Player& operator=(const Player&&) = delete;

};

extern Player* player;