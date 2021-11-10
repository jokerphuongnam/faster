#include <cstdio>
#include "errors.hpp"
namespace faster {
	namespace {
		class file {
			file(const file&) = delete;
			void operator=(const file&) = delete;
		private:
			FILE* _fp;
		public:
			file(const wchar_t* path) : file((char*)path) {

			}

			file(const char* path) {
				fopen_s(&_fp, path, "rt");
				if (!_fp) {
					throw file_not_found(std::string("'") + path + "' not found");
				}
			}

			bool is_eof() const;

			~file() {
				if (_fp) {
					fclose(_fp);
				}
			}

			int operator()() {
				return fgetc(_fp);
			}
		};
		bool file::is_eof() const {
			return feof(_fp);
		}
	}
}