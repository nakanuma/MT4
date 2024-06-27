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

	// 階層構造の初期値
	Vec3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};
	Vec3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, 0.0f},
	};
	Vec3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};
	float radius = 0.1f;

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


		// ローカル座標系での行列を作成
		Matrix shoulderMatrix = Matrix::MakeAffine(scales[0], rotates[0], translates[0]);
		Matrix elbowMatrix = Matrix::MakeAffine(scales[1], rotates[1], translates[1]);
		Matrix handMatrix = Matrix::MakeAffine(scales[2], rotates[2], translates[2]);

		// それぞれのワールド行列を求める
		Matrix worldShoulderMatrix = shoulderMatrix;
		Matrix worldElbowMatrix = elbowMatrix * worldShoulderMatrix;
		Matrix worldHandMatrix = handMatrix * worldElbowMatrix;

		// それぞれのワールド座標を抽出
		Vec3 shoulderWorldPos = { worldShoulderMatrix.m[3][0], worldShoulderMatrix.m[3][1], worldShoulderMatrix.m[3][2] };
		Vec3 elbowWorldPos = { worldElbowMatrix.m[3][0], worldElbowMatrix.m[3][1], worldElbowMatrix.m[3][2] };
		Vec3 handWorldPos = { worldHandMatrix.m[3][0], worldHandMatrix.m[3][1], worldHandMatrix.m[3][2] };

		// それぞれを球にする
		Sphere shoulder = { shoulderWorldPos,radius };
		Sphere elbow = { elbowWorldPos,radius };
		Sphere hand = { handWorldPos,radius };

		// 肩->肘への線分
		Segment shoulder2elbow = { shoulderWorldPos, elbowWorldPos - shoulderWorldPos }; // 差分を計算
		// 肘->手への線分
		Segment elbow2hand = { elbowWorldPos, handWorldPos - elbowWorldPos }; // 差分を計算

		// ImGui
		ImGui::Begin("Window");

		ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 肩（赤）の描画
		DrawSphere(shoulder, viewProjectionMatrix, viewportMatrix, RED, 20);
		// 肘（緑）の描画
		DrawSphere(elbow, viewProjectionMatrix, viewportMatrix, GREEN, 20);
		// 手（青）の描画
		DrawSphere(hand, viewProjectionMatrix, viewportMatrix, BLUE, 20);

		// 肩->肘への線分を描画
		DrawSegment(shoulder2elbow, viewProjectionMatrix, viewportMatrix);

		// 肘->手への線分を描画
		DrawSegment(elbow2hand, viewProjectionMatrix, viewportMatrix);

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
