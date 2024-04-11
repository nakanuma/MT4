#include "MyMath.h"

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	);
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	);
}

Vector3 Multiply(float scalar, const Vector3& v)
{
	return Vector3(
		scalar * v.x,
		scalar * v.y,
		scalar * v.z
	);
}

float Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Length(const Vector3& v)
{
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

Vector3 Normalize(const Vector3& v)
{
	float norm = Length(v);

	return Vector3(
		v.x / norm,
		v.y / norm,
		v.z / norm
	);
}
