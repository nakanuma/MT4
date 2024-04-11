#pragma once
#include <math.h>

class Vec3
{
public:
	float x;
	float y;
	float z;

	// 全て0で初期化
	Vec3();
	// float3つで初期化
	Vec3(float x, float y, float z);

	// 加算
	static Vec3 Add(const Vec3& v1, const Vec3& v2);
	// 減算
	static Vec3 Subtract(const Vec3& v1, const Vec3& v2);
	// スカラー倍
	static Vec3 Multiply(float scalar, const Vec3& v);
	// 内積
	static float Dot(const Vec3& v1, const Vec3& v2);
	// 長さ(ノルム)
	static float Length(const Vec3& v);
	// 正規化
	static Vec3 Normalize(const Vec3& v);
};

