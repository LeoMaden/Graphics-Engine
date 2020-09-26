#pragma once
#include "Pch.h"

namespace Engine {

	using String = std::string;

	template<typename T>
	using List = std::vector<T>;

	template<typename TKey, typename TValue>
	using Map = std::unordered_map<TKey, TValue>;

	template<typename T1, typename T2>
	using Pair = std::pair<T1, T2>;

}