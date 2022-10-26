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
#include "RepairEffect.h"
#include "BreakBlockEffect.h"
#include "WindPressureEffect.h"
#include "DamageEffect.h"
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

//struct RevivalObject
//{
//	bool isRevival = false;
//	int timer = 0;
//	int maxTimer = 180;
//	Vector3 pos;
//};

class Stage
{
private:
	// SE関連
	static uint32_t gameClearBGM;
	static uint32_t gameOverBGM;
	static uint32_t grainDiedSE;

public:
	// ゲーム開始時のステージ表示
	static std::vector<uint32_t> stageNumberTextures;
private:
	std::unique_ptr<Sprite> stageNumberSprite;
	bool isShowStageNumber;
	float sizeExrate;
	float rotAngel;
	float alpha;

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
	bool isMoveClearTime;

public:
	// クリア時間関連
	static std::vector<uint32_t> numberSheet;
private:
	Vector2 clearTimeLastDightPos;
	DWORD startTime;
	DWORD endTime;
	DWORD clearTime;
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
	static uint32_t overStrTexture;
	std::unique_ptr<Sprite> overStrSprite;

public:
	//データ保存関連
	Vector2 fastTimeLastDightPos;
	DWORD fastClearTime;
	Sprite* fastClearTimeSprites[6];
	static uint32_t fastTimeStrTexture;
	Sprite* fastTimeStrSprite;
	int fastTimeDights;
	std::vector<int> fastTimedightsNumber;
	Sprite* dotStrSprite2;

private:
	int clearScreenClock;
	int overScreenClock;

private:
	// 演出関連
	bool isCameraMoveStep;
	Vector3 cameraMoveVec;
	bool isPlayerDieEffectGenerate = false;
	WindPressureEffect* windPressureEffect;
	DamageEffect* damageEffect;


private:
	// ライン関連
	static Model* lineModel;
	static uint32_t lineModelTexture;
	std::unique_ptr<WorldTransform> lineTrans;
	std::unique_ptr<WorldTransform> lineTrans2;

	std::unique_ptr<WorldTransform> goalLineTrans;

private:
	// 耐久戦関連
	bool isEndurance;
	int isGetTime;
	int enduranceTime;
	int enduranceNowTime;
	int enduranceStartTime;
	int enduranceEndTime;
	std::unique_ptr<WorldTransform> enduranceLineTrans;
	std::vector<int> enduranceTimeDightsNumber;

	Sprite* enduranceTimeSprites[2];

private:
	//エフェクト
	std::unique_ptr<GrainScatterEffect> grainScatterEffect;
	std::unique_ptr<RepairEffect> repairEffect;
	std::unique_ptr<BreakBlockEffect> breakBlockEffect;

private:
	std::list<std::unique_ptr<Star>> stars;
	//std::vector<std::unique_ptr<RevivalObject>> starRevivals;
	uint32_t starsIndex;

	std::list<std::unique_ptr<Thorn>> thorns;

	std::list<std::unique_ptr<Block>> blocks;
	//std::vector<std::unique_ptr<RevivalObject>> blockRevivals;
	//uint32_t blocksIndex;

	std::list<std::unique_ptr<Cannon>> cannons;
	std::unique_ptr<Goal> goal;
	static uint32_t thornTexture;		// トゲテクスチャー

	bool gameClear;
	bool gameOver;
	bool playerIsHitGoal;

	int stagePcrogress;
	int stageType;

private:
	// 床がラインを超えた関連
	bool isOverLine;
	float goundeTempScaleY;
	Easing overLineEase;

private:
	void PlayerGenerateStar(const Vector3& pos);
	void CannonGenerateStar(const Vector3& pos, const Vector3& dieVec, const float& angle);
	void BlockGenerateStar(const Vector3& pos, const int& num);

	void ShowStageNumberUpdate();
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
	void WaveUpdate();

	void GameOverCameraUpdate();
	void GroundOverLineUpdate();

public:
	Stage(const int& stageType, const int& stageNumber);
	~Stage();
	static void Load();
	static void UnLoad();
	void Init();
	void Update();
	void Draw();
	void DrawSprite();
	void DrawEffectFront();
	void DrawEffectBack();

	void GenerateStar(const Vector3& pos);
	void GenerateThorn(const Vector3& pos, const bool& isReverseVertical,
		const Vector3& rot = { 0,0,0 }, const Vector3& scale = { 0.25,0.5,0.25 });
	void GenerateBlock(const Vector3& pos, const bool& haveStar, const Vector3& scale = { 2,2,2 });
	void GenerateCannon(const Vector3& pos, const Vector3& rot);
	void GenerateGoal(const Vector3& pos);

	inline int GetStageType() { return stageType; }
	inline int GetStageProgress() { return stagePcrogress; }
	inline int GetGameClear() { return gameClear; }
	inline int GetGameOver() { return gameOver; }

	inline void SetisEndurance(const bool& isEndurance) { this->isEndurance = isEndurance; }
};

