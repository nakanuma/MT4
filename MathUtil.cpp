#include "MathUtil.h"

#include <Novice.h>

#include "Vec3.h"
#include "Vec4.h"

void DrawGrid(const Matrix& viewProjectionMatrix, const Matrix& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float xPosition = -kGridHalfWidth + xIndex * kGridEvery;
		// 上の情報を使ってワールド座標形上の始点と終点を求める
		Vec4 startPoint(xPosition, 0.0f, kGridHalfWidth, 1.0f);
		Vec4 endPoint(xPosition, 0.0f, -kGridHalfWidth, 1.0f);
		// スクリーン座標系まで変換をかける
		Vec4 startScreen = Vec4::Multiply(startPoint, viewProjectionMatrix);
		Vec4 endScreen = Vec4::Multiply(endPoint, viewProjectionMatrix);
		// 正規化デバイス座標系に変換
		startScreen = { startScreen.x / startScreen.w, startScreen.y / startScreen.w, startScreen.z / startScreen.w, startScreen.w / startScreen.w, };
		endScreen = { endScreen.x / endScreen.w, endScreen.y / endScreen.w, endScreen.z / endScreen.w, endScreen.w / endScreen.w, };
		// ViewPort変換をかける
		Vec4 startViewport = Vec4::Multiply(startScreen, viewportMatrix);
		Vec4 endViewport = Vec4::Multiply(endScreen, viewportMatrix);
		// 変換した座標を使って表示
		Novice::DrawLine(
			(int)startViewport.x, (int)startViewport.y,
			(int)endViewport.x, (int)endViewport.y,
			WHITE
		);
	}
	// 左から右も同じように順々引いていく
	/*for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {

	}*/
}
