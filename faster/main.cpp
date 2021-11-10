#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <iostream>
#include <string>
#include <filesystem>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <ostream>
#include <functional>
#include "module.cpp"
#include "push_back_stream.hpp"
#include "tokenizer.hpp"

using namespace faster;

void print_token(const tokens_iterator& ti) {
	bool isUndefine = false;
	if (ti->is_reserved_token()) {
		std::cout << "reserved token: ";
	}
	else if (ti->is_identifier()) {
		std::cout << "identifier: ";
	}
	else if (ti->is_byte()) {
		std::cout << "is byte: ";
	}
	else if (ti->is_boolean()) {
		std::cout << "boolean: ";
	}
	else if (ti->is_int16()) {
		std::cout << "int16: ";
	}
	else if (ti->is_int32()) {
		std::cout << "int32: ";
	}
	else if (ti->is_int64()) {
		std::cout << "int64: ";
	}
	else if (ti->is_uint16()) {
		std::cout << "uint16: ";
	}
	else if (ti->is_uint32()) {
		std::cout << "uint32: ";
	}
	else if (ti->is_uint64()) {
		std::cout << "uint64: ";
	}
	else if (ti->is_double32()) {
		std::cout << "double32: ";
	}
	else if (ti->is_double64()) {
		std::cout << "double64: ";
	}
	else if (ti->is_character()) {
		std::cout << "character: ";
	}
	else if (ti->is_string()) {
		std::cout << "string: ";
	}
	else {
		isUndefine = true;
	}
	std::cout << std::to_string(ti->get_value()) << std::endl;
}

void handle_files(std::string dic_path, std::ostream* err = nullptr) {
	for (const std::filesystem::directory_entry& directory : std::filesystem::directory_iterator(dic_path)) {
		std::string path = directory.path().string();
		if (directory.is_directory() == 1) {
			handle_files(path, err);
		}
		if (path.substr(path.find_last_of(".") + 1) == "faster") {
			std::cout << "file: " << path.substr(path.find_last_of("/\\") + 1) << std::endl;
			file f(path.c_str());

			get_character get = [&]() {
				return f();
			};
			push_back_stream stream(&get);
			try {
				tokens_iterator ti(stream);
				print_token(ti);
				do {
					*(++ti);
					print_token(ti);
				} while (!f.is_eof());
			}
			catch (const file_not_found& e) {
				if (err) {
					*err << e.what() << std::endl;
				}
			}
			catch (const error& e) {
				if (err) {
					file f(path.c_str());
					format_error(
						e,
						[&]() {
							return f();
						},
						*err
							);
				}
			}
			catch (const runtime_error& e) {
				if (err) {
					*err << e.what() << std::endl;
				}
			}
		}
	}
}

int main() {
	//int _ = _setmode(_fileno(stdout), _O_U8TEXT);
	std::string path = __FILE__;
	path = path.substr(0, path.find_last_of("/\\") + 1) + "Project";
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(path);

	handle_files(path, &std::cerr);
}