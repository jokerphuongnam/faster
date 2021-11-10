#include "expression_tree.hpp"
#include "helpers.hpp"
#include "errors.hpp"
#include "compiler_context.hpp"

namespace faster {
	namespace {
		bool is_convertible(type_handle type_from, bool lvalue_from, type_handle type_to, bool lvalue_to) {
			if ( type_from != type_registry::get_void_handle() && type_to == type_registry::get_void_handle()) {
				return false;
			}
			if (type_to == type_registry::get_any_handle()) {
				return true;
			}
			if (lvalue_to) {
				return lvalue_from && type_from == type_to;
			}
			if (type_from == type_to) {
				return true;
			}
			if (const init_array_type* ilt = std::get_if<init_array_type>(type_from)) {
				if (lvalue_to) {
					return false;
				}
				if (type_to == type_registry::get_void_handle()) {
					return false;
				}
				if (type_to == type_registry::get_any_handle()) {
					return true;
				}
				return std::visit(overloaded{
					[&](const array_type& at) {
						for (type_handle it : ilt->inner_type_id) {
							if (it != at.inner_type_id) {
								return false;
							}
						}
						return true;
					},
					[&](const tuple_type& tt) {
						if (tt.inner_type_id.size() != ilt->inner_type_id.size()) {
							return false;
						}
						for (size_t i = 0; i < tt.inner_type_id.size(); ++i) {
							if (ilt->inner_type_id[i] != tt.inner_type_id[i].type_id) {
								return false;
							}
						}
						return true;
					},
					[&](const or_type& ot) {
						if (ot.inner_type_id.size() != ilt->inner_type_id.size()) {
							return false;
						}
						for (size_t i = 0; i < ot.inner_type_id.size(); ++i) {
							if (ilt->inner_type_id[i] != ot.inner_type_id[i]) {
								return false;
							}
						}
						return true;
					},
					[&](const type&) {
						return false;
					}
					}, *type_to);
			}
			if (const init_dictionary_type* ilt = std::get_if<init_dictionary_type>(type_from)) {
				if (lvalue_to) {
					return false;
				}
				if (type_to == type_registry::get_void_handle()) {
					return false;
				}
				if (type_to == type_registry::get_any_handle()) {
					return true;
				}
				return std::visit(overloaded{
					[&](const dictionary_type& at) {
						for (dictionary_element it : ilt->inner_type_id) {
							if (!is_convertible(it.key_return_type_id, lvalue_from, at.inner_type_id.key_return_type_id, lvalue_to)) {
								return false;
							}
							if (!is_convertible(it.value_return_type_id, lvalue_from, at.inner_type_id.value_return_type_id, lvalue_to)) {
								return false;
							}
						}
						return true;
					},
					[&](const tuple_type& tt) {
						if (tt.inner_type_id.size() != ilt->inner_type_id.size()) {
							return false;
						}
						for (size_t i = 0; i < tt.inner_type_id.size(); ++i) {
							if (ilt->inner_type_id[i].key_return_type_id != tt.inner_type_id[i].type_id) {
								return false;
							}
							if (ilt->inner_type_id[i].value_return_type_id != tt.inner_type_id[i].type_id) {
								return false;
							}
						}
						return true;
					},
					[&](const or_type& ot) {
						if (ot.inner_type_id.size() != ilt->inner_type_id.size()) {
							return false;
						}
						for (size_t i = 0; i < ot.inner_type_id.size(); ++i) {
							if (ilt->inner_type_id[i].key_return_type_id != ot.inner_type_id[i]) {
								return false;
							}
							if (ilt->inner_type_id[i].value_return_type_id != ot.inner_type_id[i]) {
								return false;
							}
						}
						return true;
					},
					[&](const type&) {
						return false;
					}
					}
				, *type_to);
			}
			return (
				type_from == type_registry::get_byte_handle() ||
				type_from == type_registry::get_bool_handle() ||
				type_from == type_registry::get_number_handle() ||
				type_from == type_registry::get_int_handle() ||
				type_from == type_registry::get_long_handle() ||
				type_from == type_registry::get_int32_handle() ||
				type_from == type_registry::get_int64_handle() ||
				type_from == type_registry::get_uint_handle() ||
				type_from == type_registry::get_ulong_handle() ||
				type_from == type_registry::get_uint16_handle() ||
				type_from == type_registry::get_uint32_handle() ||
				type_from == type_registry::get_uint64_handle() ||
				type_from == type_registry::get_float_handle() ||
				type_from == type_registry::get_double_handle() ||
				type_from == type_registry::get_double32_handle() ||
				type_from == type_registry::get_double64_handle() ||
				type_from == type_registry::get_char_handle() ||
				type_from == type_registry::get_character_handle()
				) && type_to == type_registry::get_string_handle();
		}
	}

	node::node(compiler_context& context, node_value value, std::vector<node_ptr> children, size_t line_number, size_t char_index) :
		_value(std::move(value)),
		_children(std::move(children)),
		_line_number(line_number),
		_char_index(char_index) {

	}

	const node_value& node::get_value() const {
		return _value;
	}

	bool node::is_node_operation() const {
		return std::holds_alternative<node_operation>(_value);
	}

	bool node::is_identifier() const {
		return std::holds_alternative<identifier>(_value);
	}

	bool node::is_byte() const {
		return std::holds_alternative<std::byte>(_value);
	}

	bool node::is_boolean() const {
		return std::holds_alternative<bool>(_value);
	}

	bool node::is_int16() const {
		return std::holds_alternative<short int>(_value);
	}

	bool node::is_int32() const {
		return std::holds_alternative<int>(_value);
	}

	bool node::is_int64() const {
		return std::holds_alternative<long>(_value);
	}

	bool node::is_uint16() const {
		return std::holds_alternative<unsigned short int>(_value);
	}

	bool node::is_uint32() const {
		return std::holds_alternative<unsigned int>(_value);
	}

	bool node::is_uint64() const {
		return std::holds_alternative<unsigned long>(_value);
	}

	bool node::is_double32() const {
		return std::holds_alternative<float>(_value);
	}

	bool node::is_double64() const {
		return std::holds_alternative<double>(_value);
	}

	bool node::is_character() const {
		return std::holds_alternative<char>(_value);
	}

	bool node::is_null() const {
		return std::holds_alternative<std::nullopt_t>(_value);
	}

	bool node::is_string() const {
		return std::holds_alternative<std::string>(_value);
	}

	node_operation node::get_node_operation() const {
		return std::get<node_operation>(_value);
	}

	std::string_view node::get_identifier() const {
		return std::get<identifier>(_value).name;
	}

	std::byte node::get_byte() const {
		return std::get<std::byte>(_value);
	}

	bool node::get_boolean() const {
		return std::get<bool>(_value);
	}

	short int node::get_int16() const {
		return std::get<short int>(_value);
	}

	int node::get_int32() const {
		return std::get<int>(_value);
	}

	long node::get_int64() const {
		return std::get<long>(_value);
	}

	unsigned short int node::get_uint16() const {
		return std::get<unsigned short int>(_value);
	}

	unsigned int node::get_uint32() const {
		return std::get<unsigned int>(_value);
	}

	unsigned long node::get_uint64() const {
		return std::get<unsigned long>(_value);
	}

	float node::get_double32() const {
		return std::get<float>(_value);
	}

	double node::get_double64() const {
		return std::get<double>(_value);
	}

	char node::get_character() const {
		return std::get<char>(_value);
	}

	std::string_view node::get_string() const {
		return std::get<std::string>(_value);
	}

	std::nullopt_t node::get_null() const {
		return std::nullopt;
	}

	const std::vector<node_ptr>& node::get_children() const {
		return _children;
	}

	type_handle node::get_type_id() const {
		return _type_id;
	}

	bool node::is_lvalue() const {
		return _lvalue;
	}

	size_t node::get_line_number() const {
		return _line_number;
	}

	size_t node::get_char_index() const {
		return _char_index;
	}

	void node::check_conversion(type_handle type_id, bool lvalue) const {
		if (!is_convertible(_type_id, _lvalue, type_id, lvalue)) {
			throw wrong_type_error(std::to_string(_type_id), std::to_string(type_id), lvalue,
				_line_number, _char_index);
		}
	}
}