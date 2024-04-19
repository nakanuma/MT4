#include "Vec4.h"

Vec4 Vec4::Multiply(const Vec4& v, const Matrix& m)
{
    Vec4 result;

    result.x = v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3];
    result.y = v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3];
    result.z = v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3];
    result.w = v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3];

    return result;
}

float Vec4::Length(const Vec4& v)
{
    return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2) + powf(v.w, 2));
}

Vec4 Vec4::Normalize(const Vec4& v)
{
    float norm = Length(v);

    return Vec4(
        v.x / norm,
        v.y / norm,
        v.z / norm,
        v.w / norm
    );
}
