#pragma once

#include <dod/CommonData.h>
#include <dod/DataUtils.h>
#include <concepts>
#include <tuple>

namespace Engine::TableSequence
{

	template <typename Table>
	concept CSequenceTable = requires(Table) {
		requires Dod::CommonData::CImTable<Table> or Dod::CommonData::CMutTable<Table>;
		requires std::is_default_constructible_v<Table>;
	};

	template <CSequenceTable ... Tables>
	struct Sequence
	{
		std::tuple<Tables...> tables;

		Sequence(const Tables& ... tables) noexcept
		{
			this->tables = std::make_tuple(tables...);
		}
	};

	template<CSequenceTable ... Tables, typename TFunc>
	void forEeach(const Sequence<Tables...>& seq, TFunc&& func) noexcept
	{
		std::apply(
			[&](auto&&... elements) {
				(std::invoke(std::forward<TFunc>(func), std::forward<decltype(elements)>(elements)), ...);
			},
			seq.tables
		);
	}

	template<CSequenceTable ... Tables>
	[[nodiscard]] Dod::MemTypes::capacityEls_t getNumOfElements(const Sequence<Tables...>& seq) noexcept
	{
		Dod::MemTypes::capacityEls_t numOfElements{};
		std::apply(
			[&](auto&&... elements) {
				((numOfElements += Dod::DataUtils::getNumFilledElements(elements)), ...);
			},
			seq.tables
		);

		return numOfElements;
	}

};
