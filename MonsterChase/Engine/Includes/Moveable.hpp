#pragma once

#include "GameObject.hpp"

class Moveable {

	public:

	inline				Moveable			(Vector2D pCoord, Vector2D pPrevCoord, float pMass) noexcept;

	inline				Moveable            (const SmartPtr<GameObject> & pGameObject, float pMass) noexcept;

	inline				~Moveable           () noexcept;

	inline void			UpdateForce			(Vector2D pForce) noexcept;

	inline void			AddForce			(Vector2D pForce) noexcept;

	inline GameObject * GetGameObject		() noexcept;

	private:

		friend class Physics;

		float                   vMass;			// mass of the gameobject
		Vector2D                vForce;		    // force acting on this gameobject
		SmartPtr<GameObject>    vGameObject;		// reference to the gameobject
};

#include "Moveable.hxx"
