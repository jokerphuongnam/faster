#include "compiler.hpp"
#include "errors.hpp"
#include "compiler_context.hpp"
#include "expression.hpp"
#include "tokenizer.hpp"
#include "runtime_context.hpp"
#include "helpers.hpp"
#include "push_back_stream.hpp"
#include "incomplete_function.hpp"

namespace faster {
	namespace {
		struct possible_flow {
			size_t break_level;
			bool can_continue;
			type_handle return_type_id;

			possible_flow add_loop() {
				return possible_flow{ break_level + 1, true, return_type_id };
			}

			static possible_flow in_function(type_handle return_type_id) {
				return possible_flow{ 0, false, return_type_id };
			}
		};

		error unexpected_syntax(const tokens_iterator& it) {
			return unexpected_syntax_error(std::to_string(it->get_value()), it->get_line_number(), it->get_char_index());
		}

		std::vector<expression<lvalue>::ptr> compile_declaration(compiler_context& ctx, tokens_iterator& it, current_scope in_object) {
			if (it->is_reserved_token()) {
				reserved_token rt = it->get_reserved_token();

				access_modifer am = access_modifer::am_public;
				bool is_static = false;


				switch (rt) {
				case reserved_token::kw_public:
				case reserved_token::kw_protected:
				case reserved_token::kw_internal:
				case reserved_token::kw_private:
				case reserved_token::kw_fileprivate:
				case reserved_token::kw_extensionprivate:
				case reserved_token::kw_objectprivate: {
					if (rt == reserved_token::kw_public) {
						am = access_modifer::am_public;
					}
					else if (rt == reserved_token::kw_protected) {
						if (in_object == current_scope::in_object) {
							am = access_modifer::am_protected;
						}
						else {

						}
					}
					else if (rt == reserved_token::kw_internal) {
						am = access_modifer::am_internal;
					}
					else if (rt == reserved_token::kw_private) {
						if (in_object == current_scope::global) {
							am = access_modifer::am_fileprivate;
						}
						else {
							am = access_modifer::am_private;
						}
					}
					else if (rt == reserved_token::kw_fileprivate) {
						am = access_modifer::am_fileprivate;
					}
					else if (rt == reserved_token::kw_extensionprivate) {
						if (in_object == current_scope::in_extension) {
							am = access_modifer::am_extensionprivate;
						}
						else {

						}
					}
					else if (rt == reserved_token::kw_objectprivate) {
						if (in_object == current_scope::in_object) {
							am = access_modifer::am_objectprivate;
						}
						else {

						}
					}
					++it;
					if (
						(it->has_value(reserved_token::kw_static)) ||
						(it->has_value(reserved_token::kw_function)) ||
						(it->has_value(reserved_token::kw_constant)) ||
						(it->has_value(reserved_token::kw_variable))
						) {

					}
				}
				case reserved_token::kw_static: {
					if (it->is_reserved_token()) {
						if (it->get_reserved_token() == reserved_token::kw_static) {
							is_static = true;
						}
					}
				}
				case reserved_token::kw_function:
				case reserved_token::kw_constant:
				case reserved_token::kw_variable: {

				}
				break;
				default:
					break;
				}
			}
			return std::vector<expression<lvalue>::ptr>();
		}

		std::vector<expression<lvalue>::ptr> compile_constant_declaration(compiler_context& ctx, tokens_iterator& it) {
			return std::vector<expression<lvalue>::ptr>();
		}
	}

	void parse_token_value(compiler_context&, tokens_iterator& it, const token_value& value) {
		if (it->has_value(value)) {
			++it;
			return;
		}
		throw expected_syntax_error(std::to_string(value), it->get_line_number(), it->get_char_index());
	}

	type_handle parse_type(compiler_context& ctx, tokens_iterator& it) {
		if (!it->is_reserved_token()) {
			throw unexpected_syntax(it);
		}
	}
}