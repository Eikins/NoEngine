#pragma once

#include <cstdint>
#include <stdexcept>

namespace Utils
{
	template<typename T, uint32_t ElementCount>
	class PackedHeap
	{
	private:
		T _elements[ElementCount];
		uint32_t _elementCount;
	public:
		PackedHeap()
		{
			_elementCount = 0;
		}

		~PackedHeap() {}

		inline uint32_t Count() const
		{
			return _elementCount;
		}

		T& Allocate()
		{
			if (_elementCount >= ElementCount) throw std::runtime_error("Fixed Heap Overflow");
			return _elements[_elementCount++];
		}

		void Free(T* element)
		{
			_elementCount--;
			std::ptrdiff_t index = element - _elements;

			for (uint32_t i = index; i < _elementCount; i++)
			{
				_elements[i] = _elements[i + 1];
			}
		}

		T& operator[](int i)
		{
			if (_elementCount >= ElementCount) throw std::runtime_error("Fixed Heap Index out of Bounds");
			return _elements[i];
		}
	};
}
