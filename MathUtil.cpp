#include "MathUtil.h"

#include <Novice.h>
#include <math.h>

#include "Vec3.h"
#include "Vec4.h"

void DrawGrid(const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ

	float zPositionStart = kGridHalfWidth;
	float zPositionEnd = -kGridHalfWidth;

	float xPositionStart = -kGridHalfWidth;
	float xPositionEnd = kGridHalfWidth;

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float xPosition = -kGridHalfWidth + (xIndex * kGridEvery);
		// ワールド座標系
		Vec3 startPos = { xPosition, 0.0f, zPositionStart };
		Vec3 endPos = { xPosition, 0.0f, zPositionEnd };
		// ワールド座標系->正規化デバイス座標系
		Vec3 ndcStartPos = Vec3::Transform(startPos, viewProjectionMatrix);
		Vec3 ndcEndPos = Vec3::Transform(endPos, viewProjectionMatrix);
		// 正規化デバイス座標系->スクリーン座標系
		Vec3 screenStartPos = Vec3::Transform(ndcStartPos, viewportMatrix);
		Vec3 screenEndPos = Vec3::Transform(ndcEndPos, viewportMatrix);
		if (xIndex == 5) {
			// 描画
			Novice::DrawLine(
				static_cast<int>(screenStartPos.x),
				static_cast<int>(screenStartPos.y),
				static_cast<int>(screenEndPos.x),
				static_cast<int>(screenEndPos.y),
				BLACK
			);
		} else {
			Novice::DrawLine(
				static_cast<int>(screenStartPos.x),
				static_cast<int>(screenStartPos.y),
				static_cast<int>(screenEndPos.x),
				static_cast<int>(screenEndPos.y),
				WHITE
			);
		}
	}
	// 左から右も同じように順々引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float zPosition = kGridHalfWidth - (zIndex * kGridEvery);
		// ワールド座標系
		Vec3 startPos = { xPositionStart, 0.0f, zPosition };
		Vec3 endPos = { xPositionEnd, 0.0f, zPosition };
		// ワールド座標系->正規化デバイス座標系
		Vec3 ndcStartPos = Vec3::Transform(startPos, viewProjectionMatrix);
		Vec3 ndcEndPos = Vec3::Transform(endPos, viewProjectionMatrix);
		// 正規化デバイス座標系->スクリーン座標系
		Vec3 screenStartPos = Vec3::Transform(ndcStartPos, viewportMatrix);
		Vec3 screenEndPos = Vec3::Transform(ndcEndPos, viewportMatrix);
		// 描画
		if (zIndex == 5) {
			Novice::DrawLine(
				static_cast<int>(screenStartPos.x),
				static_cast<int>(screenStartPos.y),
				static_cast<int>(screenEndPos.x),
				static_cast<int>(screenEndPos.y),
				BLACK
			);
		} else {
			Novice::DrawLine(
				static_cast<int>(screenStartPos.x),
				static_cast<int>(screenStartPos.y),
				static_cast<int>(screenEndPos.x),
				static_cast<int>(screenEndPos.y),
				WHITE
			);
		}
	}
}

void DrawSphere(const Sphere& sphere, const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix, uint32_t color, uint32_t division)
{
	const uint32_t kSubdivision = division; // 分割数
	const float kLonEvery = (PIf * 2.0f) / kSubdivision;
	const float kLatEvery = (PIf * 2.0f) / kSubdivision;
	// 緯度の方向に分割 -π/2 = π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -PIf / 2.0f + kLatEvery * latIndex; // 現在の緯度
		// 経度の方向に分割　0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度
			// world座標系でのa,b,cを求める
			Vec3 a, b, c;
			a = { cosf(lat) * cosf(lon) * sphere.radius, sinf(lat) * sphere.radius, cosf(lat) * sinf(lon) * sphere.radius };
			b = { cosf(lat + kLatEvery) * cosf(lon) * sphere.radius, sinf(lat + kLatEvery) * sphere.radius, cosf(lat + kLatEvery) * sinf(lon) * sphere.radius };
			c = { cosf(lat) * cosf(lon + kLonEvery) * sphere.radius, sinf(lat) * sphere.radius, cosf(lat) * sinf(lon + kLonEvery) * sphere.radius };
			a = Vec3::Add(a, sphere.center);
			b = Vec3::Add(b, sphere.center);
			c = Vec3::Add(c, sphere.center);
			// a,b,cをScreen座標系まで変換
			Vec3 screenA = WorldToScreen(a, viewProjectionMatrix, viewportMatrix);
			Vec3 screenB = WorldToScreen(b, viewProjectionMatrix, viewportMatrix);
			Vec3 screenC = WorldToScreen(c, viewProjectionMatrix, viewportMatrix);
			// ab,acで線を引く
			Novice::DrawLine(
				static_cast<int>(screenA.x),
				static_cast<int>(screenA.y),
				static_cast<int>(screenB.x),
				static_cast<int>(screenB.y),
				color
			);
			Novice::DrawLine(
				static_cast<int>(screenA.x),
				static_cast<int>(screenA.y),
				static_cast<int>(screenC.x),
				static_cast<int>(screenC.y),
				color
			);
		}
	}
}

Vec3 WorldToScreen(const Vec3& worldCoordinate,const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix)
{
	// ワールド座標系->正規化デバイス座標系
	Vec3 ndc = Vec3::Transform(worldCoordinate, viewProjectionMatrix);
	// 正規化デバイス座標系->スクリーン座標系
	Vec3 screen = Vec3::Transform(ndc, viewportMatrix);

	return screen;
}

void VectorScreenPrintf(int x, int y, const Vec3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + kRowHeight + (row * kRowHeight), "%6.02f", matrix.m[row][column]
			);
		}
	}
}
