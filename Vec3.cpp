#include <assert.h>
#include "Vec3.h"
#include "Matrix.h"

Vec3::Vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vec3::Vec3(float argX, float argY, float argZ)
{
	x = argX;
	y = argY;
	z = argZ;
}

Vec3 Vec3::Add(const Vec3& v1, const Vec3& v2)
{
	return Vec3(
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	);
}

Vec3 Vec3::Subtract(const Vec3& v1, const Vec3& v2)
{
	return Vec3(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	);
}

Vec3 Vec3::Multiply(float scalar, const Vec3& v)
{
	return Vec3(
		scalar * v.x,
		scalar * v.y,
		scalar * v.z
	);
}

float Vec3::Dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vec3::Length(const Vec3& v)
{
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

Vec3 Vec3::Normalize(const Vec3& v)
{
	float norm = Length(v);

	return Vec3(
		v.x / norm,
		v.y / norm,
		v.z / norm
	);
}

Vec3 Vec3::Transform(const Vec3& vector, const Matrix& matrix)
{
	Vec3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vec3 Vec3::Cross(const Vec3& v1, const Vec3& v2)
{
	return Vec3(
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x)
	);
}
