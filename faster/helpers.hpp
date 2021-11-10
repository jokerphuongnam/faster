#pragma once
#ifndef helpers_hpp
#define helpers_hpp

namespace faster {
	//blatantly stolen from https://en.cppreference.com/w/cpp/utility/variant/visit
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;
}
#endif
