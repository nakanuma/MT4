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

// グリッドの描画
void  DrawGrid(const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);
// 球の描画
void DrawSphere(const Sphere& sphere, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color, uint32_t division);
// ワールド座標->スクリーン座標に変換
Vec3 WorldToScreen(const Vec3& worldCoordinate, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);

// 球同士の当たり判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

// Vector3を画面上に表示
void VectorScreenPrintf(int x, int y, const Vec3& vector, const char* label);
// Matrix4x4を画面上に表示
void MatrixScreenPrintf(int x, int y, const Matrix& matrix, const char* label);

// カメラの操作を行う
void CameraControl(Vec3& cameraTranslate, Vec3& cameraRotate, int& prevMousePosX, int& prevMousePosY, bool& isFirstRightClick, bool& isFirstMiddleClick);

