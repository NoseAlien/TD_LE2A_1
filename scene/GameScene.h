#pragma once
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "IScene.h"
#include "Stage.h"
#include "StageSelect.h"
#include "HitStop.h"
#include "SlowMotion.h"
#include <memory>
#include <vector>

enum GameState
{
	isTitle,
	isGame,
	isSelect,
};

struct BackLight
{
	std::unique_ptr<Sprite> backLight;
	float alpha = 1;
	bool isReverse = false;

	void Generate(const Vector2& pos, const uint32_t& tex);
	void Update();
	void Draw();
};

struct CircleMove
{
	int timer = 0;
	int maxTimer = 20;
	int moveAngle = 0;
	float lenght = 2;

	CircleMove() :
		timer(0), maxTimer(0), lenght(0), moveAngle(0)
	{};
	CircleMove(const int& maxTimer, const float& lenght) :
		timer(0), maxTimer(maxTimer), lenght(lenght), moveAngle(0)
	{}
	Vector2 Move(const Vector2 pos)
	{
		timer++;
		if (timer == maxTimer / 2)
		{
			moveAngle += 9;
			if (moveAngle > 360)
			{
				moveAngle = 0;
			}

			return { cosf(DegreeToRad(moveAngle)) * lenght + pos.x,pos.y };
		}

		if (timer >= maxTimer)
		{
			moveAngle += 9;
			if (moveAngle > 360)
			{
				moveAngle = 0;
			}

			timer = 0;
			return { pos.x,sinf(DegreeToRad(moveAngle)) * lenght + pos.y };
		}

		return pos;
	}
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene
{
public:
	static Audio* audio;

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	static void Load();
	static void UnLoad();
	IScene* GetNextScene();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	std::vector<std::unique_ptr<Stage>> stages;
	std::unique_ptr<StageSelect> stageSelect;

	int gameState;
	int currentStage;
	bool isSelectEnd = false;

	bool pause = false;

	HitStop* hitstop;
	SlowMotion* slowMotion;

	// 背景関連
	static uint32_t backGroundTexture;
	std::unique_ptr<Sprite> backGroundSprite;

	static uint32_t preesSpaceStrTerxture;
	std::unique_ptr<Sprite> preesSpaceStrSprite;
	Easing preesSpaceStrEase;
	bool preesSpaceStrSubAlpha;
	static uint32_t escStrTerxture;
	std::unique_ptr<Sprite> escStrSprite;

	// 背景の星
	static uint32_t backLightTexture;
	std::vector<std::unique_ptr<BackLight>> backLights;

	// 背景色
	static uint32_t backColorTexture;
	std::unique_ptr<Sprite> backColorSprite;

	// 背景の土星
	static uint32_t saturnTexture;
	std::unique_ptr<Sprite> saturnSprite;
	CircleMove saturnCircleMove;

	// 背景の隕石
	static uint32_t meteoriteTexture;
	std::unique_ptr<Sprite> meteoriteSprite;
	CircleMove meteoriteCircleMove;
	float meteoriteAngle;
	int meteoriteAngleDir;

	// 流れ星
	static uint32_t shootingStarTexture;
	std::unique_ptr<Sprite> shootingStarSprite;
	Vector2 shootingStarPos;
	int resetTimer;
	int resetMaxTimer;
	int shootingStarSubScaleSpeed;

	// セレクト画面関連
	static uint32_t selectFrameTexture;
	std::unique_ptr<Sprite> selectFrameSprite;

	int leverAngle;
	int leverTargetAngle;
	static uint32_t leverTexture1;
	std::unique_ptr<Sprite> leverSprite1;
	static uint32_t leverTexture2;
	std::unique_ptr<Sprite> leverSprite2;

private:
	//タイトル用
	static uint32_t titleLogoTexture;
	std::unique_ptr<Sprite> titleLogoSprite;
	int titleLogoMoveAngle;

private:
	bool isGoToTitle;

public:
	// BGMやSE関連
	static uint32_t bgm;

private:
	void CurrentStageInit();
	void SelectUpdate();
	void BackGroundInit();
	void BackGroundUpdate();
	void BackGroundDraw();

	void TitleInit();
	void TitleUpdate();
	void TitleDraw();

public:
	void SaveData();
	void LoadData();
};

extern ViewProjection viewProjection_;