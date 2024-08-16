#include <stdint.h>

namespace Engine
{
	namespace Math
	{
		static const uint32_t 	allBitsSet = ~0;
		const float 				NAN = *reinterpret_cast<const float*>(&allBitsSet);
	} // namespace Math
} // namespace Engine