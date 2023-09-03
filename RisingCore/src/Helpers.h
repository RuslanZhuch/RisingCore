#pragma once

namespace RisingCore::Helpers
{

	template <size_t N, size_t Current = 0>
	constexpr void constexprLoop(auto func, auto&& ... args) {
		if constexpr (Current < N) {
			func.operator()<Current>(std::forward<decltype(args)>(args)...);
			constexprLoop<N, Current + 1>(func);
		}
	}

}
