#include <Novice.h>
#include <imgui.h>

// MyClass
#include "Vec3.h"
#include "Matrix.h"
#include "MathUtil.h"

const char kWindowTitle[] = "LE2A_14_ナカヌマ_カツシ_MT4";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// カメラの情報
	Vec3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vec3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	// マウスの前回の位置とクリックのフラグ
	int prevMousePosX = 0, prevMousePosY = 0;
	bool isFirstRightClick = true;
	bool isFirstMiddleClick = true;

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

		// マウスでカメラの操作を行う（右クリック:回転 / 中クリック:移動 / ホイール:前後）
		CameraControl(cameraTranslate, cameraRotate, prevMousePosX, prevMousePosY, isFirstRightClick, isFirstMiddleClick);

		// 各種行列の計算
		Matrix cameraMatrix = Matrix::MakeAffine({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix viewMatrix = Matrix::Inverse(cameraMatrix);
		Matrix projectionMatrix = Matrix::MakePerspectiveFov(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix viewProjectionMatrix = Matrix::Multiply(viewMatrix, projectionMatrix);
		Matrix viewportMatrix = Matrix::MakeViewport(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// ImGui
		/*ImGui::Begin("Window");

		ImGui::End();*/

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 任意軸回転行列を作成して表示
		Vec3 axis = Vec3::Normalize({1.0f, 1.0f, 1.0f});
		float angle = 0.44f;
		Matrix rotateMatrix = Matrix::MakeRotateAxisAngle(axis, angle);
		MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");

		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewportMatrix);

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
