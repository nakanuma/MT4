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


	// 正射影ベクトルと最近傍点の計算が正しいか確認するための値
	Segment segment{ {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
	Vec3 point{ -1.5f, 0.6f, 0.6f };

	/*Vec3 project = Vec3::Project(Vec3::Subtract(point, segment.origin), segment.diff);
	Vec3 closestPoint = Vec3::ClosestPoint(point, segment);*/

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

		// 正射影ベクトルと最近傍点の計算
		Vec3 project = Vec3::Project(Vec3::Subtract(point, segment.origin), segment.diff);
		Vec3 closestPoint = Vec3::ClosestPoint(point, segment);

		// ImGui
		ImGui::Begin("Window");
		ImGui::InputFloat3("Point", &point.x);
		ImGui::InputFloat3("Segment origin", &segment.origin.x);
		ImGui::InputFloat3("Segment diff", &segment.diff.x);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 線分の描画
		Vec3 start = Vec3::Transform(Vec3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vec3 end = Vec3::Transform(Vec3::Transform(Vec3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(
			static_cast<int>(start.x),
			static_cast<int>(start.y),
			static_cast<int>(end.x),
			static_cast<int>(end.y),
			WHITE);

		// 点を描画
		Sphere pointSphere{ point, 0.01f }; // 1cmの球を描画
		Sphere closestPointSphere{ closestPoint, 0.01f };
		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED, 20);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK, 20);

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
