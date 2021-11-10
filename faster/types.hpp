#pragma once
#ifndef types_h
#define types_h

#include <vector>
#include <variant>
#include <set>
#include <ostream>
#include <optional>

namespace faster {
	enum struct simple_type {
		Void,

		Any,

		Byte,

		Bool,

		Number,

		Int,
		Long,

		Int16,
		Int32,
		Int64,

		UInt,
		ULong,

		UInt16,
		UInt32,
		UInt64,

		Float,
		Double,

		Double32,
		Double64,

		Char,
		Character,
		String,
	};

	struct optional_type;
	// object
	struct object_type;
	struct enum_type;
	struct class_type;
	struct virtual_class_type;
	struct struct_type;
	struct interface_type;
	struct extension_type;

	struct array_type;
	struct dictionary_element;
	struct dictionary_type;
	struct function_type;
	struct tuple_type;
	struct or_type;
	struct generics_type;
	struct init_array_type;
	struct init_dictionary_type;
	struct typealias_type;

	using type = std::variant <
		simple_type,
		optional_type,
		enum_type,
		class_type,
		virtual_class_type,
		struct_type,
		interface_type,
		extension_type,
		array_type,
		dictionary_type,
		function_type,
		tuple_type,
		or_type,
		generics_type,
		init_array_type,
		init_dictionary_type,
		typealias_type
	>;

	using type_handle = const type*;

	struct optional_type {
		type_handle inner_type_id;
	};

	struct object_type {
		std::string type_name;
		bool is_final;
	};

	struct enum_type: object_type {
		struct enum_element {

		};

		struct enum_child {

		};
	};

	struct object_element : object_type {
		enum element_type {
			property,
			method
		};
		bool is_static;
		element_type type;
	};

	enum struct builder_type {
		result_builder,
		function_builder,
		interface_builder,
		property_wrapped
	};

	struct class_type : object_type {
		std::vector<object_element> element;
		std::vector<interface_type> implement_of;
		std::optional<object_type> extend_of;
		bool has_extend() {
			return extend_of == std::nullopt;
		}
		std::optional<builder_type> builder;
	};

	struct virtual_class_type : object_type {
		std::vector<object_element> element;
	};

	struct struct_type : object_type {
		std::vector<object_element> element;
		std::optional<builder_type> builder;
	};

	struct interface_type : object_type {
		struct interface_element : object_element {
			type_handle inner_type_id;
			bool is_optional;
			bool is_default;
		};
		std::vector<interface_element> element;
	};

	struct extension_type {
		struct extension_element : object_element {
			type_handle inner_type_id;
		};
		std::vector<extension_element> element;
		type_handle extension_of;
	};

	struct array_type {
		type_handle inner_type_id;
	};

	struct dictionary_element {
		type_handle key_return_type_id;
		type_handle value_return_type_id;
	};

	struct dictionary_type {
		dictionary_element inner_type_id;
	};

	struct param {
		bool isOptionalLabel;
		std::optional<std::string> label;
		type_handle type_id;
	};

	struct function_type {
		struct function_param : param {
			enum struct type_ref {
				In = 0,
				Out,
				InOut
			};
			type_ref ref;
			bool is_varargs;
		};
		type_handle return_type_id;
		std::vector<function_param> param_type_id;
		bool isThrows;
	};

	struct tuple_type {
		std::vector<param> inner_type_id;
	};

	struct or_type {
		std::vector<type_handle> inner_type_id;
	};

	struct generics_type {
		struct generic_element {
			std::string type_name;
			std::vector<interface_type> implement_of;
			std::optional<object_type> extend_of;
		};
		std::vector<generic_element> generics;
	};

	struct init_array_type {
		std::vector<type_handle> inner_type_id;
	};

	struct init_dictionary_type {
		std::vector<dictionary_element> inner_type_id;
	};

	struct typealias_type {
		std::string type_name;
		type_handle inner_type_id;
	};

	class type_registry {
	private:
		struct types_less {
			bool operator()(const type& t1, const type& t2) const;
		};
		std::set<type, types_less> _types;

		static type void_type;

		static type any_type;

		static type byte_type;

		static type bool_type;

		static type number_type;

		static type	int_type;
		static type	long_type;

		static type int16_type;
		static type int32_type;
		static type int64_type;

		static type	uint_type;
		static type	ulong_type;

		static type uint16_type;
		static type uint32_type;
		static type uint64_type;

		static type	float_type;
		static type	double_type;

		static type double32_type;
		static type double64_type;

		static type char_type;
		static type character_type;
		static type string_type;
	public:
		type_registry();

		type_handle get_handle(const type& t);

		// Void
		static type_handle get_void_handle() {
			return &void_type;
		}
		// Any
		static type_handle get_any_handle() {
			return &any_type;
		}
		// Byte
		static type_handle get_byte_handle() {
			return &byte_type;
		}
		// Bool
		static type_handle get_bool_handle() {
			return &bool_type;
		}
		// Number
		static type_handle get_number_handle() {
			return &number_type;
		}
		// Interger (4byte, 8byte)
		static type_handle get_int_handle() {
			return &int_type;
		}
		static type_handle get_long_handle() {
			return&long_type;
		}
		// Interger (2byte, 4byte, 8byte)
		static type_handle get_int16_handle() {
			return &int16_type;
		}
		static type_handle get_int32_handle() {
			return &int32_type;
		}
		static type_handle get_int64_handle() {
			return &int64_type;
		}
		// Unsigned Interger (4byte, 8byte)
		static type_handle get_uint_handle() {
			return &uint_type;
		}
		static type_handle get_ulong_handle() {
			return&ulong_type;
		}
		// Unsigned Interger (2byte, 4byte, 8byte)
		static type_handle get_uint16_handle() {
			return &uint16_type;
		}
		static type_handle get_uint32_handle() {
			return &uint32_type;
		}
		static type_handle get_uint64_handle() {
			return &uint64_type;
		}
		// Real Number (4byte, 8byte)
		static type_handle get_float_handle() {
			return &float_type;
		}
		static type_handle get_double_handle() {
			return &double_type;
		}
		// Real Number (4byte, 8byte)
		static type_handle get_double32_handle() {
			return &double32_type;
		}
		static type_handle get_double64_handle() {
			return &double64_type;
		}
		// String
		static type_handle get_char_handle() {
			return &char_type;
		}
		static type_handle get_character_handle() {
			return &character_type;
		}
		static type_handle get_string_handle() {
			return &string_type;
		}
	};
}

namespace std {
	std::string to_string(faster::type_handle t);
}
#endif