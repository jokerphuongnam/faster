#include "tokenizer.hpp"
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <stack>
#include <cstdlib>
#include "push_back_stream.hpp"
#include "errors.hpp"

namespace faster {
	namespace {
		enum struct character_type {
			eof,
			space,
			alphanum,
			punct
		};

		character_type get_character_type(int c) {
			if (c < 0) {
				return character_type::eof;
			}
			if (std::isspace(c)) {
				return character_type::space;
			}
			if (
				c == '#' ||
				c == '_' ||
				c == '.' ||
				c == '$' ||
				std::isalpha(c) ||
				std::isdigit(c)
				) {
				return character_type::alphanum;
			}
			return character_type::punct;
		}

		std::string trim(std::string string) {
			std::string_view s(string.c_str());
			s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
			s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
			return std::string(s);
		}

		constexpr std::string_view  single_tokens = "{}()[]@:";

		token fetch_operator(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			if (std::optional<reserved_token> t = get_operator(stream)) {
				return token(*t, line_number, char_index);
			}
			else {
				std::string unexpected;
				size_t err_line_number = stream.line_number();
				size_t err_char_index = stream.char_index();
				for (int c = stream(); get_character_type(c) == character_type::punct; c = stream()) {
					unexpected.push_back(char(c));
				}
				throw unexpected_error(unexpected, err_line_number, err_char_index);
			}
		}

		short count_dot_in_number(const std::string& s) {
			int nb_point = 0;
			for (int i = 0; i < s.length(); i++) {
				if (s[i] == '.')
				{
					nb_point++;
				}
				else if (!isdigit(s[i])) {
					return 2;
				}
			}
			if (s.empty() || s.back() == '.') {
				return 2;
			}
			if (nb_point <= 1) {
				return nb_point;
			}
			else {
				return 2;
			}
		}

		token fetch_word(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			std::string word;

			int c = stream();

			bool is_start_dot = c == '.';
			bool is_start_underscore = c == '_';

			if (
				single_tokens.find(std::string(1, c)) != std::string::npos
				) {
				if (std::optional<reserved_token> t = get_operator(std::string(1, c))) {
					return token(*t, line_number, char_index);
				}
			}

			do {
				word.push_back(char(c));
				c = stream();

				if (is_start_dot) {
					if (std::isalpha(c)) {
						stream.push_back(c);
						do {
							stream.push_back(word.back());
							word.pop_back();
						} while (word.size());
						return fetch_operator(stream);
					}
				}
				else {
					if (c == '.' && std::isalpha(word.back())) {
						break;
					}
				}

				if (
					(word.back() == '.' || c == '.' && std::isalpha(word.back())) && word.size() > 1) {
					stream.push_back(c);
					do {
						stream.push_back(word.back());
						word.pop_back();
					} while (word.size());
					return fetch_operator(stream);
				}

				if ((c == ' ' || c == '#') && word.size() > 1) {
					break;
				}

				if (
					c == ' ' ||
					single_tokens.find(std::string(1, c)) != std::string::npos
					) {
					break;
				}

			} while (get_character_type(c) == character_type::alphanum && c > 0);

			if (trim(word).size() == 1) {
				if (std::optional<reserved_token> t = get_operator(word)) {
					return token(*t, line_number, char_index);
				}
			}

			if (word.empty()) {
				return fetch_operator(stream);
			}

			if (word == "else") {
				word = trim(word);
				std::string anotherString = "";
				do {
					anotherString.push_back(char(c));
					c = stream();
				} while (get_character_type(c) == character_type::alphanum);
				if (anotherString.find("if") != std::string::npos) {
					anotherString = trim(anotherString);
					word += " ";
					word += anotherString;
				}
				else {
					do {
						stream.push_back(anotherString.back());
						anotherString.pop_back();
					} while (anotherString.size());
				}
			}

			const char end = word.back();
			std::string sub = word.substr(0, word.size() - 1);

			if (is_start_dot) {
				char last_word = word.back();
				if (last_word == 'f' || last_word == 'F') {
					std::string number = sub.substr(1, sub.size() - 1);
					if (count_dot_in_number(number) == 0) {
						word = "0" + word;
					}
				}
				else {
					std::string number = word.substr(1, word.size() - 1);
					if (count_dot_in_number(number) == 0) {
						word = "0" + word;
					}
					else {
						char* endptr;
						auto _ = strtod(word.c_str(), &endptr);
						size_t remaining = word.size() - (endptr - word.c_str());
						throw unexpected_error(
							std::string(1, char(*endptr)),
							stream.line_number() - 1,
							stream.char_index() - remaining - 1
						);
					}
				}
			}

			short count_dot_in_sub = 2;
			if (is_start_underscore) {
				if (end == 'f' || end == 'F' || end == 'l' || end == 'L') {
					count_dot_in_sub = count_dot_in_number(sub.substr(1, sub.size() - 1));
					if (count_dot_in_sub < 2) {
						throw syntax_error(word, line_number, char_index);
					}
				}
				else {
					count_dot_in_sub = count_dot_in_number(word.substr(1, word.size() - 1));
					if (count_dot_in_sub < 2) {
						throw syntax_error(word, line_number, char_index);
					}
				}
				count_dot_in_sub = 2;
			}
			else {
				if (end == 'f' || end == 'F' || end == 'l' || end == 'L') {
					count_dot_in_sub = count_dot_in_number(word.substr(0, word.size() - 1));
				}
				else {
					count_dot_in_sub = count_dot_in_number(word);
				}
			}

			stream.push_back(c);

			if (word.front() == '#') {
				if (const std::optional<reserved_token> t = get_number(word)) {
					return token(*t, line_number, char_index);
				}
			}
			if (std::optional<reserved_token> t = get_type(word)) {
				return token(*t, line_number, char_index);
			}
			if (std::optional<reserved_token> t = get_keyword(word)) {
				if (t == reserved_token::kw_true) {
					return token(true, line_number, char_index);
				}
				if (t == reserved_token::kw_false) {
					return token(false, line_number, char_index);
				}
				if (t == reserved_token::kw_null) {
					return token(null{}, line_number, char_index);
				}
				return token(*t, line_number, char_index);
			}
			if (std::isdigit(word.front())) {
				if (count_dot_in_sub < 2) {
					if (end == 'F' || end == 'f') {
						float num = std::stof(sub);
						return token(num, line_number, char_index);
					}
					if ((end == 'L' || end == 'l')) {
						long num = std::stol(sub);
						return token(num, line_number, char_index);
					}
					if (std::isdigit(end)) {
						if (count_dot_in_sub == 0) {
							int num = std::atoi(word.c_str());
							return token(num, line_number, char_index);
						}
						double num = std::stod(word);
						return token(num, line_number, char_index);
					}
				}

				char* endptr;
				auto _ = strtod(word.c_str(), &endptr);
				size_t remaining = word.size() - (endptr - word.c_str());
				throw unexpected_error(
					std::string(1, char(*endptr)),
					stream.line_number(),
					stream.char_index() - remaining
				);
			}
			else {
				return token(identifier{ std::move(word) }, line_number, char_index);
			}
		}

		token fetch_string(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			std::string str;

			bool escaped = false;
			int c = stream();
			for (; get_character_type(c) != character_type::eof; c = stream()) {
				if (c == '\\') {
					escaped = true;
				}
				else {
					if (escaped) {
						switch (c) {
						case 't':
							str.push_back('\t');
							break;
						case 'n':
							str.push_back('\n');
							break;
						case 'r':
							str.push_back('\r');
							break;
						case '0':
							str.push_back('\0');
							break;
						default:
							str.push_back(c);
							break;
						}
						escaped = false;
					}
					else {
						switch (c) {
						case '\t':
						case '\n':
						case '\r':
							stream.push_back(c);
							throw parsing_error("Expected closing '\"'", stream.line_number(), stream.char_index());
						case '"':
							return token(std::move(str), line_number, char_index);
						default:
							str.push_back(c);
						}
					}
				}
			}
			stream.push_back(c);
			throw parsing_error("Expected closing '\"'", stream.line_number(), stream.char_index());
		}

		void skip_line_comment(push_back_stream& stream) {
			int c;
			do {
				c = stream();
			} while (c != '\n' && get_character_type(c) != character_type::eof);

			if (c != '\n') {
				stream.push_back(c);
			}
		}

		void skip_block_comment(push_back_stream& stream) {
			bool closing = false;
			int c;
			do {
				c = stream();
				if (closing && c == '/') {
					return;
				}
				closing = (c == '*');
			} while (get_character_type(c) != character_type::eof);

			stream.push_back(c);
			throw parsing_error("Expected closing '*/'", stream.line_number(), stream.char_index());
		}

		token fetch_char(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			char key = 0;
			int c = stream();
			if (c == '\\') {
				c = stream();
				switch (c) {
				case 't':
					key = '\t';
					break;
				case 'n':
					key = '\n';
					break;
				case 'r':
					key = '\r';
					break;
				case '0':
					key = '\0';
					break;
				default:
					key = c;
					break;
				}
			}
			else {
				key = c;
			}

			c = stream();

			switch (c) {
			case '\t':
			case '\n':
			case '\'':
				return token(char(key), line_number, char_index);
			default:
				stream.push_back(c);
				throw parsing_error("Expected closing '\''", stream.line_number(), stream.char_index());
			}
		}

		token tokenize(push_back_stream& stream) {
			while (true) {
				size_t line_number = stream.line_number();
				size_t char_index = stream.char_index();
				int c = stream();
				switch (get_character_type(c)) {
				case character_type::eof:
					return { eof(), line_number, char_index };
				case character_type::space:
					continue;
				case character_type::alphanum: {
					stream.push_back(c);
					return fetch_word(stream);
				}
				case character_type::punct: {
					switch (c) {
					case '"':
						return fetch_string(stream);
					case '\'':
						return fetch_char(stream);
					case '/': {
						char c1 = stream();
						switch (c1) {
						case '/':
							skip_line_comment(stream);
							continue;
						case '*':
							skip_block_comment(stream);
							continue;
						default:
							stream.push_back(c1);
						}
					}
					default: {
						stream.push_back(c);
						return fetch_operator(stream);
					}
						   break;
					}
				}
				}
			}
		}
	}

	tokens_iterator& tokens_iterator::operator++() {
		_current = _get_next_token();
		return *this;
	}

	const token& tokens_iterator::operator*() const {
		return _current;
	}

	const token* tokens_iterator::operator->() const {
		return &_current;
	}

	tokens_iterator::operator bool() const {
		return !_current.is_eof();
	}

	tokens_iterator::tokens_iterator(push_back_stream& stream) :
		_current(eof(), 0, 0),
		_get_next_token([&stream]() {
		return tokenize(stream);
			}
		)
	{
		++(*this);
	}

			tokens_iterator::tokens_iterator(std::deque<token>& tokens) :
				_current(eof(), 0, 0),
				_get_next_token([&tokens]() {
				if (tokens.empty()) {
					return token(eof(), 0, 0);
				}
				else {
					token ret = std::move(tokens.front());
					tokens.pop_front();
					return ret;
				}
					}
				) {
				++(*this);
			}
}