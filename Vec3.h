#pragma once
#include <math.h>
#include <cassert>

class Matrix; // 循環参照が起きるので前方宣言
struct Line;
struct Ray;
struct Segment;

class Vec3
{
public:
	float x;
	float y;
	float z;

	// 全て0で初期化
	Vec3();
	// float3つで初期化
	Vec3(float argX, float argY, float argZ);

	Vec3 operator+(const Vec3& other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}
	Vec3 operator-(const Vec3& other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}
	Vec3 operator*(const Vec3& other) const {
		return Vec3(x * other.x, y * other.y, z * other.z);
	}
	Vec3 operator*(float scalar) const {
		return Vec3(x * scalar, y * scalar, z * scalar);
	}
	friend Vec3 operator*(float scalar, const Vec3& vec) {
		return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	Vec3 operator/(float scalar) const {
		if (scalar == 0.0f) {
			assert(0);
		}
		return Vec3(x / scalar, y / scalar, z / scalar);
	}

	Vec3& operator+=(const Vec3& other);

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

	// 座標変換
	static Vec3 Transform(const Vec3& vector, const Matrix& matrix);

	// クロス積
	static Vec3 Cross(const Vec3& v1, const Vec3& v2);

	// 正射影ベクトル
	static Vec3 Project(const Vec3& v1, const Vec3& v2);
	// 最近接点
	static Vec3 ClosestPoint(const Vec3& point, const Segment& segment);
};

// 直線
struct Line {
	Vec3 origin; // 始点
	Vec3 diff; // 終点への差分ベクトル
};

// 半直線
struct Ray {
	Vec3 origin; // 始点
	Vec3 diff; // 終点への差分ベクトル
};

// 線分
struct Segment {
	Vec3 origin; // 始点
	Vec3 diff; // 終点への差分ベクトル
};