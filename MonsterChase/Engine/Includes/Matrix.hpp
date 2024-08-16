#pragma once

#include "Vector4D.hpp"
#include <math.h>

class Matrix
{
public:

		// constructors
									Matrix() = default;

		inline
									Matrix					(float p11, float p12, float p13, float p14,
															 float p21, float p22, float p23, float p24,
															 float p31, float p32, float p33, float p34,
															 float p41, float p42, float p43, float p44) noexcept;

		inline						Matrix					(const Matrix & pMatrix) noexcept;

		// operators

		inline	Matrix&				operator=				(const Matrix & pMatrix) noexcept;
		inline	Matrix				operator*				(const Matrix & pMatrix) noexcept;
		
		// rotation

static          Matrix              CreateRotationX			(float pXRadians) noexcept;
static			Matrix				CreateRotationY			(float pYRadians) noexcept;
static			Matrix				CreateRotationZ			(float pZRadians) noexcept;

		// scale and translation

static			Matrix				CreateScale				(float pScaleX, float pScaleY, float pScaleZ) noexcept;
static			Matrix				CreateTranslation		(float pX, float pY, float pZ) noexcept;

		//
				void				InvertMatrix			() noexcept;	
				void				TransposeMatrix			() noexcept;


		//
				void				Multiply				(float pNumber) noexcept;
				Vector4D			VectorMultiply          (const Vector4D & pVector) noexcept;

		// const functions

				Matrix				GetInvertedMatrix		() const noexcept;
	            Matrix              GetTransposedMatrix		() const noexcept;

		// test case

static			void				Test					() noexcept;

private:

	// internal helper functions.

				float				RowXColumn		        (float pR1, float pR2, float pR3, float pR4, float pC1, float pC2, float pC3, float pC4) const noexcept;
				void				InternalInverseCalc     (Matrix & pMatrix) const noexcept;

private:

  float m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44;

 static Matrix sIdentityMatrix;

};

// inline functions

 
Matrix::Matrix (float p11, float p12, float p13, float p14,
		        float p21, float p22, float p23, float p24,
		        float p31, float p32, float p33, float p34,
		        float p41, float p42, float p43, float p44) noexcept
{
	m11 = p11;
	m12 = p12;
	m13 = p13;
	m14 = p14;
	m21 = p21;
	m22 = p22;
	m23 = p23;
	m24 = p24;
	m31 = p31;
	m32 = p32;
	m33 = p33;
	m34 = p34;
	m41 = p41;
	m42 = p42;
	m43 = p43;
	m44 = p44;
}


Matrix::Matrix (const Matrix & pMatrix) noexcept
{
	m11 = pMatrix.m11;
	m12 = pMatrix.m12;
	m13 = pMatrix.m13;
	m14 = pMatrix.m14;
	m21 = pMatrix.m21;
	m22 = pMatrix.m22;
	m23 = pMatrix.m23;
	m24 = pMatrix.m24;
	m31 = pMatrix.m31;
	m32 = pMatrix.m32;
	m33 = pMatrix.m33;
	m34 = pMatrix.m34;
	m41 = pMatrix.m41;
	m42 = pMatrix.m42;
	m43 = pMatrix.m43;
	m44 = pMatrix.m44;
}

Matrix&
Matrix::operator=(const Matrix& pMatrix) noexcept
{ 
	m11 = pMatrix.m11;
	m12 = pMatrix.m12;
	m13 = pMatrix.m13;
	m14 = pMatrix.m14;
	m21 = pMatrix.m21;
	m22 = pMatrix.m22;
	m23 = pMatrix.m23;
	m24 = pMatrix.m24;
	m31 = pMatrix.m31;
	m32 = pMatrix.m32;
	m33 = pMatrix.m33;
	m34 = pMatrix.m34;
	m41 = pMatrix.m41;
	m42 = pMatrix.m42;
	m43 = pMatrix.m43;
	m44 = pMatrix.m44;
	return *this;

}