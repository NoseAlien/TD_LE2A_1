#pragma once

#include "Audio.h"
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


private:
	void CurrentStageInit();
};

extern ViewProjection viewProjection_;