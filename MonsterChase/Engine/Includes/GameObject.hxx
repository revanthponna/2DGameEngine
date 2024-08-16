
#include "GameObject.hpp"

/*** GameObject ***/

// constructor

SmartPtr<GameObject>
GameObject::CreateNewGameObject() noexcept
{
		GameObject           * gameobj;

	gameobj  = new GameObject ();

	SmartPtr<GameObject> smartptr(gameobj);

	sAllGameObjects.push_back(smartptr);

	return smartptr;
}

SmartPtr<GameObject>
GameObject::CreateNewGameObject(Vector2D pCoord, Vector2D pPrevCoord) noexcept
{
	GameObject* gameobj;

	gameobj = new GameObject(pCoord, pPrevCoord);

	SmartPtr<GameObject> smartptr(gameobj);

	sAllGameObjects.push_back(smartptr);

	return smartptr;
}

GameObject::GameObject () noexcept
{
	Vector2D zero { 0, 0 };

	vCoord     = zero;
	vPrevCoord = zero;
}

// parameterized constructor

GameObject::GameObject (Vector2D pCoord, Vector2D pPrevCoord) noexcept
{
	SetGameObject (pCoord, pPrevCoord);
}

GameObject::~GameObject () noexcept
{

}

void
GameObject::ClearAll() noexcept
{
	size_t size;
	size_t ndx = 0;

	size = sAllGameObjects.size();

	for (std::vector<SmartPtr<GameObject>>::iterator it = sAllGameObjects.begin(); size > ndx;) {

		sAllGameObjects.erase(it);

		++ndx;

		if (size > ndx) {

			++it;
		}
	}

	sAllGameObjects.clear();
}

void
GameObject::SetCoord (Vector2D pCoord) noexcept
{
	vCoord = pCoord;
}

void
GameObject::SetPrevCoord (Vector2D pPrevCoord) noexcept
{
	vPrevCoord = pPrevCoord;
}

void
GameObject::SetGameObject (Vector2D pCoord, Vector2D /*pPrevCoord*/) noexcept
{
	SetCoord (pCoord);
}

Vector2D
GameObject::GetCoord () const noexcept
{
	return vCoord;
}

GLib::Point2D
GameObject::GetCoordAsPoint2D() const noexcept
{
	GLib::Point2D coord;

	coord.x = vCoord.GetXCoord();
	coord.y = vCoord.GetYCoord();

	return coord;
}

Vector2D 
GameObject::GetPrevCoord () const noexcept
{
	return vPrevCoord;
}
