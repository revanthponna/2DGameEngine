#pragma once

#include "Physics.hpp"
#include "WindowsHelpers.h"

class Renderable {

public:

inline	Renderable(GLib::Sprite* pSprite, const SmartPtr<GameObject>& pGameObject) : uSprite(pSprite), uGameObject(pGameObject) {}

inline  ~Renderable() 
{ 
	GLib::Release(uSprite);
}

	GLib::Sprite *          uSprite;            // sprite for the game object    
	SmartPtr<GameObject>    uGameObject;		// reference to the gameobject
};

class Renderer {


public:

static void	AddRenderable (const SmartPtr<GameObject> & pGameObject, const char * pPath) noexcept;

static void	Update		  () noexcept;

static void ClearAll	  () noexcept;

private:
	static std::vector<Renderable*> sRenderables;
};
