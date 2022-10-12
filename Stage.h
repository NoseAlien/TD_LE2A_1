#pragma once
#include "Ground.h"
#include "Star.h"
#include "Thorn.h"
#include "Block.h"
#include "Cannon.h"
#include "Goal.h"
#include "Sprite.h"
#include <vector>
#include <memory>

enum StageType
{
	BaseStage,
	RaceStage,
	CannonStage,
};

enum StageProgress
{
	Start,
	Play,
	Staging,
	End,

};

class Stage
{
private:
	// �J�n�J�E���g�֘A
	static std::vector<uint32_t> startTextTextures;
	Sprite* startTextSprites[4];
	int startTextIndex;
	int startTextTimer;
	int startTextMaxTimer;
	float startTextExrate;
	float startTextAngle;
	float startTextAlpha;
	bool isStartTextEnd;

private:
	// �N���A���Ԋ֘A
	DWORD startTime;
	DWORD endTime;
	DWORD clearTime;

private:
	std::list<std::unique_ptr<Star>> stars;
	std::list<std::unique_ptr<Thorn>> thorns;
	std::list<std::unique_ptr<Block>> blocks;
	std::list<std::unique_ptr<Cannon>> cannons;
	std::unique_ptr<Goal> goal;
	static uint32_t starTexture;			// ���e�N�X�`���[
	static uint32_t thornTexture;		// �g�Q�e�N�X�`���[

	Vector3 linePos1;
	Vector3 linePos2;

	bool gameClear;
	bool gameOver;
	bool playerIsHitGoal;

	int stagePcrogress;
	int stageType;

private:
	void PlayerGenerateStar(const Vector3 pos);

	void CountDownUpdate();
	void PlayerUpdate();
	void FloorUpdate();
	void StarUpdate();
	void ThornUpdate();
	void BlockUpdate();
	void CannonUpdate();
	void RaceUpdate();

public:
	Stage(const int& stageType);
	~Stage();
	static void Load();
	void Init();
	void Update();
	void Draw();
	void DrawLine();
	void DrawCountDown();

	void GenerateThorn(const Vector3& pos, const Vector3& scale = { 1,1,1 });
	void GenerateBlock(const Vector3& pos, const Vector3& scale = { 1,1,1 });
	void GenerateCannon(const Vector3& pos, const Vector3& rot);
	void GenerateGoal(const Vector3& pos);

	inline int GetStageType() { return stageType; }
};

