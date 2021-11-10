#pragma once
#ifndef variable_hpp
#define variable_hpp

#include <memory>
#include <deque>
#include <vector>
#include <functional>
#include <string>
#include <map>
#include <optional>

namespace faster {
	class variable;

	using variable_ptr = std::shared_ptr<variable>;
	using variable_value = variable;
	
	template<typename T>
	class variable_impl;

	class runtime_context;

	using byte = std::byte;

	using boolean = bool;

	using int16 = short int;
	using int32 = int;
	using int64 = long;

	using uint16 = unsigned short int;
	using uint32 = unsigned int;
	using uint64 = unsigned long;

	using double32 = float;
	using double64 = double;

	using character = char;
	using string = std::shared_ptr<std::string>;
	
	using optional = std::optional<variable_ptr>;
	using array = std::deque<variable_ptr>;
	using dictionary_element_ptr = std::shared_ptr<std::pair<variable_ptr, variable_ptr>>;
	using dictionary = std::deque<dictionary_element_ptr>;
	using function = std::function<void(runtime_context&)>;
	using tuple = array;
	using orType = array;
	using initializer_array = array;
	using initializer_dictionary = dictionary;

	using lvalue = variable_ptr;
	using cvalue = variable_ptr;

	using lbyte = std::shared_ptr<variable_impl<std::byte>>;

	using lboolean = std::shared_ptr<variable_impl<bool>>;

	using lint16 = std::shared_ptr<variable_impl<short int>>;
	using lint32 = std::shared_ptr<variable_impl<int>>;
	using lint64 = std::shared_ptr<variable_impl<long>>;

	using luint16 = std::shared_ptr<variable_impl<unsigned short int>>;
	using luint32 = std::shared_ptr<variable_impl<unsigned int>>;
	using luint64 = std::shared_ptr<variable_impl<unsigned long>>;

	using ldouble32 = std::shared_ptr<variable_impl<float>>;
	using ldouble64 = std::shared_ptr<variable_impl<double>>;

	using lcharacter = std::shared_ptr<variable_impl<char>>;

	using lstring = std::shared_ptr<variable_impl<std::string>>;

	using loptional = std::shared_ptr<variable_impl<optional>>;
	using larray = std::shared_ptr<variable_impl<array>>;
	using ldictionary = std::shared_ptr<variable_impl<dictionary>>;
	using lfunction = std::shared_ptr<variable_impl<function>>;
	using ltuple = std::shared_ptr<variable_impl<tuple>>;
	using lorType = std::shared_ptr<variable_impl<orType>>;

	class variable : public std::enable_shared_from_this<variable> {
	private:
		variable(const variable&) = delete;
		void operator=(const variable&) = delete;
	protected:
		variable() = default;
	public:
		virtual ~variable() = default;

		template <typename T>
		T static_pointer_downcast() {
			return std::static_pointer_cast<
				variable_impl<typename T::element_type::value_type>
			>(shared_from_this());
		}

		virtual variable_ptr clone() const = 0;

		virtual string to_string() const = 0;
	};

	template<typename T>
	class variable_impl : public variable {
	public:
		using value_type = T;

		value_type value;

		variable_impl(value_type value);

		variable_ptr clone() const override;

		string to_string() const override;
	};

	optional clone_variable_value(optional value);

	byte clone_variable_value(byte value);
	boolean clone_variable_value(boolean value);
	int16 clone_variable_value(int16 value);
	int32 clone_variable_value(int32 value);
	int64 clone_variable_value(int64 value);
	uint16 clone_variable_value(uint16 value);
	uint32 clone_variable_value(uint32 value);
	uint64 clone_variable_value(uint64 value);
	double32 clone_variable_value(double32 value);
	double64 clone_variable_value(double64 value);
	character clone_variable_value(character value);
	string clone_variable_value(const string& value);
	function clone_variable_value(const function& value);
	array clone_variable_value(const array& value);
	dictionary clone_variable_value(const dictionary& value);
	std::nullopt_t clone_variable_value(const std::nullopt_t& value);

	template <class T>
	T clone_variable_value(const std::shared_ptr<variable_impl<T> >& v) {
		return clone_variable_value(v->value);
	}

	string convert_to_string(optional value);

	string convert_to_string(byte value);
	string convert_to_string(boolean value);
	string convert_to_string(int16 value);
	string convert_to_string(int32 value);
	string convert_to_string(int64 value);
	string convert_to_string(uint16 value);
	string convert_to_string(uint32 value);
	string convert_to_string(uint64 value);
	string convert_to_string(double32 value);
	string convert_to_string(double64 value);
	string convert_to_string(character value);
	string convert_to_string(const string& value);
	string convert_to_string(const function& value);
	string convert_to_string(const array& value);
	string convert_to_string(const dictionary& value);
	string convert_to_string(const lvalue& var);
	string convert_to_string(const std::nullopt_t& var);
}
#endif