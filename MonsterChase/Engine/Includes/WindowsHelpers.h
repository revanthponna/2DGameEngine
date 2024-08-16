#pragma once

#include <Windows.h>
#include "GameObject.hpp"

void* LoadFile(const char* i_pFilename, size_t& o_sizeFile);
GLib::Sprite* CreateSprite(const char* i_pFilename);

