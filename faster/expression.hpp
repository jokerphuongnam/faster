#pragma once
#ifndef expression_hpp
#define expression_hpp

#include "variable.hpp"
#include "types.hpp"

#include <string>

namespace faster {
	class runtime_context;
	class tokens_iterator;
	class compiler_context;

	template <typename R>
	class expression {
		expression(const expression&) = delete;
		void operator=(const expression&) = delete;
	protected:
		expression() = default;
	public:
		using ptr = std::unique_ptr<const expression>;

		virtual R evaluate(runtime_context& context) const = 0;
		virtual ~expression() = default;
	};

	expression<void>::ptr build_void_expression(compiler_context& context, tokens_iterator& it);

	expression<byte>::ptr build_byte_expression(compiler_context& context, tokens_iterator& it);
	expression<boolean>::ptr build_boolean_expression(compiler_context& context, tokens_iterator& it);
	expression<int16>::ptr build_int16_expression(compiler_context& context, tokens_iterator& it);
	expression<int32>::ptr build_int32_expression(compiler_context& context, tokens_iterator& it);
	expression<int64>::ptr build_int64_expression(compiler_context& context, tokens_iterator& it);
	expression<uint16>::ptr build_uint16_expression(compiler_context& context, tokens_iterator& it);
	expression<uint32>::ptr build_uint32_expression(compiler_context& context, tokens_iterator& it);
	expression<uint64>::ptr build_uint64_expression(compiler_context& context, tokens_iterator& it);
	expression<double32>::ptr build_double32_expression(compiler_context& context, tokens_iterator& it);
	expression<double64>::ptr build_double64_expression(compiler_context& context, tokens_iterator& it);
	expression<character>::ptr build_character_expression(compiler_context& context, tokens_iterator& it);

	expression<optional>::ptr build_optional_expression(
		compiler_context& context,
		tokens_iterator& it,
		type_handle type_id,
		bool allow_comma);

	expression<lvalue>::ptr build_initialization_expression(
		compiler_context& context,
		tokens_iterator& it,
		type_handle type_id,
		bool allow_comma
	);

	expression<cvalue>::ptr build_initialization_copy_expression(
		compiler_context& context,
		tokens_iterator& it,
		type_handle type_id,
		bool allow_comma
	);
	expression<lvalue>::ptr build_default_initialization(type_handle type_id);
}
#endif