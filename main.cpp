#include <Novice.h>
#include <imgui.h>
#include "Vec3.h"
#include "Matrix.h"
#include "MathUtil.h"

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

	Vec3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vec3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Sphere sphere{
		{0.0f, 0.0f, 0.0f},
		1.0f
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
		Matrix cameraMatrix = Matrix::MakeAffine({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix viewMatrix = Matrix::Inverse(cameraMatrix);
		Matrix projectionMatrix = Matrix::MakePerspectiveFov(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix viewProjectionMatrix = Matrix::Multiply(viewMatrix, projectionMatrix);
		Matrix viewportMatrix = Matrix::MakeViewport(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// グリッドを表示
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// 球体を表示
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, BLACK, 20);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
