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
	static uint32_t selectFrameTexture;
	std::unique_ptr<Sprite> selectFrameSprite;

	static uint32_t backLightTexture;
	std::vector<std::unique_ptr<BackLight>> backLights;

private:
	void CurrentStageInit();
	void SelectUpdate();
	void BackGroundInit();
	void BackGroundUpdate();
	void BackGroundDraw();
};

extern ViewProjection viewProjection_;