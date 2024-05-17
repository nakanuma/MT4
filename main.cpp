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

	// 三角形の情報
	Triangle triangle = 
	{
		{
			{0.0f, 1.0f, 0.0f},
			{-0.5f, 0.0f, 0.0f},
			{0.5f, 0.0f, 0.0f}
		}
	};
	// 線分の情報
	Segment segment =
	{
		{1.0f, 0.5f, -0.5f},
		{1.0f, 0.5f, 1.0f},
	};
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

		// 三角形と線分の衝突判定
		if (IsCollision(triangle, segment)) {
			color = RED;
		} else {
			color = WHITE;
		}

		// ImGui
		ImGui::Begin("Window");
		ImGui::DragFloat3("Triangle.v0", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("Triangle.v1", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("Triangle.v2", &triangle.vertices[2].x, 0.01f);
		ImGui::DragFloat3("Segment.Origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment.Diff", &segment.diff.x, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 三角形を描画
		DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, WHITE);
		// 線分を描画
		Vec3 start = Vec3::Transform(Vec3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vec3 end = Vec3::Transform(Vec3::Transform(Vec3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(
			static_cast<int>(start.x),
			static_cast<int>(start.y),
			static_cast<int>(end.x),
			static_cast<int>(end.y),
			color);

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
