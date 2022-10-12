#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"
#include "BreakGroundEffect.h"
#include "Audio.h"
#include <memory>

class Ground
{
public:
	static Audio* audio;

private:
	uint32_t damageSE;
	uint32_t defeatSE;

	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	Model* enemyModel = nullptr;		// ���f��
	uint32_t enemyTexture = 0;			// �e�N�X�`���[
	uint32_t enemyTexture2 = 0;			// �e�N�X�`���[

	bool isAlive;
	int hp;
	int maxhp;
	bool isAddScale;		// �g��̃t���O
	float collisionRadius;	// �����蔻��̔��a
	int isAddScaleCountDown;
	int addScaleCount;
	float addScale;
	float preScaleY;

	int maxRecoveryTimer;	// �ő�l

	bool isDanger;	// �������ɂȂ��Ă��邩
	bool isSuctionStar;	// �����z������t���O
	int isHit;

private:
	// �G�t�F�N�g�֘A
	std::unique_ptr<BreakGroundEffect> breakGroundEffect;
	bool isGeneEffect;


public:
	void Load();
	void Init(const int& maxhp);
	void Update();
	void Draw(const ViewProjection& viewProjection_);
	void EffectUpdate();
	void EffectDraw();

	inline void Damage(const int subhp)
	{
		hp -= subhp;
		audio->PlayWave(damageSE);
	}

	inline Vector3 GetPos() { return trans->translation_; }
	inline Vector3 GetScale() { return trans->scale_; }
	inline float GetRadius() { return collisionRadius; }
	inline int GetHP() { return hp; }
	inline int GetAddScaleCount() { return addScaleCount; }
	inline int GetisAddScaleCountDown() { return isAddScaleCountDown; }
	inline bool GetisAddScale() { return isAddScale; }
	inline bool GetisSuctionStar() { return isSuctionStar; }
	inline int GetisHit() { return isHit; }

	inline void SetPos(const Vector3& pos) { trans->translation_ = pos; }
	inline void SetisDanger(const bool& isDanger) { this->isDanger = isDanger; }
	inline void SetisAddScale(const bool& isAddScale) { this->isAddScale = isAddScale; }
	inline void SetisAddScaleCountDown(const int& isAddScaleCountDown) { this->isAddScaleCountDown = isAddScaleCountDown; }
	inline void SetAddScaleCount(const int& addScaleCount) { this->addScaleCount = addScaleCount; }
	inline void SetisSuctionStar(const bool& isSuctionStar) { this->isSuctionStar = isSuctionStar; }
	inline void SetisHit(const int& isHit) { this->isHit = isHit; }

public:
	static Ground* GetInstance();
	static void DestroyInstance();
private:
	Ground();
	~Ground();

	Ground(const Ground&) = delete;
	Ground& operator=(const Ground&) = delete;
	Ground(const Ground&&) = delete;
	Ground& operator=(const Ground&&) = delete;
};

extern Ground* ground;