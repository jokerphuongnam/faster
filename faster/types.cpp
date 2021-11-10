#include "types.hpp"
#include "helpers.hpp"

namespace faster {
	bool type_registry::types_less::operator()(
		const type& t1,
		const type& t2
		) const {
		const size_t idx1 = t1.index();
		const size_t idx2 = t2.index();

		if (idx1 != idx2) {
			return idx1 < idx2;
		}

		switch (idx1) {
		case 0:
			return std::get<0>(t1) < std::get<0>(t2);
		case 1:
			return std::get<1>(t1).inner_type_id < std::get<1>(t2).inner_type_id;
		case 2:
			return std::get<2>(t1).type_name < std::get<2>(t2).type_name;
		case 3:
			return std::get<3>(t1).type_name < std::get<3>(t2).type_name;
		case 4:
			return std::get<4>(t1).type_name < std::get<4>(t2).type_name;
		case 5:
			return std::get<5>(t1).type_name < std::get<5>(t2).type_name;
		case 6:
			return std::get<6>(t1).type_name < std::get<6>(t2).type_name;
		case 7:
			return std::get<7>(t1).extension_of < std::get<7>(t2).extension_of;
		case 8:
			return std::get<8>(t1).inner_type_id < std::get<8>(t2).inner_type_id;
		case 9: {
			const dictionary_type& ft1 = std::get<9>(t1);
			const dictionary_type& ft2 = std::get<9>(t2);
			if (ft1.inner_type_id.key_return_type_id != ft2.inner_type_id.key_return_type_id) {
				return ft1.inner_type_id.key_return_type_id < ft2.inner_type_id.key_return_type_id;
			}
			if (ft1.inner_type_id.value_return_type_id != ft2.inner_type_id.value_return_type_id) {
				return ft1.inner_type_id.value_return_type_id < ft2.inner_type_id.value_return_type_id;
			}
			return false;
		}
		case 10: {
			const function_type& ft1 = std::get<10>(t1);
			const function_type& ft2 = std::get<10>(t2);

			if (ft1.return_type_id != ft2.return_type_id) {
				return ft1.return_type_id < ft2.return_type_id;
			}

			if (ft1.param_type_id.size() != ft2.param_type_id.size()) {
				return ft1.param_type_id.size() < ft2.param_type_id.size();
			}

			for (size_t i = 0; i < ft1.param_type_id.size(); ++i) {
				if (ft1.param_type_id[i].label != ft2.param_type_id[i].label) {
					return ft1.param_type_id[i].label < ft2.param_type_id[i].label;
				}
				if (ft1.param_type_id[i].type_id != ft2.param_type_id[i].type_id) {
					return ft1.param_type_id[i].type_id < ft2.param_type_id[i].type_id;
				}
				if (ft1.param_type_id[i].ref != ft2.param_type_id[i].ref) {
					return ft1.param_type_id[i].ref < ft2.param_type_id[i].ref;
				}
			}
			return false;
		}
		case 11: {
			const tuple_type& tt1 = std::get<11>(t1);
			const tuple_type& tt2 = std::get<11>(t2);

			if (tt1.inner_type_id.size() != tt2.inner_type_id.size()) {
				return tt1.inner_type_id.size() < tt2.inner_type_id.size();
			}

			for (size_t i = 0; i < tt1.inner_type_id.size(); ++i) {
				if (tt1.inner_type_id[i].label != tt2.inner_type_id[i].label) {
					return tt1.inner_type_id[i].label < tt2.inner_type_id[i].label;
				}
			}
			return false;
		}
		case 12: {
			const or_type& tt1 = std::get<12>(t1);
			const or_type& tt2 = std::get<12>(t2);

			if (tt1.inner_type_id.size() != tt2.inner_type_id.size()) {
				return tt1.inner_type_id.size() < tt2.inner_type_id.size();
			}

			for (size_t i = 0; i < tt1.inner_type_id.size(); ++i) {
				if (tt1.inner_type_id[i] != tt2.inner_type_id[i]) {
					return tt1.inner_type_id[i] < tt2.inner_type_id[i];
				}
			}
			return false;
		}
		case 13: {
			const generics_type& ilt1 = std::get<13>(t1);
			const generics_type& ilt2 = std::get<13>(t2);

			if (ilt1.generics.size() != ilt2.generics.size()) {
				return ilt1.generics.size() < ilt2.generics.size();
			}

			for (size_t i = 0; i < ilt1.generics.size(); ++i) {
				if (ilt1.generics[i].type_name != ilt2.generics[i].type_name) {
					return ilt1.generics[i].type_name < ilt2.generics[i].type_name;
				}
			}
			return false;
		}
		case 14: {
			const init_array_type& ilt1 = std::get<14>(t1);
			const init_array_type& ilt2 = std::get<14>(t2);

			if (ilt1.inner_type_id.size() != ilt2.inner_type_id.size()) {
				return ilt1.inner_type_id.size() < ilt2.inner_type_id.size();
			}

			for (size_t i = 0; i < ilt1.inner_type_id.size(); ++i) {
				if (ilt1.inner_type_id[i] != ilt2.inner_type_id[i]) {
					return ilt1.inner_type_id[i] < ilt2.inner_type_id[i];
				}
			}
			return false;
		}
		case 15: {
			const init_dictionary_type& ilt1 = std::get<15>(t1);
			const init_dictionary_type& ilt2 = std::get<15>(t2);

			if (ilt1.inner_type_id.size() != ilt2.inner_type_id.size()) {
				return ilt1.inner_type_id.size() < ilt2.inner_type_id.size();
			}

			for (size_t i = 0; i < ilt1.inner_type_id.size(); ++i) {
				if (ilt1.inner_type_id[i].key_return_type_id != ilt2.inner_type_id[i].key_return_type_id) {
					return ilt1.inner_type_id[i].key_return_type_id < ilt2.inner_type_id[i].key_return_type_id;
				}
				if (ilt1.inner_type_id[i].value_return_type_id != ilt2.inner_type_id[i].value_return_type_id) {
					return ilt1.inner_type_id[i].value_return_type_id < ilt2.inner_type_id[i].value_return_type_id;
				}
			}
			return false;
		}
		case 16:
			return std::get<16>(t1).type_name < std::get<16>(t2).type_name;
		}
		return false;
	}

	type_registry::type_registry() {}

	type_handle type_registry::get_handle(const type& t) {
		return std::visit(
			overloaded{
				[](simple_type st) -> type_handle {
					switch (st)
					{
					case simple_type::Void:
						return type_registry::get_void_handle();
					case simple_type::Any:
						return type_registry::get_any_handle();
					case simple_type::Byte:
						return type_registry::get_byte_handle();
					case simple_type::Bool:
						return type_registry::get_bool_handle();
					case simple_type::Number:
						return type_registry::get_number_handle();
					case simple_type::Int:
						return type_registry::get_int_handle();
					case simple_type::Long:
						return type_registry::get_long_handle();
					case simple_type::Int16:
						return type_registry::get_int16_handle();
					case simple_type::Int32:
						return type_registry::get_int32_handle();
					case simple_type::Int64:
						return type_registry::get_int64_handle();
					case simple_type::UInt:
						return type_registry::get_uint_handle();
					case simple_type::ULong:
						return type_registry::get_ulong_handle();
					case simple_type::UInt16:
						return type_registry::get_uint16_handle();
					case simple_type::UInt32:
						return type_registry::get_uint32_handle();
					case simple_type::UInt64:
						return type_registry::get_uint64_handle();
					case simple_type::Float:
						return type_registry::get_float_handle();
					case simple_type::Double:
						return type_registry::get_double_handle();
					case simple_type::Double32:
						return type_registry::get_double32_handle();
					case simple_type::Double64:
						return type_registry::get_double64_handle();
					case simple_type::Char:
						return type_registry::get_char_handle();
					case simple_type::Character:
						return type_registry::get_character_handle();
					case simple_type::String:
						return type_registry::get_number_handle();
				}
				},
				[this](const auto& st) {
						return &(*(_types.insert(st).first));
				}
			},
			t);
	}

	type type_registry::void_type = simple_type::Void;

	type type_registry::any_type = simple_type::Any;

	type type_registry::byte_type = simple_type::Byte;

	type type_registry::bool_type = simple_type::Bool;

	type type_registry::number_type = simple_type::Number;

	type type_registry::int_type = simple_type::Int;
	type type_registry::long_type = simple_type::Long;

	type type_registry::int16_type = simple_type::Int16;
	type type_registry::int32_type = simple_type::Int32;
	type type_registry::int64_type = simple_type::Int64;

	type type_registry::uint_type = simple_type::UInt;
	type type_registry::ulong_type = simple_type::ULong;

	type type_registry::uint16_type = simple_type::Int16;
	type type_registry::uint32_type = simple_type::Int32;
	type type_registry::uint64_type = simple_type::Int64;

	type type_registry::float_type = simple_type::Float;
	type type_registry::double_type = simple_type::Double;

	type type_registry::double32_type = simple_type::Double32;
	type type_registry::double64_type = simple_type::Double64;

	type type_registry::char_type = simple_type::Char;
	type type_registry::character_type = simple_type::Character;
	type type_registry::string_type = simple_type::String;
}

namespace std {
	using namespace faster;
	std::string to_string(type_handle t) {
		return std::visit(
			overloaded{
				[](const simple_type& st) -> std::string {
				switch (st)
				{
				case simple_type::Void:
					return std::string("Void");
				case simple_type::Any:
					return std::string("Any");
				case simple_type::Byte:
					return std::string("Byte");
				case simple_type::Bool:
					return std::string("Bool");
				case simple_type::Number:
					return std::string("Number");
				case simple_type::Int:
					return std::string("Int");
				case simple_type::Long:
					return std::string("Long");
				case simple_type::Int16:
					return std::string("Int16");
				case simple_type::Int32:
					return std::string("Int32");
				case simple_type::Int64:
					return std::string("Int64");
				case simple_type::UInt:
					return std::string("UInt");
				case simple_type::ULong:
					return std::string("ULong");
				case simple_type::UInt16:
					return std::string("UInt16");
				case simple_type::UInt32:
					return std::string("UInt32");
				case simple_type::UInt64:
					return std::string("UInt64");
				case simple_type::Float:
					return std::string("Float");
				case simple_type::Double:
					return std::string("Double");
				case simple_type::Double32:
					return std::string("Double32");
				case simple_type::Double64:
					return std::string("Double64");
				case simple_type::Char:
					return std::string("Char");
				case simple_type::Character:
					return std::string("Character");
				case simple_type::String:
					return std::string("String");
				}
			},
				[](const optional_type& ot) -> std::string {
					std::string ret = to_string(ot.inner_type_id);
					return ret + "?";
				},
				[](const enum_type& et) -> std::string {
					return et.type_name;
				},
				[](const class_type& ct) -> std::string {
					return ct.type_name;
				},
				[](const virtual_class_type& vct) -> std::string {
					return vct.type_name;
				},
				[](const struct_type& st) -> std::string {
					return st.type_name;
				},
				[](const interface_type& it) -> std::string {
					return it.type_name;
				},
				[](const extension_type& et) -> std::string {
					return to_string(et.extension_of);
				},
				[](const array_type& at) ->std::string {
					std::string ret = to_string(at.inner_type_id);
					return "[" + ret + "]";
				},
				[](const dictionary_type& at) ->std::string {
					std::string key = to_string(at.inner_type_id.key_return_type_id);
					std::string value = to_string(at.inner_type_id.value_return_type_id);
					return "[" + key + ":" + value + "]";
				},
				[](const function_type& ft) -> std::string {
					std::string ret = "(";
					const char* separator = "";
					bool isRethows = false;
					for (const function_type::function_param& pa : ft.param_type_id) {
						ret += separator;
						if (pa.label) {
							ret += pa.label.value();
							ret += pa.isOptionalLabel ? "?:" : ":";
						}
						switch (pa.ref) {
						case function_type::function_param::type_ref::Out:
							ret += "out";
						case function_type::function_param::type_ref::InOut:
							ret += "inout";
						}
						ret += " ";
						ret += to_string(pa.type_id);
						ret += pa.is_varargs ? "..." : "";
						separator = ",";
						if (!isRethows && std::holds_alternative<function_type>(*pa.type_id)) {
							isRethows = std::get<function_type>(*pa.type_id).isThrows;
						}
					}
					ret += ")";
					ret += ft.isThrows ? isRethows ? "rethrows:" : "throws:" : ":";
					ret += to_string(ft.return_type_id);
					return ret;
				},
				[](const tuple_type& tt) -> std::string {
					std::string ret = "(";
					const char* separator = "";
					for (const param& it : tt.inner_type_id) {
						if (it.label) {
							ret += it.label.value();
							ret += ":";
						}
						ret += separator + to_string(it.type_id);
						separator = ",";
					}
					ret += ")";
					return ret;
				},
				[](const or_type& ot) -> std::string {
					std::string ret = "";
					const char* separator = "";
					for (const type_handle& it : ot.inner_type_id) {
						ret += separator + to_string(it);
						separator = "|";
					}
					return ret;
				},
				[](const generics_type& gt) -> std::string {
					std::string ret = "<";
					const char* separator = "";
					for (const generics_type::generic_element& ge : gt.generics) {
						ret += separator + ge.type_name;
						if (ge.extend_of.has_value()) {
							ret += ":";
							ret += ge.extend_of.value().type_name;
						}
						else {
							ret += ge.implement_of.empty() ? "" : ":";
							separator = "";
						}

						for (const interface_type it : ge.implement_of) {
							ret += it.type_name;
							separator = ",";
						}

						separator = ",";
					}
					ret += ">";
					return ret;
				},
				[](const init_array_type& ilt) {
					std::string ret = "[";
					const char* separator = "";
					for (const type_handle& it : ilt.inner_type_id) {
						ret += separator + to_string(it);
						separator = ",";
					}
					ret += "]";
					return ret;
				},
				[](const init_dictionary_type& ilt) {
					std::string ret = "[";
					const char* separator = "";
					for (const dictionary_element& it : ilt.inner_type_id) {
						ret += separator + to_string(it.key_return_type_id);
						ret += ":";
						ret += to_string(it.value_return_type_id);
						separator = ",";
					}
					ret += "]";
					return ret;
				},
				[](const typealias_type& tt) {
					std::string ret = to_string(tt.inner_type_id);
					return ret;
				}
			},
			*t);
	}
}