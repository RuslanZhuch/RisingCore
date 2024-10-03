#pragma once

#include <dod/MemTypes.h>

#include <cstdlib>
#include <span>

namespace Engine::LocalMemory
{

	struct Buffer
	{
		Buffer(size_t numOfBytes)
		{
			this->data = static_cast<Dod::MemTypes::data_t*>(_aligned_malloc(numOfBytes, 64));
			if (this->data != nullptr)
				this->numOfBytes = numOfBytes;
		}

		~Buffer()
		{
			_aligned_free(data);
		}

		Buffer(const Buffer& other) = delete;
		Buffer(Buffer&& other) = delete;

		[[nodiscard]] Buffer& operator=(const Buffer& other) = delete;
		[[nodiscard]] Buffer& operator=(Buffer&& other) = delete;

		Dod::MemTypes::data_t* data{};
		size_t numOfBytes{};
	};

	template <typename TSource>
	void set(const Buffer& memory, const std::span<const TSource> src, size_t offset)
	{
		if (offset > memory.numOfBytes)
			return;

		const auto srcSizeInBytes{ src.size() * sizeof(TSource) };
		const auto offsetedDestSizeInBytes{ memory.numOfBytes - offset };
		const auto commonSizeInBytes{ std::min(srcSizeInBytes, offsetedDestSizeInBytes) };
		std::memcpy(memory.data + offset, src.data(), commonSizeInBytes);
	}

};
