#pragma once
#ifndef push_back_stream_hpp
#define push_back_stream_hpp
#include <stack>
#include <functional>

namespace faster {
	using get_character = std::function<int()>;

	class push_back_stream {
	private:
		const get_character& _input;
		std::stack<int> _stack;
		size_t _line_number;
		size_t _char_index;
	public:
		push_back_stream(const get_character* input);

		int operator()();

		void push_back(int c);
		bool is_empty();

		size_t line_number() const;
		size_t char_index() const;
	};
}

#endif