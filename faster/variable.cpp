#include "variable.hpp"

namespace faster {
	namespace {
		string from_std_string(std::string str) {
			return std::make_shared<std::string>(std::move(str));
		}
	}

	template<typename T>
	variable_impl<T>::variable_impl(T value) : value(std::move(value)) {}

	template<typename T>
	variable_ptr variable_impl<T>::clone() const {
		return std::make_shared<variable_impl<T> >(clone_variable_value(value));
	}

	template<typename T>
	string variable_impl<T>::to_string() const {
		return convert_to_string(value);
	}

	template class variable_impl<byte>;
	template class variable_impl<boolean>;
	template class variable_impl<int16>;
	template class variable_impl<int32>;
	template class variable_impl<int64>;
	template class variable_impl<uint16>;
	template class variable_impl<uint32>;
	template class variable_impl<uint64>;
	template class variable_impl<double32>;
	template class variable_impl<double64>;
	template class variable_impl<character>;
	template class variable_impl<string>;
	template class variable_impl<function>;
	template class variable_impl<array>;
	template class variable_impl<dictionary>;

	optional clone_variable_value(optional value) {
		return value;
	}

	byte clone_variable_value(byte value) {
		return value;
	}
	boolean clone_variable_value(boolean value) {
		return value;
	}
	int16 clone_variable_value(int16 value) {
		return value;
	}
	int32 clone_variable_value(int32 value) {
		return value;
	}
	int64 clone_variable_value(int64 value) {
		return value;
	}
	uint16 clone_variable_value(uint16 value) {
		return value;
	}
	uint32 clone_variable_value(uint32 value) {
		return value;
	}
	uint64 clone_variable_value(uint64 value) {
		return value;
	}
	double32 clone_variable_value(double32 value) {
		return value;
	}
	double64 clone_variable_value(double64 value) {
		return value;
	}
	character clone_variable_value(character value) {
		return value;
	}
	string clone_variable_value(const string& value) {
		return value;
	}
	function clone_variable_value(const function& value) {
		return value;
	}
	array clone_variable_value(const array& value) {
		array ret;
		for (const variable_ptr& v : value) {
			ret.push_back(v->clone());
		}
		return ret;
	}
	dictionary clone_variable_value(const dictionary& value) {
		dictionary ret;
		for (const dictionary_element_ptr& v : value) {
			ret.push_back(v);
		}
		return ret;
	}

	std::nullopt_t clone_variable_value(const std::nullopt_t& value) {
		return std::nullopt;
	}

	string convert_to_string(optional value) {
		if (value) {
			std::string ret = "Optional(";
			ret += *(value.value()->to_string());
			ret += ")";
			return from_std_string(ret);
		}
		else {
			return from_std_string("null");
		}
	}

	string convert_to_string(byte value) {
		return from_std_string((reinterpret_cast<char*>(value)));
	}
	string convert_to_string(boolean value) {
		return from_std_string(value ? "true" : "false");
	}
	string convert_to_string(int16 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(int32 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(int64 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(uint16 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(uint32 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(uint64 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(double32 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(double64 value) {
		return from_std_string(std::to_string(value));
	}
	string convert_to_string(character value)
	{
		return from_std_string(std::string(1, value));
	}
	string convert_to_string(const string& value) {
		return value;
	}
	string convert_to_string(const function& value) {
		return from_std_string("Function");
	}
	string convert_to_string(const array& value) {
		std::string ret = "[";
		const char* separator = "";
		for (const variable_ptr& v : value) {
			ret += separator;
			ret += *(v->to_string());
			separator = ", ";
		}
		ret += "]";
		return from_std_string(std::move(ret));
	}
	string convert_to_string(const dictionary& value) {
		std::string ret = "[";
		const char* separator = "";
		for (const dictionary_element_ptr& v : value) {
			ret += separator;
			ret += *(v->first->to_string());
			ret += ":";
			ret += *(v->second->to_string());
			separator = ", ";
		}
		ret += "]";
		return from_std_string(std::move(ret));
	}
	string convert_to_string(const lvalue& var) {
		return var->to_string();
	}
	string convert_to_string(const std::nullopt_t& var) {
		return from_std_string("null");
	}
}