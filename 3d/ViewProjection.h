#pragma once

#include "MathUtility.h"
#include "Random.h"
#include "SlowMotion.h"
#include "Collision.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4 view;       // ワールド → ビュー変換行列
	Matrix4 projection; // ビュー → プロジェクション変換行列
	Vector3 cameraPos;  // カメラ座標（ワールド座標）
};

/// <summary>
/// ビュープロジェクション変換データ
/// </summary>
struct ViewProjection {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;

#pragma region ビュー行列の設定
	// 視点座標
	Vector3 eye = { 0, 0, -50.0f };
	// 注視点座標
	Vector3 target = { 0, 0, 0 };
	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };
#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = 45.0f * MathUtility::PI / 180.0f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;
#pragma endregion

	// ビュー行列
	Matrix4 matView;
	// 射影行列
	Matrix4 matProjection;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();


private:
	Vector3 prevPos = eye;
	Vector3 prevTarget = target;
	Vector3 shakeVector = { 0,0,0 };
	float shakeValue = 0;
	int shakeTimer = 0;
	int shakeMaxTimer = 0;
	int shakePerFrame = 0;

public:

	inline void SetShakeValue(const float& shakeValue, const int& shakeTimer, const int& shakePerFrame = 1)
	{
		prevPos = eye;
		prevTarget = target;
		this->shakeValue = shakeValue;
		this->shakeTimer = max(shakeTimer, 1);
		this->shakeMaxTimer = max(shakeTimer,1);
		this->shakePerFrame = max(shakePerFrame, 1);
	}

	void ShakeUpdate()
	{
		if (shakeTimer > 0)
		{
			if(shakeTimer % shakePerFrame == 0)
			{
				eye = prevPos;
				target = prevTarget;

				float shakeVecRad = Random::RangeF(0, 2 * 3.1415926);

				shakeVector =
				{
					sin(shakeVecRad),
					cos(shakeVecRad),
					0
				};
				shakeVector *= shakeValue * (shakeTimer / (float)shakeMaxTimer);

				eye += shakeVector;
				target += shakeVector;
			}
			//target += shakeValue;

			shakeTimer--;
			if (shakeTimer <= 0)
			{
				eye = prevPos;
				target = prevTarget;
				//target = prevTarget;
				//isShack = false;
			}
		}
		else
		{
			shakeVector = { 0,0,0 };
		}
	}
};
