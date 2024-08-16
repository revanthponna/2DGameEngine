#pragma once

#include "Input.hpp"
#include <vector>

class Physics {

public:

static	Moveable*   AddPhysicsObject	(Vector2D pCoord, Vector2D pVelocity, float pMass) noexcept;

static	Moveable*   AddPhysicsObject	(const SmartPtr<GameObject>& pGameObject, float pMass) noexcept;

static	void		Update				(float pDt) noexcept;

static  void		ClearAll			() noexcept;

static  void		RemovePhysicsObject	(Moveable * pMoveable) noexcept;

	static std::vector<Moveable *> sMovableObjects;
};

