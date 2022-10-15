#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Audio.h"
#include "Input.h"
#include "Vector3.h"

#include "WeakAttackEffect.h"
#include "HeavyAttackEffect.h"
#include "PlayerDieEffect.h"
#include "SlowMotion.h"

class Player
{
public:
	static Audio* audio;

private:
	std::unique_ptr<WeakAttackEffect> weakAttackEffect;
	std::unique_ptr<HeavyAttackEffect> heavyAttackEffect;
	std::unique_ptr<PlayerDieEffect> playerDieEffect;

public:	// �e�N�X�`���[
	static uint32_t playerTexAnime[9];		// �e�N�X�`���[�A�j���V����
	int animeIndex;
	int fream;
	int maxFream;

private:
	Input* input_ = nullptr;

	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	Model* playerModel = nullptr;		// ���f��

	uint32_t jumpSE = 0;
	uint32_t damageSE = 0;

	uint32_t redPixel = 0;			// �e�N�X�`���[

	SlowMotion* slowMotion;

	int life;
	bool isAlive;

	float maxSpeed;	// �ړ����x
	float speed;	// �ړ����x
	float radius;	// ���a
	float bigRadius; // ���߂Ă鎞�̔��a

	bool isAttack;	// �U�����Ă��邩�ǂ���
	float attackMoveSpeed;	// �U�����̈ړ����x
	bool isReverse;			// �U�����̔��]�t���O

	bool isWeakAttack;		// ��U��
	bool isHeavyAttack;		// ���U��
	bool isEngulfAttack;	// �������ݍU��

	float collisionRadius;	// �����蔻��̔��a

	int pushKeyFream;		// �L�[���������Ƃ��̃t���[��
	int maxPushKeyFream;	// �ő�l

	int haveStarNum;	// �X�^�[���������߂���?

	int stopTimer = 0;	// ���ŏ����~�܂�^�C�}�[
	int stageType;

	bool isDamage;
	int damageTimer;
	int maxDamageTimer;
	bool isGround;	// �n�ʂɂ��邩�ǂ����̃t���O

	const int starAttackDamage;
	const int weakAttackDamage;
	const int heavyAttackDamage;

	float addScaleValue = 0.5;
	int addScaleStep = 0;
	float maxSize = 2.5;

public:
	// �p�^�[���Q�̂��邽�߂Ɏg�����
	bool place = false;

private:
	void MoveUpdate();
	void AttackUpdate();
	void DamageUpdate();

public:
	void Load();

	void Init();
	void Update();
	void SelectSceneUpdate();
	void Draw(const ViewProjection& viewProjection_);
	void EffectGenerate(const Vector3& pos);
	void EffectUpdate();
	void EffectDraw();
	void DieEffectGenerate();
	inline bool GetDieEffectisEnd() { return playerDieEffect->GetisEnd(); }
	inline void SetDieEffectisEnd(const bool& isEnd) { playerDieEffect->SetisEnd(isEnd); }

	void SetisDamage(const bool& isDamage);

	inline void UpdateMatrix() { trans->UpdateMatrix(); }

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { /*return { 1,1,1 };*/return trans->scale_; }
	inline float GetSpeed() { return speed; }
	inline float GetRadius() { return radius; }
	inline bool GetisWeakAttack() { return isWeakAttack; }
	inline bool GetisHeavyAttack() { return isHeavyAttack; }
	inline bool GetisEngulfAttack() { return isEngulfAttack; }
	inline int GetHaveStarNum() { return haveStarNum; }
	// �U���̃_���[�W
	inline int GetStarAttackDamage() { return starAttackDamage; }
	inline int GetWeakAttackDamage() { return weakAttackDamage; }
	inline int GetHeavyAttackDamage() { return heavyAttackDamage; }
	inline float GetAttackMoveSpeed() { return attackMoveSpeed; }
	inline bool GetisReverse() { return isReverse && addScaleStep == 2; }
	inline bool GetisReverse2() { return isReverse; }

	inline int GetStopTimer() { return stopTimer; }
	inline int GetLife() { return life; }
	inline bool GetisAlive() { return isAlive; }
	inline bool GetisGround() { return isGround; }

	inline void HaveStarNumIncriment() { haveStarNum++; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }
	inline void SetisReverse(const bool& isReverse) { this->isReverse = isReverse; }
	inline void SetisEngulfAttack(const bool& isEngulfAttack) { this->isEngulfAttack = isEngulfAttack; }
	inline void SetHaveStarNum(const int& haveStarNum) { this->haveStarNum = haveStarNum; }
	inline void SetStageType(const int& stageType) { this->stageType = stageType; }
	inline void SetLife(const int& life) { this->life = life; }

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