#pragma once
#include <math.h>

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 {
public:
	float x; // x成分
	float y; // y成分
	float z; // z成分

public:

	// コンストラクタ
	Vector3();                          // 零ベクトルとする
	Vector3(float x, float y, float z); // x成分, y成分, z成分 を指定しての生成

	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
	Vector3& operator+=(float s)
	{
		x += s;
		y += s;
		z += s;
		return *this;
	}
	Vector3& operator-=(float s)
	{
		x -= s;
		y -= s;
		z -= s;
		return *this;
	}

	Vector3 operator*(float num) const { return { x * num, y * num, z * num }; }
	Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
	Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
	Vector3 operator*(const Vector3& other) const { return { x * other.x, y * other.y, z * other.z }; }
	Vector3 operator/(const Vector3& other) const { return { x / other.x, y / other.y, z / other.z }; }

	float Magnitude() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
	Vector3 Normalized() const { return { x / Magnitude(), y / Magnitude(), z / Magnitude() }; }

	static float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
	static Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return {
			(v1.y * v2.z - v1.z * v2.y),
			(v1.z * v2.x - v1.x * v2.z),
			(v1.x * v2.y - v1.y * v2.x) };
	}
};
