#include "Matrix4x4.h"
#include "MathOperator.h"

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	return{
		m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1], m1.m[0][2] + m2.m[0][2], m1.m[0][3] + m2.m[0][3],
		m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1], m1.m[1][2] + m2.m[1][2], m1.m[1][3] + m2.m[1][3],
		m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1], m1.m[2][2] + m2.m[2][2], m1.m[2][3] + m2.m[2][3],
		m1.m[3][0] + m2.m[3][0], m1.m[3][1] + m2.m[3][1], m1.m[3][2] + m2.m[3][2], m1.m[3][3] + m2.m[3][3]
	};
}
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	return{
		m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1], m1.m[0][2] - m2.m[0][2], m1.m[0][3] - m2.m[0][3],
		m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1], m1.m[1][2] - m2.m[1][2], m1.m[1][3] - m2.m[1][3],
		m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1], m1.m[2][2] - m2.m[2][2], m1.m[2][3] - m2.m[2][3],
		m1.m[3][0] - m2.m[3][0], m1.m[3][1] - m2.m[3][1], m1.m[3][2] - m2.m[3][2], m1.m[3][3] - m2.m[3][3]
	};
}
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 resultMatrix = {};

	resultMatrix.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	resultMatrix.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	resultMatrix.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	resultMatrix.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	resultMatrix.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	resultMatrix.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	resultMatrix.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	resultMatrix.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	resultMatrix.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	resultMatrix.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	resultMatrix.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	resultMatrix.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	resultMatrix.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	resultMatrix.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	resultMatrix.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	resultMatrix.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return resultMatrix;
}
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result{};

	float as =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	float determinantRecp = 1.0f / as;

	// 一行目
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[0][2] = (
		m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) *
		determinantRecp;

	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) *
		determinantRecp;


	// 二行目
	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[1][1] = (
		m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) *
		determinantRecp;

	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) *
		determinantRecp;

	result.m[1][3] = (
		m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) *
		determinantRecp;


	// 三行目
	result.m[2][0] = (
		m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) *
		determinantRecp;

	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) *
		determinantRecp;

	result.m[2][2] = (
		m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) *
		determinantRecp;

	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) *
		determinantRecp;


	// 四行目
	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) *
		determinantRecp;

	result.m[3][1] = (

		m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) *
		determinantRecp;

	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) *
		determinantRecp;

	result.m[3][3] = (

		m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) *
		determinantRecp;

	return result;
}

float InverseNum(const Matrix4x4& m, int a, int b, int c, int d, int e, int f, int g, int h) {
	float resultNum;
	resultNum = m.m[a - 1][b - 1] * m.m[c - 1][d - 1] * m.m[e - 1][f - 1] * m.m[g - 1][h - 1];
	return resultNum;
}
float InverseNum2(const Matrix4x4& m, int a, int b, int c, int d, int e, int f) {
	float resultNum;
	resultNum = m.m[a - 1][b - 1] * m.m[c - 1][d - 1] * m.m[e - 1][f - 1];
	return resultNum;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 resultTranspose = {};

	resultTranspose.m[0][0] = m.m[0][0];
	resultTranspose.m[0][1] = m.m[1][0];
	resultTranspose.m[0][2] = m.m[2][0];
	resultTranspose.m[0][3] = m.m[3][0];

	resultTranspose.m[1][0] = m.m[0][1];
	resultTranspose.m[1][1] = m.m[1][1];
	resultTranspose.m[1][2] = m.m[2][1];
	resultTranspose.m[1][3] = m.m[3][1];

	resultTranspose.m[2][0] = m.m[0][2];
	resultTranspose.m[2][1] = m.m[1][2];
	resultTranspose.m[2][2] = m.m[2][2];
	resultTranspose.m[2][3] = m.m[3][2];

	resultTranspose.m[3][0] = m.m[0][3];
	resultTranspose.m[3][1] = m.m[1][3];
	resultTranspose.m[3][2] = m.m[2][3];
	resultTranspose.m[3][3] = m.m[3][3];

	return resultTranspose;
}
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 resultIdentity = {};
	resultIdentity.m[0][0] = 1.0f;
	resultIdentity.m[0][1] = 0.0f;
	resultIdentity.m[0][2] = 0.0f;
	resultIdentity.m[0][3] = 0.0;

	resultIdentity.m[1][0] = 0.0f;
	resultIdentity.m[1][1] = 1.0f;
	resultIdentity.m[1][2] = 0.0f;
	resultIdentity.m[1][3] = 0.0f;

	resultIdentity.m[2][0] = 0.0f;
	resultIdentity.m[2][1] = 0.0f;
	resultIdentity.m[2][2] = 1.0f;
	resultIdentity.m[2][3] = 0.0f;

	resultIdentity.m[3][0] = 0.0f;
	resultIdentity.m[3][1] = 0.0f;
	resultIdentity.m[3][2] = 0.0f;
	resultIdentity.m[3][3] = 1.0f;

	return resultIdentity;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	return{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translate.x,translate.y,translate.z,1
	};
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	return{
		scale.x, 0, 0,0,
		0, scale.y, 0,0,
		0, 0, scale.z,0,
		0,0,0,1
	};
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	float c = std::cos(radian);
	float s = std::sin(radian);
	return{
		1,0,0,0,
		0,c,s,0,
		0,-s,c,0,
		0,0,0,1
	};
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	float c = std::cos(radian);
	float s = std::sin(radian);
	return{
		c,0,-s,0,
		0,1,0,0,
		s,0,c,0,
		0,0,0,1
	};
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	float c = std::cos(radian);
	float s = std::sin(radian);
	return{
		c,s,0,0,
		-s,c,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate) {
	Matrix4x4 result{};

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	result = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 resultAffine = {};
	resultAffine.m[0][0] = scale.x * rotateXYZMatrix.m[0][0];
	resultAffine.m[0][1] = scale.x * rotateXYZMatrix.m[0][1];
	resultAffine.m[0][2] = scale.x * rotateXYZMatrix.m[0][2];
	resultAffine.m[0][3] = 0;

	resultAffine.m[1][0] = scale.y * rotateXYZMatrix.m[1][0];
	resultAffine.m[1][1] = scale.y * rotateXYZMatrix.m[1][1];
	resultAffine.m[1][2] = scale.y * rotateXYZMatrix.m[1][2];
	resultAffine.m[1][3] = 0;

	resultAffine.m[2][0] = scale.z * rotateXYZMatrix.m[2][0];
	resultAffine.m[2][1] = scale.z * rotateXYZMatrix.m[2][1];
	resultAffine.m[2][2] = scale.z * rotateXYZMatrix.m[2][2];
	resultAffine.m[2][3] = 0;

	resultAffine.m[3][0] = translate.x;
	resultAffine.m[3][1] = translate.y;
	resultAffine.m[3][2] = translate.z;
	resultAffine.m[3][3] = 1;

	return resultAffine;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 resultPerspectiveFov = {};

	resultPerspectiveFov.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2));
	resultPerspectiveFov.m[0][1] = 0;
	resultPerspectiveFov.m[0][2] = 0;
	resultPerspectiveFov.m[0][3] = 0;

	resultPerspectiveFov.m[1][0] = 0;
	resultPerspectiveFov.m[1][1] = (1 / std::tan(fovY / 2));
	resultPerspectiveFov.m[1][2] = 0;
	resultPerspectiveFov.m[1][3] = 0;

	resultPerspectiveFov.m[2][0] = 0;
	resultPerspectiveFov.m[2][1] = 0;
	resultPerspectiveFov.m[2][2] = farClip / (farClip - nearClip);
	resultPerspectiveFov.m[2][3] = 1;

	resultPerspectiveFov.m[3][0] = 0;
	resultPerspectiveFov.m[3][1] = 0;
	resultPerspectiveFov.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	resultPerspectiveFov.m[3][3] = 0;

	return resultPerspectiveFov;
}
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 resultOrthographic = {};

	resultOrthographic.m[0][0] = 2 / (right - left);
	resultOrthographic.m[0][1] = 0;
	resultOrthographic.m[0][2] = 0;
	resultOrthographic.m[0][3] = 0;

	resultOrthographic.m[1][0] = 0;
	resultOrthographic.m[1][1] = 2 / (top - bottom);
	resultOrthographic.m[1][2] = 0;
	resultOrthographic.m[1][3] = 0;

	resultOrthographic.m[2][0] = 0;
	resultOrthographic.m[2][1] = 0;
	resultOrthographic.m[2][2] = 1 / (farClip - nearClip);
	resultOrthographic.m[2][3] = 0;

	resultOrthographic.m[3][0] = (left + right) / (left - right);
	resultOrthographic.m[3][1] = (top + bottom) / (bottom - top);
	resultOrthographic.m[3][2] = nearClip / (nearClip - farClip);
	resultOrthographic.m[3][3] = 1;

	return resultOrthographic;
}
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 resultViewport = {};

	resultViewport.m[0][0] = width / 2;
	resultViewport.m[0][1] = 0;
	resultViewport.m[0][2] = 0;
	resultViewport.m[0][3] = 0;

	resultViewport.m[1][0] = 0;
	resultViewport.m[1][1] = -height / 2;
	resultViewport.m[1][2] = 0;
	resultViewport.m[1][3] = 0;

	resultViewport.m[2][0] = 0;
	resultViewport.m[2][1] = 0;
	resultViewport.m[2][2] = maxDepth - minDepth;
	resultViewport.m[2][3] = 0;

	resultViewport.m[3][0] = left + width / 2;
	resultViewport.m[3][1] = top + height / 2;
	resultViewport.m[3][2] = minDepth;
	resultViewport.m[3][3] = 1;

	return resultViewport;
}

Vector3 Add(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Subtract(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

//Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
//	Vector3 result;
//	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
//		1.0f * matrix.m[3][0];
//	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
//		1.0f * matrix.m[3][1];
//	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
//		1.0f * matrix.m[3][2];
//	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
//		1.0f * matrix.m[3][3];
//	assert(w != 0.0f);
//	result.x /= w;
//	result.y /= w;
//	result.z /= w;
//	return result;
//}

Vector3 TransformNormal(const Vector3& velocity, const Matrix4x4& matrix) {
	Vector3 result{
		velocity.x * matrix.m[0][0] + velocity.y * matrix.m[1][0] + velocity.z * matrix.m[2][0],
		velocity.x * matrix.m[0][1] + velocity.y * matrix.m[1][1] + velocity.z * matrix.m[2][1],
		velocity.x * matrix.m[0][2] + velocity.y * matrix.m[1][2] + velocity.z * matrix.m[2][2] };
	return result;
}

Vector3 MultiplyVectorMatrix(const Vector3& velocity, const Matrix4x4& matrix) {
	Vector3 result{
		velocity.x * matrix.m[0][0] + velocity.y * matrix.m[0][1] + velocity.z * matrix.m[0][2],
		velocity.x * matrix.m[1][0] + velocity.y * matrix.m[1][1] + velocity.z * matrix.m[2][2],
		velocity.x * matrix.m[2][0] + velocity.y * matrix.m[2][1] + velocity.z * matrix.m[2][2] };
	return result;
}

Vector3 Cross(const Vector3& vector1, const Vector3& vector2) {
	Vector3 result;
	result.x = vector1.y * vector2.z - vector1.z * vector2.y;
	result.y = vector1.z * vector2.x - vector1.x * vector2.z;
	result.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return result;
}

Vector3 Normalize(const Vector3& v) {
	Vector3 num = { v.x / Length(v), v.y / Length(v), v.z / Length(v) };
	return num;
}
float Length(const Vector3& v) {
	float num = { sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) };
	return num;
}

Vector3 Lerp(const Vector3& start, const Vector3& end, const float t) {
	return start + t * (end - start);
}
Quartenion Slerp(const Quartenion& q0, const Quartenion& q1, float t) {
	Quartenion q = q0;
	float dot = Dot(q0, q1);
	if (dot < 0.0f) {
		q = -q;
		dot = -dot;
	}
	constexpr float EPSILON = 0.0001f;
	if (dot >= 1.0f - EPSILON) {
		return (1.0f - t) * q + t * q0;
	}
	float theta = std::acos(dot);
	float sinTheta = 1.0f / std::sin(theta);
	float scale1 = std::sin((1.0f - t) * theta) * sinTheta;
	float scale2 = std::sin(t * theta) * sinTheta;
	return (q * scale1) + (q1 * scale2);
}