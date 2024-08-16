#pragma once

#include "Timer.hpp"

class Vector2D {

	public:

	inline					Vector2D		() noexcept;

	inline			   		Vector2D		(float pXCoord, float pYCoord) noexcept;

	inline	void			SetXCoord		(float pXCoord) noexcept;
	inline	void			SetYCoord		(float pYCoord) noexcept;

	inline	float			GetXCoord		() const noexcept;
	inline	float			GetYCoord		() const noexcept;

	inline float			GetXMagnitude	() const noexcept;
	inline float			GetYMagnitude   () const noexcept;

	inline	bool			operator ==		(const Vector2D & pVector) const noexcept;	
	inline	bool			operator !=		(const Vector2D & pVector) const noexcept;

	inline	Vector2D		operator +		(const Vector2D & pVector) const noexcept;
	inline	Vector2D		operator -		(const Vector2D & pVector) const noexcept;

	inline	Vector2D		operator *		(float pInt) const noexcept;
	inline	Vector2D		operator /		(float pInt) const noexcept;

	inline	Vector2D		operator -		() const noexcept;

	inline	Vector2D &   	operator +=		(const Vector2D & pVector) noexcept;
	inline	Vector2D &   	operator -=		(const Vector2D & pVector) noexcept;
	inline	Vector2D &   	operator *=		(float pInt) noexcept;
	inline	Vector2D &   	operator /=		(float pInt) noexcept;

	private:

		float uXCoord;
		float uYCoord;
};

#include "Vector2D.hxx"
