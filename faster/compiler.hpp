#pragma once
#ifndef compiler_hpp
#define compiler_hpp

#include "types.hpp"
#include "tokens.hpp"
#include "statement.hpp"

#include <vector>
#include <functional>

namespace faster {
	enum struct access_modifer {
		am_public,
		am_protected,
		am_internal,
		am_private,
		am_fileprivate,
		am_extensionprivate,
		am_objectprivate
	};

	enum struct current_scope {
		global,
		in_object,
		in_interface,
		in_extension,
		in_function
	};

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