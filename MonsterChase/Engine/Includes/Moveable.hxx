#pragma once

#include "Moveable.hpp"

inline 
Moveable::Moveable (Vector2D pCoord, Vector2D pPrevCoord, float pMass) noexcept
{
	vGameObject = GameObject::CreateNewGameObject(pCoord, pPrevCoord);
	vMass = pMass;
}

inline 
Moveable::Moveable(const SmartPtr<GameObject> & pGameObject, float pMass) noexcept : vGameObject (pGameObject), vMass(pMass)
{
}

inline Moveable::~Moveable() noexcept
{
	//vGameObject.~SmartPtr ();
}

inline void 
Moveable::UpdateForce (Vector2D pForce) noexcept
{
	vForce = pForce;
}

inline void
Moveable::AddForce (Vector2D pForce) noexcept
{
	vForce += pForce;
}

inline GameObject*
Moveable::GetGameObject() noexcept
{
	return vGameObject.GetData();
}
