#pragma once
#ifndef compiler_object_hpp
#define compiler_object_hpp

#include <memory>
#include "variable.hpp"
#include "incomplete_function.hpp"

namespace faster {
	class compiler_object;
	class compiler_interface;
	class compiler_class;
	class compiler_struct;
	class compiler_enum;

	class compiler_object {

	};

	class compiler_interface : public compiler_object {
		struct compiler_interface_method {
			std::string name;
			std::vector<std::string> params;
			type_handle type_id;
			bool is_optional;
		};

		struct compiler_interface_property {
			std::string name;
			type_handle type_id;
			bool can_get;
			bool can_set;
		};

		struct compiler_contructor {
			std::vector<std::string> params;
			bool is_optional;
		};

		std::vector<compiler_interface_method> methods;
		std::vector<compiler_interface_property> properties;
		std::vector<compiler_contructor> require_contructors;
	};

	struct compiler_variable {
		std::string name;
		type_handle type_id;
	};

	struct compiler_getter {
		function getter;
		type_handle type_id;
		compiler_getter(type_handle type_id) {
			this->type_id = type_id;
		}
	};

	struct compiler_setter {
		function setter;
		std::string param;
		static compiler_setter setter(std::string param = "value") {
			compiler_setter set;
			set.param = param;
			return set;
		}
		static compiler_setter willGet(std::string param = "value") {
			compiler_setter set;
			set.param = param;
			return set;
		}
		static compiler_setter willSet(std::string param = "newValue") {
			compiler_setter set;
			set.param = param;
			return set;
		}
		static compiler_setter didSet(std::string param = "oldValue") {
			compiler_setter set;
			set.param = param;
			return set;
		}
	};

	struct compiler_compute_variable: compiler_variable {
		
	};

	struct compiler_store_variable : compiler_variable {

	};

	class compiler_class {

	};

	class compiler_struct {
		std::string case_name;

		
	};
}

#endif