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


private:
	// 背景関連
	static Model* backCubeModel;
	std::vector<std::unique_ptr<WorldTransform>> backCubeTrans;
	std::vector<float> backCubeMoveSpeed;
	std::vector<float> backCubeMoveAngle;
	std::vector<int> backCubeAngleSign;
	void GenerateBackCube(const Vector3& pos);
	void BackGroundUpdate();
	void BackGroundDraw();

private:
	void CurrentStageInit();
	void SelectUpdate();
};

extern ViewProjection viewProjection_;