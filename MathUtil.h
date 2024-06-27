#pragma once
#include "Matrix.h"

static constexpr float PIf = 3.14159265359f;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 球
struct Sphere {
	Vec3 center; // 中心点
	float radius; // 半径
};

// 平面
struct Plane {
	Vec3 normal; // 法線
	float distance; // 距離
};

// 三角形
struct Triangle {
	Vec3 vertices[3]; // 頂点
};

// AABB
struct AABB {
	Vec3 min;
	Vec3 max;
};

// OBB
struct OBB {
	Vec3 center; // 中心点
	Vec3 orientations[3]; // 座標軸
	Vec3 size; // 座標軸方向の長さの半分
};

// ばね
struct Spring {
	// アンカー。固定された端の位置
	Vec3 anchor;
	float naturalLength; // 自然長
	float stiffness; // 剛性。バネ定数k
	float dampingCoefficient; // 減衰係数
};

// ボール
struct Ball {
	Vec3 position; // ボールの位置
	Vec3 velocity; // ボールの速度
	Vec3 acceleration; // ボールの加速度
	float mass; // ボールの質量
	float radius; // ボールの半径
	unsigned int color; // ボールの色
};

// 振り子
struct Pendulum {
	Vec3 anchor; // アンカーポイント。固定された端の位置
	float length; // 紐の長さ
	float angle; // 現在の角度
	float angularVelocity; // 角速度ω
	float angularAcceleration; // 角加速度
};

// 円錐振り子
struct ConicalPendulum {
	Vec3 anchor; // アンカーポイント。固定された端の位置
	float length; // 紐の長さ
	float halfApexAngle; // 円錐の頂角の半分
	float angle; // 現在の角度
	float angularVelocity; // 角速度ω
};

///
///	描画
/// 

// 線分の描画
void DrawSegment(const Segment& segment, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);
// グリッドの描画
void  DrawGrid(const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);
// 球の描画
void DrawSphere(const Sphere& sphere, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color, uint32_t division);
// 平面の頂点を求める
Vec3 Perpendicular(const Vec3& vector);
// 平面の描画
void DrawPlane(const Plane& plane, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color);
// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color);
// AABBの描画
void DrawAABB(const AABB& aabb, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color);
// OBBの描画
void DrawOBB(const OBB& obb, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color);

// ワールド座標->スクリーン座標に変換
Vec3 WorldToScreen(const Vec3& worldCoordinate, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);

///
///	衝突判定
/// 

// 球同士の衝突判定
bool IsCollision(const Sphere& s1, const Sphere& s2);
// 球と平面の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane);
// 線分と平面の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane);
// 三角形と線分の衝突判定
bool IsCollision(const Triangle& triangle, const Segment& segment);
// AABB同士の衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
// AABBと球の衝突判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);
// AABBと線分の衝突判定
bool IsCollision(const AABB& aabb, const Segment& segment);
// OBBと球の衝突判定
bool IsCollision(const OBB& obb, const Sphere& sphere, Matrix& rotateMatrix);

///
///	便利系
/// 

// Vector3を画面上に表示
void VectorScreenPrintf(int x, int y, const Vec3& vector, const char* label);
// Matrix4x4を画面上に表示
void MatrixScreenPrintf(int x, int y, const Matrix& matrix, const char* label);
// AABBのminとmaxの入れ替えを防止
void PreventionSwtichMinMax(AABB aabb);
// 反射ベクトルを求める
Vec3 Reflect(const Vec3& input, const Vec3& normal);

// カメラの操作を行う
void CameraControl(Vec3& cameraTranslate, Vec3& cameraRotate, int& prevMousePosX, int& prevMousePosY, bool& isFirstRightClick, bool& isFirstMiddleClick);

