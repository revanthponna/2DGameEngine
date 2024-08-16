#pragma once

#include "WeakPtr.hpp"
#include <vector>

#pragma once

/*** GameObject ***/

class GameObject {

public:

static inline SmartPtr<GameObject> CreateNewGameObject () noexcept;
static inline SmartPtr<GameObject> CreateNewGameObject (Vector2D pCoord, Vector2D pPrevCoord) noexcept;

	inline						~GameObject			() noexcept;

static inline void					ClearAll			() noexcept;

	inline void					SetCoord			(Vector2D pCoord) noexcept;
	inline void					SetPrevCoord        (Vector2D pPrevCoord) noexcept;
	inline void					SetGameObject		(Vector2D pCoord, Vector2D pPrevCoord) noexcept;

	inline Vector2D				GetCoord			() const noexcept;
	inline GLib::Point2D		GetCoordAsPoint2D   () const noexcept;
	inline Vector2D				GetPrevCoord    	() const noexcept;

	static std::vector<SmartPtr<GameObject>> sAllGameObjects;

private:

	inline						GameObject			() noexcept;
	inline						GameObject			(Vector2D pCoord, Vector2D pVelocity) noexcept;

protected:

	Vector2D		vCoord;			// coordinates of the object.
	Vector2D        vPrevCoord;		// previous coordinates of the object.
};

#include "GameObject.hxx"
