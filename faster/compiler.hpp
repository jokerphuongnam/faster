#pragma once
#ifndef compiler_hpp
#define compiler_hpp

#include <vector>
#include <functional>
#include <string>

namespace faster {
	class compiler_context;
	class tokens_iterator;
	class runtime_context;

	using function = std::function<void(runtime_context&)>;

	runtime_context compile(
		tokens_iterator& ti,
		const std::vector<std::pair<std::string, function>>& externalFunction,
		std::vector<std::string> publicFunction
	);
}
#endif