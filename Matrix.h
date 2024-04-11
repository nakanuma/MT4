#pragma once

class Matrix {
public:

	/// <summary>
	/// 4x4行列
	/// </summary>
	
	float m[4][4];

	// 単位行列で初期化
	Matrix();

	// floatを16個で初期化
	Matrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// 加法
	static Matrix Add(const Matrix& m1, const Matrix& m2);
	// 減法
	static Matrix Subtract(const Matrix& m1, const Matrix& m2);
	// 積
	static Matrix Multiply(const Matrix& m1, const Matrix& m2);
	// 逆行列
	static Matrix Inverse(const Matrix& m);
	// 転置行列
	static Matrix Transpose(const Matrix& m);
	// 単位行列の作成
	static Matrix MakeIdentity();
};
