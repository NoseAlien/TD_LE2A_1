#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Audio.h"
#include "Input.h"
#include "Vector3.h"

#include "WeakAttackEffect.h"
#include "HeavyAttackEffect.h"
#include "PlayerDieEffect.h"
#include "PlayerMoveEffect.h"
#include "SlowMotion.h"

class Player
{
public:
	static Audio* audio;

private:
	std::unique_ptr<WeakAttackEffect> weakAttackEffect;
	std::unique_ptr<HeavyAttackEffect> heavyAttackEffect;
	std::unique_ptr<PlayerDieEffect> playerDieEffect;
	std::unique_ptr<PlayerMoveEffect> playerMoveEffect;

public:	// �e�N�X�`���[
	static uint32_t playerTexAnime[9];		// �e�N�X�`���[�A�j���V����
	int animeIndex;
	int fream;
	int maxFream;
	static uint32_t redPixel;		// �e�N�X�`���[

private:
	Input* input_ = nullptr;

	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	Model* playerModel = nullptr;		// ���f��
	std::unique_ptr<Model> collapseModel = nullptr;
	std::unique_ptr<WorldTransform> collapseTrans = nullptr;

private:
	// SE�֘A	
	uint32_t jumpSE = 0;
	uint32_t damageSE = 0;

	SlowMotion* slowMotion;

private:
	// ���C�t�֘A
	static uint32_t heartTexture;
	std::vector<Sprite*> heartSprites;
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
	bool isJump;	// �n�ʂ���V��ɖ߂�t���O

private:
	// �U���֘A
	const int starAttackDamage;
	const int weakAttackDamage;
	const int heavyAttackDamage;

	const float addScaleValue = 0.5;
	int addScaleStep = 0;
	float maxSize = 2.5;

	int isJumpAddScaleStep = 0;
	bool isHitGround;

private:
	// �u���b�N�̓��������֘A
	bool isHitBlock;
	int hitedBlockStep;

	bool isAttackBlock;	// �ォ��U��������

private:
	// ���@�̃X�|�[�����f���֘A
	static std::unique_ptr<Model> spawnModel;
	std::unique_ptr<WorldTransform> spawnTrans = nullptr;	// �g�����X�t�H�[��

private:
	void MoveUpdate();
	void AttackUpdate();
	void DamageUpdate();
	void HitBlockUpdate();

public:
	void Load();

	void Init();
	void Update();
	void SelectSceneUpdate();
	void Draw(const ViewProjection& viewProjection_);
	void DrawSpriteBack();
	void DrawSpriteFront();

	void EffectGenerate(const Vector3& pos);
	void EffectUpdate();
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
	inline bool GetisJump() { return isJump; }
	inline int GetisJumpAddScaleStep() { return isJumpAddScaleStep; }
	inline bool GetisHitBlock() { return isHitBlock; }
	inline bool GetisAttackBlock() { return isAttackBlock; }

	inline void HaveStarNumIncriment() { haveStarNum++; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }
	inline void SetisReverse(const bool& isReverse) { this->isReverse = isReverse; }
	inline void SetisEngulfAttack(const bool& isEngulfAttack) { this->isEngulfAttack = isEngulfAttack; }
	inline void SetHaveStarNum(const int& haveStarNum) { this->haveStarNum = haveStarNum; }
	inline void SetStageType(const int& stageType) { this->stageType = stageType; }
	inline void SetLife(const int& life) { this->life = life; }
	inline void SetisHitBlock(const bool& isHitBlock) { this->isHitBlock = isHitBlock; }
	inline void SetSpeed(const float& speed) { this->speed = speed; }
	inline void SetisGround(const bool& isGround) { this->isGround = isGround; }
	inline void SetAttackMoveSpeed(const float& attackMoveSpeed) { this->attackMoveSpeed = attackMoveSpeed; }
	inline void SetisHitGround(const bool& isHitGround) { this->isHitGround = isHitGround; }
	inline void SetisAttackBlock(const bool& isAttackBlock) { this->isAttackBlock = isAttackBlock; }

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