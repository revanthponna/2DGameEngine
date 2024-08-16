#pragma once

#include <Windows.h>

#include <DirectXColors.h>

#include "GLib.h"

#include "Basic.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"
#include "SmartPtr.hpp"
#include "WeakPtr.hpp"
#include "GameObject.hpp"
#include "Moveable.hpp"
#include "DynamicQueue.hpp"
#include "String.hpp"
#include "StringUtils.hpp"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"


// Individual Systems
#include "Input.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "CollisionSystem.hpp"

// Job system
#include <conio.h>
#include <iostream>
#include <functional>
#include <string>
#include <vector>

#include "JobSystem.h"
#include "Events.h"

#include "HashedString.h"

#include "json.hpp"

#include "WindowsHelpers.h"

void	InitializeEngine () noexcept;