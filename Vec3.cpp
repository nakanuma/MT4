#include "Vec3.h"

Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z)
{
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
