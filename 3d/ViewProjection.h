#pragma once

#include "MathUtility.h"
#include "Random.h"
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
	bool isShack = false;
	Vector3 shakeValue = { 0,0,0 };
	float shackValueMin = 0;
	float shackValueMax = 0;
	int shakeTimer = 0;
	int shakeMaxTimer = 0;

public:

	inline void SetShakeValue(const float& shackValueMin, const float& shackValueMax, const int& shakeMaxTimer)
	{
		isShack = true;
		prevPos = eye;
		shakeTimer = 0;
		this->shackValueMin = shackValueMin;
		this->shackValueMax = shackValueMax;
		this->shakeMaxTimer = shakeMaxTimer;
	}

	void ShackUpdate()
	{
		if (isShack == true)
		{
			shakeTimer++;
			if (shakeTimer >= shakeMaxTimer)
			{
				isShack = false;
			}

			shakeValue =
			{
				Random::RangeF(shackValueMin,shackValueMax),
				Random::RangeF(shackValueMin,shackValueMax),
				Random::RangeF(shackValueMin,shackValueMax),
			};

			eye += shakeValue;
		}
		if (isShack == false)
		{
			shakeValue = { 0,0,0 };
			eye = prevPos;
		}
	}
};
