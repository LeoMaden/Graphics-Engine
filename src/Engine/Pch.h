#pragma once

// std Types.
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// std Streams.
#include <iostream>
#include <sstream>
#include <fstream>

// Engine common headers.
#include "Log.h"




namespace Engine {

	using String = std::string;

	template<typename T>
	using List = std::vector<T>;

	template<typename TKey, typename TValue>
	using Map = std::unordered_map<TKey, TValue>;

}