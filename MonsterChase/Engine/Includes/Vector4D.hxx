#pragma once

#include "Vector4D.hpp"

inline
Vector4D::Vector4D(float pXCoord, float pYCoord, float pZCoord, float pWCoord) noexcept : uXCoord{ pXCoord }, uYCoord{ pYCoord }, uZCoord{ pZCoord }, uWCoord {pWCoord}
{
}

inline
Vector4D::Vector4D() noexcept
{
	uXCoord = 0;
	uYCoord = 0;
	uZCoord = 0;
	uWCoord = 0;
}

inline
void
Vector4D::SetXCoord(float pXCoord) noexcept
{
	uXCoord = pXCoord;
}

inline
void
Vector4D::SetYCoord(float pYCoord) noexcept
{
	uYCoord = pYCoord;
}

inline
void
Vector4D::SetZCoord(float pZCoord) noexcept
{
	uZCoord = pZCoord;
}

inline
void
Vector4D::SetWCoord(float pWCoord) noexcept
{
	uWCoord = pWCoord;
}

inline
float
Vector4D::GetXCoord() const noexcept
{
	return uXCoord;
}

inline
float
Vector4D::GetYCoord() const noexcept
{
	return uYCoord;
}

inline
float
Vector4D::GetZCoord() const noexcept
{
	return uZCoord;
}

inline
float
Vector4D::GetWCoord() const noexcept
{
	return uWCoord;
}

inline
float
Vector4D::GetXMagnitude() const noexcept
{
	return ((uXCoord >= 0.0f) ? uXCoord : -uXCoord);
}

inline
float
Vector4D::GetYMagnitude() const noexcept
{
	return ((uYCoord >= 0.0f) ? uYCoord : -uYCoord);
}

inline
float
Vector4D::GetZMagnitude() const noexcept
{
	return ((uZCoord >= 0.0f) ? uZCoord : -uZCoord);
}

inline
float
Vector4D::GetWMagnitude() const noexcept
{
	return ((uWCoord >= 0.0f) ? uWCoord : -uWCoord);
}

inline
bool
Vector4D::operator == (const Vector4D& pVector) const noexcept
{
	return ((uXCoord == pVector.uXCoord) && (uYCoord == pVector.uYCoord) && (uZCoord == pVector.uZCoord) && (uWCoord == pVector.uWCoord));
}

inline
bool
Vector4D::operator != (const Vector4D& pVector) const noexcept
{
	return ((uXCoord != pVector.uXCoord) || (uYCoord != pVector.uYCoord) || (uZCoord != pVector.uZCoord) || (uWCoord != pVector.uWCoord));
}

inline
Vector4D
Vector4D::operator + (const Vector4D& pVector) const noexcept
{
	Vector4D vec;

	vec.uXCoord = uXCoord + pVector.uXCoord;
	vec.uYCoord = uYCoord + pVector.uYCoord;
	vec.uWCoord = uWCoord + pVector.uWCoord;
	vec.uZCoord = uZCoord + pVector.uZCoord;

	return vec;
}

inline
Vector4D
Vector4D::operator - (const Vector4D& pVector) const noexcept
{
	Vector4D vec;

	vec.uXCoord = uXCoord - pVector.uXCoord;
	vec.uYCoord = uYCoord - pVector.uYCoord;
	vec.uWCoord = uWCoord - pVector.uWCoord;
	vec.uZCoord = uZCoord - pVector.uZCoord;

	return vec;
}

inline
Vector4D
Vector4D::operator * (float pInt) const noexcept
{
	Vector4D vec;

	vec.uXCoord = uXCoord * pInt;
	vec.uYCoord = uYCoord * pInt;
	vec.uZCoord = uZCoord * pInt;
	vec.uWCoord = uWCoord * pInt;

	return vec;
}

inline
Vector4D
Vector4D::operator / (float pInt) const noexcept
{
	Vector4D vec;

	vec.uXCoord = uXCoord / pInt;
	vec.uYCoord = uYCoord / pInt;
	vec.uZCoord = uZCoord / pInt;
	vec.uWCoord = uWCoord / pInt;

	return vec;
}

inline
Vector4D
Vector4D::operator - () const noexcept
{
	Vector4D vec;

	vec.uXCoord = -uXCoord;
	vec.uYCoord = -uYCoord;
	vec.uZCoord = -uZCoord;
	vec.uWCoord = -uWCoord;

	return vec;
}

inline
Vector4D&
Vector4D::operator += (const Vector4D& pVector) noexcept
{
	uXCoord += pVector.uXCoord;
	uYCoord += pVector.uYCoord;
	uZCoord += pVector.uZCoord;
	uWCoord += pVector.uWCoord;

	return *this;
}

inline
Vector4D&
Vector4D::operator -= (const Vector4D& pVector) noexcept
{
	uXCoord -= pVector.uXCoord;
	uYCoord -= pVector.uYCoord;
	uZCoord -= pVector.uZCoord;
	uWCoord -= pVector.uWCoord;

	return *this;
}

inline
Vector4D&
Vector4D::operator /= (float pInt) noexcept
{
	uXCoord /= pInt;
	uYCoord /= pInt;
	uZCoord /= pInt;
	uWCoord /= pInt;

	return *this;
}

inline
Vector4D&
Vector4D::operator *= (float pInt) noexcept
{
	uXCoord *= pInt;
	uYCoord *= pInt;
	uZCoord *= pInt;
	uWCoord *= pInt;

	return *this;
}

static inline
Vector4D
operator * (float pInt, const Vector4D& pVector) noexcept
{
	Vector4D vec;

	vec = pVector * pInt;

	return vec;
}

