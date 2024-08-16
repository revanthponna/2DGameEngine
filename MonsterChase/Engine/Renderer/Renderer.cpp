
#include "Renderer.hpp"
#include <DirectXColors.h>

std::vector<Renderable*>  Renderer::sRenderables{};

void
Renderer::AddRenderable(const SmartPtr<GameObject>& pGameObject, const char* pPath) noexcept
{
	Renderable* ren = new Renderable(CreateSprite(pPath), pGameObject);

	sRenderables.push_back(ren);
}

void
Renderer::Update() noexcept
{
	Renderable* ren;
	size_t size;
	size_t ndx = 0;

	size = sRenderables.size();

	// IMPORTANT: Tell GLib that we want to start rendering
	GLib::BeginRendering(DirectX::Colors::Blue);
	// Tell GLib that we want to render some sprites
	GLib::Sprites::BeginRendering();

	// Iterate through all renderables objects
	for (std::vector<Renderable*>::iterator it = sRenderables.begin(); size > ndx; ++it)
	{
		ren = *it;

		GLib::Render(*(ren->uSprite), ren->uGameObject->GetCoordAsPoint2D(), 0.0f, 0.0f);

		++ndx;
	}

	// Tell GLib we're done rendering sprites
	GLib::Sprites::EndRendering();

	// IMPORTANT: Tell GLib we're done rendering
	GLib::EndRendering();

	//GLib::Release(pGoodGuy);
}

void
Renderer::ClearAll () noexcept
{
	size_t size;
	size_t ndx = 0;

	size = sRenderables.size();

	for (std::vector<Renderable*>::iterator it = sRenderables.begin(); size > ndx;) {

		delete (*it);

		sRenderables.erase(it);
		
		++ndx;

		if (size > ndx) {

			++it;
		}
	}

	sRenderables.clear ();
}
