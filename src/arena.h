#ifndef ASTL_ARENA_H
#define ASTL_ARENA_H

#include "bitvector.h"

namespace astl
{
	template<class T, size_t N>
	class StaticArena
	{
		uint8_t data_[N * sizeof(T)];
		StaticBitVector<size_t, N> available_slots_;

	public:
		static const bool is_movable = false;

		StaticArena()
			:available_slots_(N, 1) {};

		template <class ...Args>
		T* create(Args&&... args)
		{
			for (size_t i = 0; i < available_slots_.size(); i++)
			{
				if (available_slots_[i])
				{
					available_slots_[i] = false;
					return new (data_ + i * sizeof(T)) T(std::forward<Args>(args)...);
				}
			}
			return nullptr;
		}

		bool destroy(T* ptr)
		{
			uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(ptr);

			if (byte_ptr < &data_[0] || byte_ptr > &data_[N * sizeof(T) - 1] || (byte_ptr - &data_[0]) % sizeof(T) != 0)
			{
				return false;
			}

			ptr->~T();
			available_slots_[(byte_ptr - &data_[0]) / sizeof(T)] = true;
			return true;
		}
	};


	template<class T>
	class HeapArena
	{
	public:
		static const bool is_movable = true;

		HeapArena() {};

		template <class ...Args>
		T* create(Args&&... args)
		{
			return new T(std::forward<Args>(args)...);
		}

		bool destroy(T* ptr)
		{
			delete ptr;
			return true;
		}
	};

}

#endif