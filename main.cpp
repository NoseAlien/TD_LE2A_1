#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <string>

#pragma comment(lib,"winmm.lib")

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);


#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize();

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();


#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	//1フレームの時間
	const float FRAME_TIME = 1.0f / 60.0f;
	//フレームの経過時間
	float frameTime = 0;
	//計測開始時間
	LARGE_INTEGER timeStart;
	//計測終了時間
	LARGE_INTEGER timeEnd;
	//計測周波数
	LARGE_INTEGER timeFreq;

	//周波数取得
	QueryPerformanceFrequency(&timeFreq);
	//計測開始時間の初期化
	QueryPerformanceCounter(&timeStart);

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		gameScene->Update();
		// 軸表示の更新
		axisIndicator->Update();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		gameScene->Draw();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// 描画終了
		dxCommon->PostDraw();

		// 今の時間を取得
		QueryPerformanceCounter(&timeEnd);
		// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
		frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

		//経過時間が1/60秒未満(処理時間に余裕がある)
		if (frameTime < FRAME_TIME)
		{
			//Sleepの時間を計算
			DWORD sleepTime = static_cast<DWORD>((FRAME_TIME - frameTime) * 1000);
			//分解能を上げる(こうしないとSleepの精度はガタガタ)
			timeBeginPeriod(1);
			//寝る
			Sleep(sleepTime);
			//戻す
			timeEndPeriod(1);
			//計測終了時間を計測開始時間に
			timeStart = timeEnd;

			std::string fps = std::to_string(frameTime) + "\n";
			OutputDebugStringA(fps.c_str());

			//処理を中断しループへ
			continue;
		}
		//計測終了時間を計測開始時間に
		timeStart = timeEnd;
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}