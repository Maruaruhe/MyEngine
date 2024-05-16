#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Vector4.h"

struct Vector3;

struct Matrix3x3 {
	float m[3][3];
};
struct Matrix4x4 {
	float m[4][4];
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct ParticleInfo {
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float liftTime;
	float currentTime;
};

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 Transpose(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

float InverseNum(const Matrix4x4& m, int a, int b, int c, int d, int e, int f, int g, int h);
float InverseNum2(const Matrix4x4& m, int a, int b, int c, int d, int e, int f);

Vector3 Add(Vector3 v1, Vector3 v2);
Vector3 Subtract(Vector3 v1, Vector3 v2);

Vector3 TransformNormal(const Vector3& velocity, const Matrix4x4& matrix);
Vector3 MultiplyVectorMatrix(const Vector3& velocity, const Matrix4x4& matrix);

Vector3 Cross(const Vector3& vector1, const Vector3& vector2);

Vector3 Normalize(const Vector3& v);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

float Length(const Vector3& v);

Vector3 Lerp(const Vector3& start, const Vector3& end, const float t);
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
float Dot(const Quaternion& q1, const Quaternion& q2);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
Matrix4x4 MakeRotateMatrix(const Quaternion& q);