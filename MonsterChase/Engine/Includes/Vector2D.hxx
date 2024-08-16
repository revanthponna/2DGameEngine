#pragma once

#include "Vector2D.hpp"

inline
Vector2D::Vector2D (float pXCoord, float pYCoord) noexcept : uXCoord {pXCoord}, uYCoord {pYCoord}
{
}

inline
Vector2D::Vector2D () noexcept
{
	uXCoord = 0;
	uYCoord = 0;
}

inline 
void
Vector2D::SetXCoord	(float pXCoord) noexcept
{
	uXCoord = pXCoord;
}

inline
void
Vector2D::SetYCoord (float pYCoord) noexcept
{
	uYCoord = pYCoord;
}

inline
float
Vector2D::GetXCoord () const noexcept
{
	return uXCoord;
}

inline
float
Vector2D::GetYCoord () const noexcept
{
	return uYCoord;
}

inline
float
Vector2D::GetXMagnitude() const noexcept
{
	return ((uXCoord >= 0.0f) ? uXCoord : -uXCoord);
}

inline
float
Vector2D::GetYMagnitude() const noexcept
{
	return ((uYCoord >= 0.0f) ? uYCoord : -uYCoord);
}

inline
bool
Vector2D::operator == (const Vector2D & pVector) const noexcept
{
	return ((uXCoord == pVector.uXCoord) && (uYCoord == pVector.uYCoord));
}

inline
bool
Vector2D::operator != (const Vector2D & pVector) const noexcept
{
	return ((uXCoord != pVector.uXCoord) || (uYCoord != pVector.uYCoord));
}

inline
Vector2D 
Vector2D::operator + (const Vector2D & pVector) const noexcept
{
	Vector2D vec;

	vec.uXCoord = uXCoord + pVector.uXCoord;
	vec.uYCoord = uYCoord + pVector.uYCoord;

	return vec;
}

inline
Vector2D 
Vector2D::operator - (const Vector2D & pVector) const noexcept
{
	Vector2D vec;

	vec.uXCoord = uXCoord - pVector.uXCoord;
	vec.uYCoord = uYCoord - pVector.uYCoord;

	return vec;
}

inline
Vector2D 
Vector2D::operator * (float pInt) const noexcept
{
	Vector2D vec;

	vec.uXCoord = uXCoord * pInt;
	vec.uYCoord = uYCoord * pInt;

	return vec;
}

inline
Vector2D 
Vector2D::operator / (float pInt) const noexcept
{
	Vector2D vec;

	vec.uXCoord = uXCoord / pInt;
	vec.uYCoord = uYCoord / pInt;

	return vec;
}

inline
Vector2D
Vector2D::operator - () const noexcept
{
	Vector2D vec;

	vec.uXCoord = -uXCoord;
	vec.uYCoord = -uYCoord;

	return vec;
}

inline
Vector2D &
Vector2D::operator += (const Vector2D & pVector) noexcept
{
	uXCoord += pVector.uXCoord;
	uYCoord += pVector.uYCoord;

	return *this;
}

inline
Vector2D &
Vector2D::operator -= (const Vector2D & pVector) noexcept
{
	uXCoord -= pVector.uXCoord;
	uYCoord -= pVector.uYCoord;

	return *this;
}

inline
Vector2D &
Vector2D::operator /= (float pInt) noexcept
{
	uXCoord /= pInt;
	uYCoord /= pInt;

	return *this;
}

inline
Vector2D &
Vector2D::operator *= (float pInt) noexcept
{
	uXCoord *= pInt;
	uYCoord *= pInt;

	return *this;
}

static inline
Vector2D
operator * (float pInt, const Vector2D & pVector) noexcept
{
	Vector2D vec;

	vec = pVector * pInt;

	return vec;
}

