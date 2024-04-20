#pragma once
#include "Matrix.h"

static constexpr float PIf = 3.14159265359f;

struct Sphere {
	Vec3 center; // 中心点
	float radius; // 半径
};

void  DrawGrid(const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);

void DrawSphere(const Sphere& sphere, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color, uint32_t division);

Vec3 WorldToScreen(const Vec3& worldCoordinate, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix);

