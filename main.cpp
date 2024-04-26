#include <Novice.h>
#include <imgui.h>

// MyClass
#include "Vec3.h"
#include "Matrix.h"
#include "MathUtil.h"

const char kWindowTitle[] = "LE2B_19_ナカヌマ_カツシ_MT3";

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

	// 平面の情報
	Plane plane{ {0.0f, 1.0f, 0.0f}, 1.0f };
	// 球の情報
	Sphere sphere{ {0.0f, 0.0f, 0.0f}, 0.5f };
	uint32_t color = WHITE;

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

		//// 平面と球の衝突判定
		//if (IsCollision(sphere, plane)) {
		//	color = RED;
		//} else {
		//	color = WHITE;
		//}

		// ImGui
		ImGui::Begin("Window");
		ImGui::DragFloat3("Sphere Center", &sphere.center.x, 0.1f);
		ImGui::DragFloat("Sphere Radius", &sphere.radius, 0.1f);
		ImGui::DragFloat3("Plane Normal", &plane.normal.x, 0.1f);
		plane.normal = Vec3::Normalize(plane.normal); // 法線を正規化
		ImGui::DragFloat("Plane Distance", &plane.distance, 0.1f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 平面の描画を行う
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);
		// 球の描画を行う
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, color, 20);

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
