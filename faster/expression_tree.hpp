#pragma once
#ifndef expression_tree_hpp
#define expression_tree_hpp
#include <memory>
#include <variant>
#include <vector>
#include "tokens.hpp"
#include "types.hpp"

namespace faster {
	enum struct node_operation {
		param,

		preinc,
		predec,
		postinc,
		postdec,
		positive,
		negative,
		bnot,
		lnot,
		size,
		tostring,

		add,
		sub,
		mul,
		div,
		idiv,
		mod,
		band,
		bor,
		bxor,
		bsl,
		bsr,
		concat,
		assign,
		add_assign,
		sub_assign,
		mul_assign,
		div_assign,
		idiv_assign,
		mod_assign,
		band_assign,
		bor_assign,
		bxor_assign,
		bsl_assign,
		bsr_assign,
		concat_assign,
		eq,
		ne,
		lt,
		gt,
		le,
		ge,
		comma,
		land,
		lor,
		index,

		ternary,

		call,

		init,
	};

	struct node;
	using node_ptr = std::unique_ptr<node>;

	using node_value = std::variant<
		node_operation,
		std::string,
		std::byte,
		bool,
		short int,
		int,
		long,
		unsigned short int,
		unsigned int,
		unsigned long,
		float,
		double,
		char,
		identifier,
		std::nullopt_t
	>;

	class compiler_context;

	struct node {
	private:
		node_value _value;
		std::vector<node_ptr> _children;
		type_handle _type_id;
		bool _lvalue;
		size_t _line_number;
		size_t _char_index;
	public:
		node(compiler_context& context, node_value value, std::vector<node_ptr> children, size_t line_number, size_t char_index);

		const node_value& get_value() const;

		bool is_node_operation() const;
		bool is_optional() const;
		bool is_identifier() const;

		bool is_enum_token() const;

		bool is_byte() const;

		bool is_boolean() const;

		bool is_int16() const;
		bool is_int32() const;
		bool is_int64() const;

		bool is_uint16() const;
		bool is_uint32() const;
		bool is_uint64() const;

		bool is_double32() const;
		bool is_double64() const;

		bool is_character() const;

		bool is_string() const;
		bool is_null() const;

		node_operation get_node_operation() const;
		std::string_view get_identifier() const;

		std::byte get_byte() const;

		bool get_boolean() const;

		short int get_int16() const;
		int get_int32() const;
		long get_int64() const;

		unsigned short int get_uint16() const;
		unsigned int get_uint32() const;
		unsigned long get_uint64() const;

		float get_double32() const;
		double get_double64() const;

		char get_character() const;

		std::string_view get_string() const;

		std::nullopt_t get_null() const;

		const std::vector<node_ptr>& get_children() const;

		type_handle get_type_id() const;
		bool is_lvalue() const;

		size_t get_line_number() const;
		size_t get_char_index() const;

		void check_conversion(type_handle type_id, bool lvalue) const;
	};
}
#endif