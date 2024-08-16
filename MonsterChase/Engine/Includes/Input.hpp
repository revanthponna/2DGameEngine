#pragma once

#include "StringUtils.hpp"

namespace Engine {

	namespace Input {

		template <typename RetType = void *, typename InParamType = void *, typename OutParamType = void *>
		using InputCBFunc = RetType(*) (unsigned int, bool, InParamType, OutParamType);

	}

}