#include "Matrix.hpp"

Matrix Matrix::sIdentityMatrix{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

Matrix
Matrix::operator*(const Matrix& other) noexcept
{
	Matrix Multiplied = sIdentityMatrix;

	Multiplied.m11 = RowXColumn(m11, m12, m13, m14, other.m11, other.m21, other.m31, other.m41);
	Multiplied.m12 = RowXColumn(m11, m12, m13, m14, other.m12, other.m22, other.m32, other.m42);
	Multiplied.m13 = RowXColumn(m11, m12, m13, m14, other.m13, other.m23, other.m33, other.m43);
	Multiplied.m14 = RowXColumn(m11, m12, m13, m14, other.m14, other.m24, other.m34, other.m44);
	Multiplied.m21 = RowXColumn(m21, m22, m23, m24, other.m11, other.m21, other.m31, other.m41);
	Multiplied.m22 = RowXColumn(m21, m22, m23, m24, other.m12, other.m22, other.m32, other.m42);
	Multiplied.m23 = RowXColumn(m21, m22, m23, m24, other.m13, other.m23, other.m33, other.m43);
	Multiplied.m24 = RowXColumn(m21, m22, m23, m24, other.m14, other.m24, other.m34, other.m44);
	Multiplied.m31 = RowXColumn(m31, m32, m33, m34, other.m11, other.m21, other.m31, other.m41);
	Multiplied.m32 = RowXColumn(m31, m32, m33, m34, other.m12, other.m22, other.m32, other.m42);
	Multiplied.m33 = RowXColumn(m31, m32, m33, m34, other.m13, other.m23, other.m33, other.m43);
	Multiplied.m34 = RowXColumn(m31, m32, m33, m34, other.m14, other.m24, other.m34, other.m44);
	Multiplied.m41 = RowXColumn(m41, m42, m43, m44, other.m11, other.m21, other.m31, other.m41);
	Multiplied.m42 = RowXColumn(m41, m42, m43, m44, other.m12, other.m22, other.m32, other.m42);
	Multiplied.m43 = RowXColumn(m41, m42, m43, m44, other.m13, other.m23, other.m33, other.m43);
	Multiplied.m44 = RowXColumn(m41, m42, m43, m44, other.m14, other.m24, other.m34, other.m44);

	return Multiplied;
}

Matrix
Matrix::CreateRotationX(float pXRadians) noexcept
{
	Matrix RotX;

	RotX = sIdentityMatrix;

	RotX.m22 = std::cos(pXRadians);
	RotX.m23 = -(std::sin(pXRadians));
	RotX.m32 = std::sin(pXRadians);
	RotX.m33 = std::cos(pXRadians);

	return RotX;
}

Matrix
Matrix::CreateRotationY(float pYRadians) noexcept
{
	Matrix RotY;

	RotY = sIdentityMatrix;

	RotY.m11 = std::cos(pYRadians);
	RotY.m13 = std::sin(pYRadians);
	RotY.m31 = -std::sin(pYRadians);
	RotY.m33 = std::cos(pYRadians);

	return RotY;
}
Matrix
Matrix::CreateRotationZ(float pZRadians) noexcept
{
	Matrix RotZ;

	RotZ = sIdentityMatrix;

	RotZ.m11 = std::cos(pZRadians);
	RotZ.m12 = -(std::sin(pZRadians));
	RotZ.m21 = std::sin(pZRadians);
	RotZ.m22 = std::cos(pZRadians);

	return RotZ;
}

Matrix
Matrix::CreateTranslation(float pX, float pY, float pZ) noexcept
{
	Matrix Translation;

	Translation = sIdentityMatrix;

	Translation.m14 = pX;
	Translation.m24 = pY;
	Translation.m34 = pZ;

	return Translation;
}
Matrix
Matrix::CreateScale(float pScaleX, float pScaleY, float pScaleZ) noexcept
{
	Matrix Scale;

	Scale = sIdentityMatrix;

	Scale.m11 = pScaleX;
	Scale.m22 = pScaleY;
	Scale.m33 = pScaleZ;

	return Scale;
}

void
Matrix::InvertMatrix() noexcept
{
	*this = GetInvertedMatrix();
}

void
Matrix::TransposeMatrix() noexcept
{
	*this = GetTransposedMatrix ();
}

void
Matrix::Multiply(float pNumber) noexcept
{
	m11 = m11 * pNumber;
	m12 = m12 * pNumber;
	m13 = m13 * pNumber;
	m14 = m14 * pNumber;
	m21 = m21 * pNumber;
	m22 = m22 * pNumber;
	m23 = m23 * pNumber;
	m24 = m24 * pNumber;
	m31 = m31 * pNumber;
	m32 = m32 * pNumber;
	m33 = m33 * pNumber;
	m34 = m34 * pNumber;
	m41 = m41 * pNumber;
	m42 = m42 * pNumber;
	m43 = m43 * pNumber;
	m44 = m44 * pNumber;
}

Vector4D
Matrix::VectorMultiply (const Vector4D & pVector) noexcept
{
	Vector4D result;

	float pX = pVector.GetXCoord();
	float pY = pVector.GetYCoord();
	float pZ = pVector.GetZCoord();
	float pW = pVector.GetWCoord();

	float x = RowXColumn (pX, pY, pZ, pW, m11, m21, m31, m41);
	float y = RowXColumn (pX, pY, pZ, pW, m12, m22, m32, m42);
	float z = RowXColumn (pX, pY, pZ, pW, m13, m23, m33, m43);
	float w = RowXColumn (pX, pY, pZ, pW, m14, m24, m34, m44);

	result = Vector4D (x, y, z, w);

	return result;
}

Matrix
Matrix::GetInvertedMatrix() const noexcept
{
	Matrix invMatrix;
	float  determinent;

	invMatrix = sIdentityMatrix;

	InternalInverseCalc(invMatrix);

	determinent = m11 * invMatrix.m11 + m12 * invMatrix.m21 + m13 * invMatrix.m31 + m14 * invMatrix.m41;

	if (!determinent)
		return *this;

	determinent = 1.0f / determinent;

	invMatrix.Multiply(determinent);

	return invMatrix;
}

Matrix
Matrix::GetTransposedMatrix () const noexcept
{
	Matrix TransposeMatrix = *this;

	std::swap(TransposeMatrix.m12, TransposeMatrix.m21);
	std::swap(TransposeMatrix.m13, TransposeMatrix.m31);
	std::swap(TransposeMatrix.m14, TransposeMatrix.m41);
	std::swap(TransposeMatrix.m23, TransposeMatrix.m32);
	std::swap(TransposeMatrix.m24, TransposeMatrix.m42);
	std::swap(TransposeMatrix.m34, TransposeMatrix.m43);

	return TransposeMatrix;

}

void
Matrix::InternalInverseCalc (Matrix & pMatrix) const noexcept
{
	pMatrix.m11 = (m22 * m33 * m44) -
		          (m22 * m34 * m43) -
				  (m32 * m23 * m44) +
				  (m32 * m24 * m43) +
				  (m42 * m23 * m34) -
				  (m42 * m24 * m33);

	pMatrix.m21 = (-m21 * m33 * m44) +
				  (m21 * m34 * m43) +
				  (m31 * m23 * m44) -
				  (m31 * m24 * m43) -
				  (m41 * m23 * m34) +
				  (m41 * m24 * m33);

	pMatrix.m31 = (m21 * m32 * m44) -
				  (m21 * m34 * m42) -
				  (m31 * m22 * m44) +
				  (m31 * m24 * m42) +
				  (m41 * m22 * m34) -
				  (m41 * m24 * m32);

	pMatrix.m41 = (-m21 * m32 * m43) +
				  (m21 * m33 * m42) +
				  (m31 * m22 * m43) -
				  (m31 * m23 * m42) -
				  (m41 * m22 * m33) +
				  (m41 * m23 * m32);

	pMatrix.m21 = (-m12 * m33 * m44) +
				  (m12 * m34 * m43) +
				  (m32 * m13 * m44) -
				  (m32 * m14 * m43) -
				  (m42 * m13 * m34) +
				  (m42 * m14 * m33);

	pMatrix.m22 = (m11 * m33 * m44) -
				  (m11 * m34 * m43) -
				  (m31 * m13 * m44) +
				  (m31 * m14 * m43) +
				  (m41 * m13 * m34) -
				  (m41 * m14 * m33);

	pMatrix.m32 = (-m11 * m32 * m44) +
			  	(m11 * m34 * m42) +
			  	(m31 * m12 * m44) -
			  	(m31 * m14 * m42) -
			  	(m41 * m12 * m34) +
			  	(m41 * m14 * m32);

	pMatrix.m42 = (m11 * m32 * m43) -
			  	(m11 * m33 * m42) -
			  	(m31 * m12 * m43) +
			  	(m31 * m13 * m42) +
			  	(m41 * m12 * m33) -
			  	(m41 * m13 * m32);
			  
	pMatrix.m13 = (m12 * m23 * m44) -
			  	(m12 * m24 * m43) -
			  	(m22 * m13 * m44) +
			  	(m22 * m14 * m43) +
			  	(m42 * m13 * m24) -
			  	(m42 * m14 * m23);

	pMatrix.m23 = (-m11 * m23 * m44) +
			  	(m11 * m24 * m43) +
			  	(m21 * m13 * m44) -
			  	(m21 * m14 * m43) -
			  	(m41 * m13 * m24) +
			  	(m41 * m14 * m23);

	pMatrix.m33 = (m11 * m22 * m44) -
			  	(m11 * m24 * m42) -
			  	(m21 * m12 * m44) +
			  	(m21 * m14 * m42) +
			  	(m41 * m12 * m24) -
			  	(m41 * m14 * m22);

	pMatrix.m43 = (-m11 * m22 * m43) +
			  	(m11 * m23 * m42) +
			  	(m21 * m12 * m43) -
			  	(m21 * m13 * m42) -
			  	(m41 * m12 * m23) +
			  	(m41 * m13 * m22);

	pMatrix.m14 = (-m12 * m23 * m34) +
			  	(m12 * m24 * m33) +
			  	(m22 * m13 * m34) -
			  	(m22 * m14 * m33) -
			  	(m32 * m13 * m24) +
			  	(m32 * m14 * m23);

	pMatrix.m24 = (m11 * m23 * m34) -
			  	(m11 * m24 * m33) -
			  	(m21 * m13 * m34) +
			  	(m21 * m14 * m33) +
			  	(m31 * m13 * m24) -
			  	(m31 * m14 * m23);

	pMatrix.m34 = (-m11 * m22 * m34) +
			  	(m11 * m24 * m32) +
			  	(m21 * m12 * m34) -
			  	(m21 * m14 * m32) -
			  	(m31 * m12 * m24) +
			  	(m31 * m14 * m22);

	pMatrix.m44 = (m11 * m22 * m33) -
			  	(m11 * m23 * m32) -
			  	(m21 * m12 * m33) +
			  	(m21 * m13 * m32) +
			  	(m31 * m12 * m23) -
			  	(m31 * m13 * m22);
}

float
Matrix::RowXColumn (float pR1, float pR2, float pR3, float pR4, float pC1, float pC2, float pC3, float pC4) const noexcept
{
	float result;

	result = pR1 * pC1 + pR2 * pC2 + pR3 * pC3 + pR4 * pC4;

	return result;
}

// test cases for Vector4D and Matrix

void
Vector4D::Test() noexcept
{
	Vector4D A(0, 1, 0, 0);
	Vector4D B(2, 3, 0, 1);

	// equality
	bool bEqual = A == B;
	assert(bEqual == false);

	bEqual = B == Vector4D(2, 3, 0, 1);
	assert(bEqual == true);

	// Inequality
	bool bNotEqual = A != B;
	assert(bNotEqual == true);

	bNotEqual = B != Vector4D(2, 3, 0, 1);
	assert(bNotEqual == false);

	// Vector4D + Vector4D
	Vector4D C = A + B;
	assert(C == Vector4D(2, 4, 0, 1));

	// Vector4D - Vector4D
	C = B - A;
	assert(C == Vector4D(2, 2, 0, 1));

	// Vector4D * int
	C = A * 2;
	assert(C == Vector4D(0, 2, 0, 0));

	// Vector4D / int
	C = Vector4D(6, 4, 0, 3) / 2;
	assert(C == Vector4D(3, 2, 0, 1.5f));

	// int * Vector4D
	C = 2 * B;
	assert(C == Vector4D(4, 6, 0, 2));

	// negate
	C = -B;
	assert(C == Vector4D(-2, -3, 0, -1));

	// Vector4D += Vector4D
	B += Vector4D(2, 1, 0, 0);
	assert(B == Vector4D(4, 4, 0, 1));

	// Vector4D -= Point2d
	B -= Vector4D(2, 1, 0, 0);
	assert(B == Vector4D(2, 3, 0, 1));

	// Vector4D *= int
	B *= 2;
	assert(B == Vector4D(4, 6, 0, 2));

	// Vector4D /= int
	B /= 2;
	assert(B == Vector4D(2, 3, 0, 1));

}

void
Matrix::Test () noexcept
{
		Matrix mat(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);

// construction

	assert (mat.m11 == 1);
	assert (mat.m12 == 2);
	assert (mat.m13 == 3);
	assert (mat.m14 == 4);

	assert(mat.m21 == 1);
	assert(mat.m22 == 2);
	assert(mat.m23 == 3);
	assert(mat.m24 == 4);

	assert(mat.m31 == 1);
	assert(mat.m32 == 2);
	assert(mat.m33 == 3);
	assert(mat.m34 == 4);

	assert(mat.m41 == 1);
	assert(mat.m42 == 2);
	assert(mat.m43 == 3);
	assert(mat.m44 == 4);

// copy construction

	Matrix mat2(mat);

	assert(mat2.m11 == 1);
	assert(mat2.m12 == 2);
	assert(mat2.m13 == 3);
	assert(mat2.m14 == 4);

	assert(mat2.m21 == 1);
	assert(mat2.m22 == 2);
	assert(mat2.m23 == 3);
	assert(mat2.m24 == 4);

	assert(mat2.m31 == 1);
	assert(mat2.m32 == 2);
	assert(mat2.m33 == 3);
	assert(mat2.m34 == 4);

	assert(mat2.m41 == 1);
	assert(mat2.m42 == 2);
	assert(mat2.m43 == 3);
	assert(mat2.m44 == 4);


// assignment

	Matrix mat3 = mat;

	assert(mat3.m11 == 1);
	assert(mat3.m12 == 2);
	assert(mat3.m13 == 3);
	assert(mat3.m14 == 4);

	assert(mat3.m21 == 1);
	assert(mat3.m22 == 2);
	assert(mat3.m23 == 3);
	assert(mat3.m24 == 4);

	assert(mat3.m31 == 1);
	assert(mat3.m32 == 2);
	assert(mat3.m33 == 3);
	assert(mat3.m34 == 4);

	assert(mat3.m41 == 1);
	assert(mat3.m42 == 2);
	assert(mat3.m43 == 3);
	assert(mat3.m44 == 4);

// multiply

	mat3 = mat3 * Matrix::sIdentityMatrix;

	assert(mat3.m11 == 1);
	assert(mat3.m12 == 2);
	assert(mat3.m13 == 3);
	assert(mat3.m14 == 4);

	assert(mat3.m21 == 1);
	assert(mat3.m22 == 2);
	assert(mat3.m23 == 3);
	assert(mat3.m24 == 4);

	assert(mat3.m31 == 1);
	assert(mat3.m32 == 2);
	assert(mat3.m33 == 3);
	assert(mat3.m34 == 4);

	assert(mat3.m41 == 1);
	assert(mat3.m42 == 2);
	assert(mat3.m43 == 3);
	assert(mat3.m44 == 4);

// scale

	mat3 = Matrix::CreateScale (1, 2, 3);

	assert(mat3.m11 == 1);
	assert(mat3.m12 == 0);
	assert(mat3.m13 == 0);
	assert(mat3.m14 == 0);

	assert(mat3.m21 == 0);
	assert(mat3.m22 == 2);
	assert(mat3.m23 == 0);
	assert(mat3.m24 == 0);

	assert(mat3.m31 == 0);
	assert(mat3.m32 == 0);
	assert(mat3.m33 == 3);
	assert(mat3.m34 == 0);

	assert(mat3.m41 == 0);
	assert(mat3.m42 == 0);
	assert(mat3.m43 == 0);
	assert(mat3.m44 == 1);

// rotation

	mat3 = Matrix::CreateTranslation (1, 2, 3);

	assert(mat3.m11 == 1);
	assert(mat3.m12 == 0);
	assert(mat3.m13 == 0);
	assert(mat3.m14 == 1);

	assert(mat3.m21 == 0);
	assert(mat3.m22 == 1);
	assert(mat3.m23 == 0);
	assert(mat3.m24 == 2);

	assert(mat3.m31 == 0);
	assert(mat3.m32 == 0);
	assert(mat3.m33 == 1);
	assert(mat3.m34 == 3);

	assert(mat3.m41 == 0);
	assert(mat3.m42 == 0);
	assert(mat3.m43 == 0);
	assert(mat3.m44 == 1);

// inverse on identity matrix

	mat3 = sIdentityMatrix;

	mat3.InvertMatrix ();

	// result should also be identity matrix
	assert(mat3.m11 == 1);
	assert(mat3.m12 == 0);
	assert(mat3.m13 == 0);
	assert(mat3.m14 == 0);

	assert(mat3.m21 == 0);
	assert(mat3.m22 == 1);
	assert(mat3.m23 == 0);
	assert(mat3.m24 == 0);

	assert(mat3.m31 == 0);
	assert(mat3.m32 == 0);
	assert(mat3.m33 == 1);
	assert(mat3.m34 == 0);

	assert(mat3.m41 == 0);
	assert(mat3.m42 == 0);
	assert(mat3.m43 == 0);
	assert(mat3.m44 == 1);

// transpose

	Matrix mat4 (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	mat4.TransposeMatrix ();

	assert(mat4.m11 == 1);
	assert(mat4.m12 == 5);
	assert(mat4.m13 == 9);
	assert(mat4.m14 == 13);

	assert(mat4.m21 == 2);
	assert(mat4.m22 == 6);
	assert(mat4.m23 == 10);
	assert(mat4.m24 == 14);

	assert(mat4.m31 == 3);
	assert(mat4.m32 == 7);
	assert(mat4.m33 == 11);
	assert(mat4.m34 == 15);

	assert(mat4.m41 == 4);
	assert(mat4.m42 == 8);
	assert(mat4.m43 == 12);
	assert(mat4.m44 == 16);

//// create rotation on X
//
//		Matrix rot;
//
//	rot = Matrix::CreateRotationX (90);
//
//	assert(rot.m11 == 1);
//	assert(rot.m12 == 0);
//	assert(rot.m13 == 0);
//	assert(rot.m14 == 0);
//
//	assert(rot.m21 == 0);
//	assert(rot.m22 == 0);
//	assert(rot.m23 == -1);
//	assert(rot.m24 == 0);
//
//	assert(rot.m31 == 0);
//	assert(rot.m32 == 1);
//	assert(rot.m33 == 0);
//	assert(rot.m34 == 0);
//
//	assert(rot.m41 == 0);
//	assert(rot.m42 == 0);
//	assert(rot.m43 == 0);
//	assert(rot.m44 == 1);
}
