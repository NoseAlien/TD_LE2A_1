#pragma once
#include "Ground.h"
#include "Star.h"
#include "Thorn.h"
#include "Block.h"
#include "Cannon.h"
#include "Goal.h"
#include "Sprite.h"
#include "Vector2.h"
#include "GrainScatterEffect.h"
#include <vector>
#include <memory>

enum StageType
{
	BaseStage,
	RaceStage,
	CannonStage,
	EnduranceStage,
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
	// 開始カウント関連
	static std::vector<uint32_t> startTextTextures;
	Sprite* startTextSprites[4];
	int startTextIndex;
	int startTextTimer;
	int startTextMaxTimer;
	float startTextExrate;
	float startTextAngle;
	float startTextAlpha;
	bool isStartTextEnd;
	Vector2 clearTimeLastDightPos;
	bool isMoveClearTime;

private:
	// クリア時間関連
	DWORD startTime;
	DWORD endTime;
	DWORD clearTime;
	static std::vector<uint32_t> numberSheet;
	Sprite* clearTimeSprites[6];
	static uint32_t timeStrTexture;
	Sprite* timeStrSprite;
	static uint32_t dotStrTexture;
	Sprite* dotStrSprite;
	int clearTimeDights;
	std::vector<int> dightsNumber;
	bool isShowClearTime;
	static uint32_t clearStrTexture;
	Sprite* clearStrSprite;

	int clearScreenClock;

private:
	// 演出関連
	bool isCameraMoveStep;
	Vector3 cameraMoveVec;
	bool isPlayerDieEffectGenerate = false;

private:
	// ライン関連
	static Model* lineModel;
	static uint32_t lineModelTexture;
	std::unique_ptr<WorldTransform> lineTrans;
	std::unique_ptr<WorldTransform> lineTrans2;

private:
	// 耐久戦関連
	bool isEndurance;
	int isGetTime;
	int enduranceTime;
	int enduranceNowTime;
	int enduranceStartTime;
	int enduranceEndTime;
	std::unique_ptr<WorldTransform> enduranceLineTrans;

	Sprite* enduranceTimeSprites[2];

private:
	//エフェクト
	std::unique_ptr<GrainScatterEffect> grainScatterEffect;

private:
	std::list<std::unique_ptr<Star>> stars;
	std::list<std::unique_ptr<Thorn>> thorns;
	std::list<std::unique_ptr<Block>> blocks;
	std::list<std::unique_ptr<Cannon>> cannons;
	std::unique_ptr<Goal> goal;
	static uint32_t thornTexture;		// トゲテクスチャー

	bool gameClear;
	bool gameOver;
	bool playerIsHitGoal;

	int stagePcrogress;
	int stageType;

private:
	void PlayerGenerateStar(const Vector3& pos);
	void CannonGenerateStar(const Vector3& pos, const Vector3& dieVec);
	void BlockGenerateStar(const Vector3& pos, const int& num);

	void CountDownUpdate();
	void ClearTimeUpdate();
	void PlayerUpdate();
	void FloorUpdate();
	void StarUpdate();
	void ThornUpdate();
	void BlockUpdate();
	void CannonUpdate();
	void RaceUpdate();
	void EnduranceUpdate();

	void GameOverCameraUpdate();


public:
	Stage(const int& stageType);
	~Stage();
	static void Load();
	static void UnLoad();
	void Init();
	void Update();
	void Draw();
	void DrawSprite();
	void DrawEffectFront();
	void DrawEffectBack();

	void GenerateThorn(const Vector3& pos, const bool& isReverseVertical, const Vector3& scale = { 0.5,0.5,0.5 });
	void GenerateBlock(const Vector3& pos, const bool& haveStar, const Vector3& scale = { 2,2,2 });
	void GenerateCannon(const Vector3& pos, const Vector3& rot);
	void GenerateGoal(const Vector3& pos);

	inline int GetStageType() { return stageType; }
	inline int GetStageProgress() { return stagePcrogress; }
	inline int GetGameClear() { return gameClear; }
	inline int GetGameOver() { return gameOver; }

	inline void SetisEndurance(const bool& isEndurance) { this->isEndurance = isEndurance; }
};

