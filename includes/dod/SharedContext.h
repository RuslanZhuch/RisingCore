#pragma once

#include <dod/MemPool.h>

namespace Dod::SharedContext
{

	template<typename Ctx>
	class Controller
	{
	public:
		Controller()
		{
			this->context.load();
		}
	public:
		Ctx context;
		Dod::MemPool memory;
	};

	template <typename Ctx>
	[[nodiscard]] const auto& get(const Controller<Ctx>* controller) noexcept
	{
		return controller->context;
	}

	template <typename Ctx>
	void flush(Controller<Ctx>* controller) noexcept
	{
		controller->context.reset();
	}

	template <typename Ctx>
	void merge(Controller<Ctx>* target, const Ctx& src) noexcept
	{
		target->context.merge(src);
	}

};
