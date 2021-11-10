#include "tokens.hpp"
#include "lookup.hpp"
#include <string_view>
#include "helpers.hpp"
#include <stack>
#include <string>
#include "push_back_stream.hpp"

namespace faster {
	namespace {
		const lookup<std::string_view, reserved_token> operator_token_map{
			{"++", reserved_token::inc},
			{"--", reserved_token::dec},

			{"+", reserved_token::add},
			{"-", reserved_token::sub},
			{"..", reserved_token::concat},
			{"*", reserved_token::mul},
			{"/", reserved_token::div},
			{"\\", reserved_token::idiv},
			{"%", reserved_token::mod},

			{"~", reserved_token::bitwise_not},
			{"&", reserved_token::bitwise_and},
			{"|", reserved_token::bitwise_or},
			{"^", reserved_token::bitwise_xor},
			{"<<", reserved_token::shiftl},
			{">>", reserved_token::shiftr},

			{"=", reserved_token::assign},

			{"+=", reserved_token::add_assign},
			{"-=", reserved_token::sub_assign},
			{"..=", reserved_token::concat_assign},
			{"*=", reserved_token::mul_assign},
			{"/=", reserved_token::div_assign},
			{"\\=", reserved_token::idiv_assign},
			{"%=", reserved_token::mod_assign},

			{"&=", reserved_token::and_assign},
			{"|=", reserved_token::or_assign},
			{"^=", reserved_token::xor_assign},
			{"<<=", reserved_token::shiftl_assign},
			{">>=", reserved_token::shiftr_assign},

			{"!", reserved_token::logical_not},
			{"&&", reserved_token::logical_and},
			{"||", reserved_token::logical_or},

			{"==", reserved_token::eq},
			{"!=", reserved_token::ne},
			{"<", reserved_token::lt},
			{">", reserved_token::gt},
			{"<=", reserved_token::le},
			{">=", reserved_token::ge},

			{"->", reserved_token::right_arrow},
			{"<-", reserved_token::left_arrow},

			{"_", reserved_token::underscore},
			{"@", reserved_token::at},
			{"#", reserved_token::sharp},

			{"===", reserved_token::tripple_equal},

			{"?", reserved_token::question},
			{"|", reserved_token::object_or},
			{":", reserved_token::colon},

			{",", reserved_token::comma},
			{".", reserved_token::dot},

			{"...", reserved_token::ellippsis},

			{"<..", reserved_token::lt_concat},
			{">..", reserved_token::gt_concat},

			{"..<", reserved_token::concat_lt},
			{"..>", reserved_token::concat_gt},

			{"??", reserved_token::null_coalescing},

			{";", reserved_token::semicolon},

			{"(", reserved_token::open_round},
			{")", reserved_token::close_round},

			{"{", reserved_token::open_curly},
			{"}", reserved_token::close_curly},

			{"[", reserved_token::open_square},
			{"]", reserved_token::close_square},
		};

		const lookup<std::string_view, reserved_token> number_token_map{
			{"#if", reserved_token::kw_number_if},
			{"#else", reserved_token::kw_number_else},
			{"#elseif", reserved_token::kw_number_elseif},
			{"#endif", reserved_token::kw_number_endif},

			{"#define", reserved_token::kw_number_define},

			{"#file", reserved_token::kw_number_file},
			{"#line", reserved_token::kw_number_line},
			{"#char", reserved_token::kw_number_char},

			{"#function", reserved_token::kw_number_function}
		};

		const lookup<std::string_view, reserved_token> keyword_token_map{
			{"import", reserved_token::kw_import},

			{"null", reserved_token::kw_null},
			{"optional", reserved_token::kw_optional},

			{"enum", reserved_token::kw_enum},

			{"mutating", reserved_token::kw_mutating},

			{"sizeof", reserved_token::kw_sizeof},
			{"tostring", reserved_token::kw_tostring},

			{"is", reserved_token::kw_is},
			{"isExact", reserved_token::kw_is_exact},
			{"as", reserved_token::kw_as},

			{"true", reserved_token::kw_true},
			{"false", reserved_token::kw_false},

			{"if", reserved_token::kw_if},
			{"guard", reserved_token::kw_guard},
			{"else", reserved_token::kw_else},
			{"else if", reserved_token::kw_elif},

			{"switch", reserved_token::kw_switch},
			{"case", reserved_token::kw_case},
			{"default", reserved_token::kw_default},
			{"fallthough", reserved_token::kw_fallthough},

			{"throw", reserved_token::kw_throw},
			{"throws", reserved_token::kw_throws},
			{"rethrows", reserved_token::kw_rethrows},

			{"for", reserved_token::kw_for},
			{"while", reserved_token::kw_while},
			{"do", reserved_token::kw_do},

			{"break", reserved_token::kw_break},
			{"continue", reserved_token::kw_continue},
			{"return", reserved_token::kw_return},

			{"where", reserved_token::kw_where},

			{"typealias", reserved_token::kw_typealias},

			{"func", reserved_token::kw_function},

			{"var", reserved_token::kw_variable},
			{"const", reserved_token::kw_constant},
			{"lazy", reserved_token::kw_lazy},

			{"private", reserved_token::kw_private},
			{"fileprivate", reserved_token::kw_fileprivate},
			{"extensionprivate", reserved_token::kw_extensionprivate},
			{"objectprivate", reserved_token::kw_objectprivate},
			{"protected", reserved_token::kw_protected},
			{"internal", reserved_token::kw_internal},
			{"public", reserved_token::kw_public},

			{"class", reserved_token::kw_class},
			{"struct", reserved_token::kw_struct},
			{"interface", reserved_token::kw_interface},
			{"this", reserved_token::kw_this},
			{"This", reserved_token::kw_This},
			{"abstract", reserved_token::kw_abstract},
			{"constructor", reserved_token::kw_constructor},
			{"init", reserved_token::kw_init},
			{"destructor", reserved_token::kw_destructor},
			{"override", reserved_token::kw_override},

			{"static", reserved_token::kw_static},

			{"get", reserved_token::kw_get},
			{"set", reserved_token::kw_set},

			{"willGet", reserved_token::kw_will_get},
			{"willSet", reserved_token::kw_will_set},
			{"ditSet", reserved_token::kw_did_set}
		};

		const lookup<std::string_view, reserved_token> type_token_map{
			{"Void", reserved_token::kw_void},

			{"Any", reserved_token::kw_any},

			{"Byte", reserved_token::kw_byte},

			{"Bool", reserved_token::kw_bool},

			{"Number", reserved_token::kw_number},

			{"Int", reserved_token::kw_int16},
			{"Long", reserved_token::kw_int32},

			{"Int16", reserved_token::kw_int16},
			{"Int32", reserved_token::kw_int32},
			{"Int64", reserved_token::kw_int64},

			{"UInt", reserved_token::kw_uInt16},
			{"ULong", reserved_token::kw_uInt32},

			{"UInt16", reserved_token::kw_uInt16},
			{"UInt32", reserved_token::kw_uInt32},
			{"UInt64", reserved_token::kw_uInt64},

			{"Double32", reserved_token::kw_double32},
			{"Double64", reserved_token::kw_double64},

			{"Float", reserved_token::kw_double32},
			{"Double", reserved_token::kw_double64},

			{"Char", reserved_token::kw_character},
			{"Character", reserved_token::kw_character},

			{"String", reserved_token::kw_string}
		};

		const lookup<reserved_token, std::string_view> token_string_map = ([]() {
			std::vector<std::pair<reserved_token, std::string_view>> container;
			container.reserve(operator_token_map.size() + keyword_token_map.size());
			for (const auto& p : operator_token_map) {
				container.emplace_back(p.second, p.first);
			}
			for (const auto& p : number_token_map) {
				container.emplace_back(p.second, p.first);
			}
			for (const auto& p : keyword_token_map) {
				container.emplace_back(p.second, p.first);
			}
			for (const auto& p : type_token_map) {
				container.emplace_back(p.second, p.first);
			}
			return lookup<reserved_token, std::string_view>(std::move(container));
			}
		)();
	}

	std::optional<reserved_token> get_keyword(std::string_view word) {
		auto it = keyword_token_map.find(word);
		return it == keyword_token_map.end() ? std::nullopt : std::make_optional(it->second);
	}

	std::optional<reserved_token> get_type(std::string_view word) {
		auto it = type_token_map.find(word);
		return it == type_token_map.end() ? std::nullopt : std::make_optional(it->second);
	}

	std::optional<reserved_token> get_number(std::string_view word) {
		auto it = number_token_map.find(word);
		return it == number_token_map.end() ? std::nullopt : std::make_optional(it->second);
	}

	std::optional<reserved_token> get_operator(std::string_view word) {
		auto it = operator_token_map.find(word);
		return it == operator_token_map.end() ? std::nullopt : std::make_optional(it->second);
	}

	namespace {
		class maximal_munch_comparator {
		private:
			size_t _idx;
		public:
			maximal_munch_comparator(size_t idx) :
				_idx(idx)
			{
			}

			bool operator()(char l, char r) const {
				return l < r;
			}

			bool operator()(std::pair<std::string_view, reserved_token> l, char r) const {
				return l.first.size() <= _idx || l.first[_idx] < r;
			}

			bool operator()(char l, std::pair<std::string_view, reserved_token> r) const {
				return r.first.size() > _idx && l < r.first[_idx];
			}

			bool operator()(std::pair<std::string_view, reserved_token> l, std::pair<std::string_view, reserved_token> r) const {
				return r.first.size() > _idx && (l.first.size() < _idx || l.first[_idx] < r.first[_idx]);
			}
		};
	}

	std::optional<reserved_token> get_operator(push_back_stream& stream) {
		auto candidates = std::make_pair(operator_token_map.begin(), operator_token_map.end());

		std::optional<reserved_token> ret;
		size_t match_size = 0;

		std::stack<int> chars;

		for (size_t idx = 0; candidates.first != candidates.second; ++idx) {
			char c = stream();
			chars.push(c);

			candidates = std::equal_range(candidates.first, candidates.second, char(chars.top()), maximal_munch_comparator(idx));

			if (candidates.first != candidates.second && candidates.first->first.size() == idx + 1) {
				match_size = idx + 1;
				ret = candidates.first->second;
			}
		}

		while (chars.size() > match_size) {
			stream.push_back(chars.top());
			chars.pop();
		}

		return ret;
	}

	token::token(token_value value, size_t line_number, size_t char_index) :
		_value(std::move(value)),
		_line_number(line_number),
		_char_index(char_index) {
	}
	bool token::is_reserved_token() const {
		return std::holds_alternative<reserved_token>(_value);
	}
	bool token::is_identifier() const {
		return std::holds_alternative<identifier>(_value);
	}
	bool token::is_byte() const {
		return std::holds_alternative<std::byte>(_value);
	}
	bool token::is_boolean() const {
		return std::holds_alternative<bool>(_value);
	}
	bool token::is_int16() const {
		return std::holds_alternative<short int>(_value);
	}
	bool token::is_int32() const {
		return std::holds_alternative<int>(_value);
	}
	bool token::is_int64() const {
		return std::holds_alternative<long>(_value);
	}
	bool token::is_uint16() const {
		return std::holds_alternative<unsigned short int>(_value);
	}
	bool token::is_uint32() const {
		return std::holds_alternative<unsigned int>(_value);
	}
	bool token::is_uint64() const {
		return std::holds_alternative<unsigned long>(_value);
	}
	bool token::is_double32() const {
		return std::holds_alternative<float>(_value);
	}
	bool token::is_double64() const {
		return std::holds_alternative<double>(_value);
	}
	bool token::is_character() const {
		return std::holds_alternative<char>(_value);
	}
	bool token::is_string() const {
		return std::holds_alternative<std::string>(_value);
	}
	bool token::is_eof() const {
		return std::holds_alternative<eof>(_value);
	}

	reserved_token token::get_reserved_token() const {
		return std::get<reserved_token>(_value);
	}
	const identifier& token::get_identifier() const {
		return std::get<identifier>(_value);
	}
	std::byte token::get_byte() const {
		return std::get<std::byte>(_value);
	}
	bool token::get_boolean() const {
		return std::get<bool>(_value);
	}
	short int token::get_int16() const {
		return std::get<short int>(_value);
	}
	int token::get_int32() const {
		return std::get<int>(_value);
	}
	long token::get_int64() const {
		return std::get<long>(_value);
	}
	unsigned short int token::get_uint16() const {
		return std::get<unsigned short int>(_value);
	}
	unsigned int token::get_uint32() const {
		return std::get<unsigned int>(_value);
	}
	unsigned long token::get_uint64() const {
		return std::get<unsigned long>(_value);
	}
	float token::get_double32() const {
		return std::get<float>(_value);
	}
	double token::get_double64() const {
		return std::get<double>(_value);
	}
	char token::get_character() const {
		return std::get<char>(_value);
	}
	const std::string& token::get_string() const {
		return std::get<std::string>(_value);
	}
	const token_value& token::get_value() const {
		return _value;
	}
	size_t token::get_line_number() const {
		return _line_number;
	}
	size_t token::get_char_index() const {
		return _char_index;
	}
	bool token::has_value(const token_value& value) const {
		return _value == value;
	}

	bool operator==(const identifier& id1, const identifier& id2) {
		return id1.name == id2.name;
	}

	bool operator!=(const identifier& id1, const identifier& id2) {
		return id1.name != id2.name;
	}

	bool operator==(const null&, const null&) {
		return true;
	}

	bool operator!=(const null&, const null&) {
		return false;
	}

	bool operator==(const eof&, const eof&) {
		return true;
	}

	bool operator!=(const eof&, const eof&) {
		return false;
	}
}

namespace std {
	using namespace faster;
	std::string to_string(reserved_token t) {
		return std::string(token_string_map.find(t)->second);
	}

	std::string to_string(const token_value& t) {
		return std::visit(
			overloaded{
				[](reserved_token rt) -> string {
					return to_string(rt);
				},
				[](const identifier& id) -> string {
					return id.name;
				},
				[](byte b) -> string {
					return (reinterpret_cast<char*>(b));
				},
				[](bool b) -> string {
					return b ? "true" : "false";
				},
				[](short int si) -> string {
					return to_string(si);
				},
				[](int i) -> string {
					return to_string(i);
				},
				[](long l) -> string {
					return to_string(l);
				},
				[](unsigned short int usi) -> string {
					return to_string(usi);
				},
				[](unsigned int ui) -> string {
					return to_string(ui);
				},
				[](unsigned long ul) -> string {
					return to_string(ul);
				},
				[](float f) -> string {
					return to_string(f);
				},
				[](double d) -> string {
					return to_string(d);
				},
				[](char c) -> string {
					return string(1, c);
				},
				[](const std::string& str) -> string {
					return str;
				},
				[](const null&) -> string {
					return "null";
				},
				[](const eof&) -> string {
					return std::string("<EOF>");
				}
			},
			t
					);
	}
}
