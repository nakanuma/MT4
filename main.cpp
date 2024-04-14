#include <Novice.h>
#include "Vec3.h"
#include "Matrix.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vec3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + kRowHeight + (row * kRowHeight), "%6.02f", matrix.m[row][column]
			);
		}
	}
}

const char kWindowTitle[] = "LE2B_19_ナカヌマ_カツシ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// クロス積の確認用
	Vec3 v1 = { 1.2f, -3.9f, 2.5f };
	Vec3 v2 = { 2.8f, 0.4f, -1.3f };
	Vec3 cross = Vec3::Cross(v1, v2);

	// 三角形で使用
	Vec3 rotate{ 0.0f,0.0f,0.0f };
	Vec3 translate{ 0.0f,0.0f,0.0f };
	Vec3 cameraPosition{ 0.0f,0.0f,5.0f };
	const Vec3 kLocalVertices[3] = {
		{0.0f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f}
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// 各種行列の計算
		Matrix worldMatrix = Matrix::MakeAffine({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix cameraMatrix = Matrix::MakeAffine({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix viewMatrix = Matrix::Inverse(cameraMatrix);
		Matrix projectionMatrix = Matrix::MakePerspectiveFov(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix worldViewProjectionMatrix = Matrix::Multiply(worldMatrix, Matrix::Multiply(viewMatrix, projectionMatrix));
		Matrix viewportMatrix = Matrix::MakeViewport(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vec3 screenVertices[3] = {};
		for (uint32_t i = 0; i < 3; i++) {
			Vec3 ndcVertex = Vec3::Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Vec3::Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// クロス積の確認用計算結果を表示
		VectorScreenPrintf(0, 0, cross, "Cross");

		// 三角形を前後左右に移動させる
		if (keys[DIK_W]) {
			translate.z += 0.03f;
		}
		if (keys[DIK_S]) {
			translate.z -= 0.03f;
		}
		if (keys[DIK_A]) {
			translate.x += 0.02f;
		}
		if (keys[DIK_D]) {
			translate.x -= 0.02f;
		}
		// 三角形を回転させる
		rotate.y += 0.03f;

		// 三角形を描画
		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y),
			int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y),
			RED,
			kFillModeSolid
		);

		// 三角形の頂点のスクリーン座標を表示
		/*VectorScreenPrintf(0, kRowHeight, screenVertices[0], "v0");
		VectorScreenPrintf(0, kRowHeight * 2, screenVertices[1], "v1");
		VectorScreenPrintf(0, kRowHeight * 3, screenVertices[2], "v2");*/

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
