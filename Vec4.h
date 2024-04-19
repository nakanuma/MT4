#pragma once
#include "Matrix.h"

class Vec4
{
public:
	float x;
	float y;
	float z;
	float w;

	// Vec4とMatrixの積
	static Vec4 Multiply(const Vec4& v, const Matrix& m);

	// 長さ(ノルム)
	static float Length(const Vec4& v);
	// 正規化
	static Vec4 Normalize(const Vec4& v);
};

