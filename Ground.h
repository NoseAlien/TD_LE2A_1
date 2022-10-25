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
	// SE�֘A
	uint32_t damageSE;
	uint32_t largeDamageSE;
	uint32_t defeatSE;
	uint32_t recoverySE;

	static Model* enemyModel;			// ���f��
	WorldTransform* trans = nullptr;	// �g�����X�t�H�[��
	uint32_t enemyTexture = 0;			// �e�N�X�`���[
	uint32_t enemyDangerTexture = 0;	// �e�N�X�`���[

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
	// �\��֘A
	std::unique_ptr<Sprite> faceSprite;

	// �A�C�h�����
	static uint32_t idleTexture;

	// �܂΂����֘A
	static std::vector<uint32_t> blinkTexture;
	int timer;
	int maxTimer;	// �u��������Ԋu
	bool isBlink;
	int blinkAnimeTimer;
	int blinkAnimeMaxTimer;
	int blinkAnimeIndex;

	// �����֘A
	static std::vector<uint32_t> surprisedTexture;
	bool isSurprised;
	int surprisedAnimeTimer;
	int surprisedAnimeMaxTimer;
	int surprisedAnimeIndex;
	int surprisedCount;	// ���ɂ��A�j���V�������񂷉�

	// �����֘A
	static std::vector<uint32_t> cryTexture;
	bool isCry;
	int cryAnimeTimer;
	int cryAnimeMaxTimer;
	int cryAnimeIndex;

	// �Ђѕ\���֘A
	static std::vector<uint32_t> groundCrackTexture;

private:
	// �G�t�F�N�g�֘A
	std::unique_ptr<BreakGroundEffect> breakGroundEffect;
	bool isGeneEffect;

public:
	void Load();
	void Init(const int& maxhp);
	void Update();
	void Draw(const ViewProjection& viewProjection_);
	void DrawSprite();
	void EffectUpdate();
	void EffectDraw();
	void SetThickness(const int& num);

	inline void Damage(const int subhp)
	{
		hp -= subhp;
		isSurprised = true;
		audio->PlayWave(damageSE);
	}

	inline void LargeDamage(const int subhp)
	{
		hp -= subhp;
		isSurprised = true;
		audio->PlayWave(largeDamageSE);
	}

	inline void AddHP(const int& addhp)
	{
		if (isAlive == true)
		{
			hp += addhp;
			trans->scale_.y += 1.2f / 15;
			audio->PlayWave(recoverySE);
		}
	}

	inline Vector3 GetMouthPos()
	{
		return
		{
			trans->translation_.x + 32,
			trans->translation_.y + 6 * trans->scale_.y,
			trans->translation_.z
		};
	}

	inline Vector3 GetPos() { return trans->translation_; }
	//inline Vector3 GetScale() { return trans->scale_; }
	inline Vector3 GetScale() { return { 46,10 * trans->scale_.y,5 }; }
	inline Vector3 GetScaleExrate() { return trans->scale_; }
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
	inline void SetScale(const Vector3& scale) { trans->scale_ = scale; }

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