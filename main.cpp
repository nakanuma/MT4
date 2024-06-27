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

	// deltaTime
	float deltaTime = 1.0f / 60.0f;
	bool isSimulationRunning = false;

	// 円錐振り子の初期値
	ConicalPendulum conicalPendulm;
	conicalPendulm.anchor = { 0.0f, 1.0f, 0.0f };
	conicalPendulm.length = 0.8f;
	conicalPendulm.halfApexAngle = 0.7f;
	conicalPendulm.angle = 0.0f;
	conicalPendulm.angularVelocity = 0.0f;

	float radius = std::sinf(conicalPendulm.halfApexAngle) * conicalPendulm.length;
	float height = std::cosf(conicalPendulm.halfApexAngle) * conicalPendulm.length;

	Ball ball;
	ball.position = {
		ball.position.x = conicalPendulm.anchor.x + std::cosf(conicalPendulm.angle) * radius,
		ball.position.y = conicalPendulm.anchor.y - height,
		ball.position.z = conicalPendulm.anchor.z - std::sinf(conicalPendulm.angle) * radius
	};
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

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

		if (isSimulationRunning) {
			// 角速度の計算
			conicalPendulm.angularVelocity = std::sqrtf(9.8f / (conicalPendulm.length * std::cosf(conicalPendulm.halfApexAngle)));
			conicalPendulm.angle += conicalPendulm.angularVelocity * deltaTime;

			radius = std::sinf(conicalPendulm.halfApexAngle) * conicalPendulm.length;
			height = std::cosf(conicalPendulm.halfApexAngle) * conicalPendulm.length;
			ball.position.x = conicalPendulm.anchor.x + std::cosf(conicalPendulm.angle) * radius;
			ball.position.y = conicalPendulm.anchor.y - height;
			ball.position.z = conicalPendulm.anchor.z - std::sinf(conicalPendulm.angle) * radius;
		}

		// ImGui
		ImGui::Begin("Window");

		if (ImGui::Button("Start")) {
			isSimulationRunning = true;
		}
		ImGui::DragFloat("Length", &conicalPendulm.length, 0.01f);
		ImGui::DragFloat("HalfApexAngle", &conicalPendulm.halfApexAngle, 0.01f);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッドを描画
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		
		// 円錐振り子のアンカーポイントから球の中心への線分を描画
		DrawSegment({ conicalPendulm.anchor, ball.position - conicalPendulm.anchor }, viewProjectionMatrix, viewportMatrix);
		// 球を描画
		DrawSphere({ ball.position, ball.radius }, viewProjectionMatrix, viewportMatrix, WHITE, 20);

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
