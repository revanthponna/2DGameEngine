#pragma once

#include "Vector2D.hpp"


class Vector4D {

public:

	inline					Vector4D		() noexcept;

	inline			   		Vector4D		(float pXCoord, float pYCoord, float pZCoord, float pWCoord) noexcept;

	inline	void			SetXCoord		(float pXCoord) noexcept;
	inline	void			SetYCoord		(float pYCoord) noexcept;
	inline	void			SetZCoord		(float pZCoord) noexcept;
	inline	void			SetWCoord		(float pWCoord) noexcept;

	inline	float			GetXCoord		() const noexcept;
	inline	float			GetYCoord		() const noexcept;
	inline	float			GetZCoord		() const noexcept;
	inline	float			GetWCoord		() const noexcept;

	inline float			GetXMagnitude	() const noexcept;
	inline float			GetYMagnitude	() const noexcept;
	inline float			GetZMagnitude	() const noexcept;
	inline float			GetWMagnitude	() const noexcept;

	inline	bool			operator ==		(const Vector4D& pVector) const noexcept;
	inline	bool			operator !=		(const Vector4D& pVector) const noexcept;

	inline	Vector4D		operator +		(const Vector4D& pVector) const noexcept;
	inline	Vector4D		operator -		(const Vector4D& pVector) const noexcept;

	inline	Vector4D		operator *		(float pInt) const noexcept;
	inline	Vector4D		operator /		(float pInt) const noexcept;

	inline	Vector4D		operator -		() const noexcept;

	inline	Vector4D&		operator +=		(const Vector4D& pVector) noexcept;
	inline	Vector4D&		operator -=		(const Vector4D& pVector) noexcept;
	inline	Vector4D&		operator *=		(float pInt) noexcept;
	inline	Vector4D&		operator /=		(float pInt) noexcept;

static		void			Test			() noexcept;

private:

	float uXCoord;
	float uYCoord;
	float uZCoord;
	float uWCoord;
};

#include "Vector4D.hxx"