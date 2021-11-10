#pragma once
#ifndef tokens_hpp
#define tokens_hpp

#include <optional>
#include <string_view>
#include <ostream>
#include <variant>
#include <vector>

namespace faster {
	enum struct reserved_token {
		inc,
		dec,

		add,
		sub,
		concat,
		mul,
		div,
		idiv,
		mod,

		bitwise_not,
		bitwise_and,
		bitwise_or,
		bitwise_xor,
		shiftl,
		shiftr,

		assign,

		add_assign,
		sub_assign,
		concat_assign,
		mul_assign,
		div_assign,
		idiv_assign,
		mod_assign,

		and_assign,
		or_assign,
		xor_assign,
		shiftl_assign,
		shiftr_assign,

		logical_not,
		logical_and,
		logical_or,

		eq,
		ne,
		lt,
		gt,
		le,
		ge,

		right_arrow,
		left_arrow,

		underscore,
		at,
		sharp,

		tripple_equal,

		question,
		colon,

		comma,
		dot,
		double_dot,

		ellippsis,

		lt_double_dot,
		gt_double_dot,

		double_dot_lt,
		double_dot_gt,

		semicolon,

		open_round,
		close_round,

		open_curly,
		close_curly,

		open_square,
		close_square,

		kw_number_if,
		kw_number_else,
		kw_number_elseif,
		kw_number_endif,

		kw_number_define,

		kw_number_file,
		kw_number_line,
		kw_number_char,

		kw_number_function,

		kw_import,

		kw_null,
		kw_optional,

		kw_enum,

		kw_mutating,

		kw_sizeof,
		kw_tostring,

		kw_is,
		kw_is_exact,
		kw_as,

		kw_true,
		kw_false,

		kw_if,
		kw_guard,
		kw_else,
		kw_elif,

		kw_switch,
		kw_case,
		kw_default,

		kw_throw,
		kw_throws,
		kw_rethrows,

		kw_try,
		kw_catch,
		kw_final,

		kw_for,
		kw_while,
		kw_do,

		kw_break,
		kw_continue,
		kw_return,

		kw_where,

		kw_typealias,

		kw_function,

		kw_variable,
		kw_constant,
		kw_lazy,

		kw_void,

		kw_any,

		kw_byte,

		kw_bool,

		kw_number,

		kw_int16,
		kw_int32,
		kw_int64,

		kw_uInt16,
		kw_uInt32,
		kw_uInt64,

		kw_double32,
		kw_double64,

		kw_character,
		kw_string,

		kw_private,
		kw_fileprivate,
		kw_extensionprivate,
		kw_objectprivate,
		kw_protected,
		kw_internal,
		kw_public,

		kw_class,
		kw_struct,
		kw_interface,
		kw_this,
		kw_This,
		kw_abstract,
		kw_constructor,
		kw_init,
		kw_destructor,
		kw_override,

		kw_static,

		kw_get,
		kw_set,

		kw_will_get,
		kw_will_set,
		kw_did_set
	};

	class push_back_stream;

	std::ostream& operator<<(std::ostream& os, reserved_token t);

	std::optional<reserved_token> get_keyword(std::string_view word);

	std::optional<reserved_token> get_type(std::string_view word);

	std::optional<reserved_token> get_number(std::string_view word);

	std::optional<reserved_token> get_operator(std::string_view word);

	std::optional<reserved_token> get_operator(push_back_stream& stream);

	struct identifier {
		std::string name;
	};

	bool operator==(const identifier& id1, const identifier& id2);
	bool operator!=(const identifier& id1, const identifier& id2);

	struct null {};

	bool operator==(const null&, const null&);
	bool operator!=(const null&, const null&);

	struct eof {
	};

	bool operator==(const eof&, const eof&);
	bool operator!=(const eof&, const eof&);

	using token_value = std::variant<
		reserved_token,
		identifier,
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
		std::string,
		null,
		eof
	>;

	class token {
	private:
		token_value _value;
		size_t _line_number;
		size_t _char_index;
	public:
		token(token_value value, size_t line_number, size_t char_index);

		//check
		bool is_reserved_token() const;

		bool is_identifier() const;

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
		bool is_eof() const;

		//get
		reserved_token get_reserved_token() const;

		const identifier& get_identifier() const;

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

		const std::string& get_string() const;
		const token_value& get_value() const;

		size_t get_line_number() const;
		size_t get_char_index() const;

		bool has_value(const token_value& value) const;
	};
}

namespace std {
	std::string to_string(faster::reserved_token t);
	std::string to_string(const faster::token_value& t);
}
#endif