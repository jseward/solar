#include "command_line_parser.h"
#include <iomanip>
#include "solar/utility/verify.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/trace.h"
#include "solar/utility/alert.h"
#include "solar/strings/string_marshal.h"

namespace solar {

	command_line_parser::option::option(option_type type, char short_key, const std::string& long_key, const std::string& description)
		: _type(type)
		, _short_key(short_key)
		, _long_key(long_key)
		, _description(description)
		, _is_parsed(false) {
	}

	command_line_parser::option::option(option_type type, char short_key, const std::string& long_key, const std::string& description, const std::string& default_value)
		: _type(type)
		, _short_key(short_key)
		, _long_key(long_key)
		, _description(description)
		, _default_value(default_value)
		, _is_parsed(false) {
	}

	bool command_line_parser::option::is_required() const {
		return (_type == option_type::REQUIRED_VALUE);
	}

	command_line_parser::command_line_parser() {
		add_switch('h', "help", "");
	}

	command_line_parser& command_line_parser::add_switch(char short_key, const std::string& long_key, const std::string& description) {
		verify_unique_key(short_key, long_key);
		_options.emplace_back(option_type::SWITCH, short_key, long_key, description);
		return *this;
	}

	command_line_parser& command_line_parser::add_required_value(char short_key, const std::string& long_key, const std::string& description) {
		verify_unique_key(short_key, long_key);
		_options.emplace_back(option_type::REQUIRED_VALUE, short_key, long_key, description);
		return *this;
	}

	command_line_parser& command_line_parser::add_optional_value(char short_key, const std::string& long_key, const std::string& description, const std::string& default_value) {
		verify_unique_key(short_key, long_key);
		_options.emplace_back(option_type::OPTIONAL_VALUE, short_key, long_key, description, default_value);
		return *this;
	}

	bool command_line_parser::execute(int argc, wchar_t* argv[]) {
		bool success = false;
		IF_VERIFY(argc > 0) {
			std::vector<std::string> args;
			for (int i = 1; i < argc; ++i) {
				args.emplace_back(utf16_to_utf8(argv[i]));
			}

			if (!try_parse(utf16_to_utf8(argv[0]), args)) {
				TRACE(build_usage_string().c_str());
			}
			else {
				if (is_help_set()) {
					TRACE(build_help_string().c_str());
				}
				else {
					success = true;
				}
			}
		}
		return success;
	}

	bool command_line_parser::try_parse(const std::string& command, const std::vector<std::string>& args) {
		_command = command;

		auto next_arg_option_value_iter = _options.end();

		for (const auto& arg : args) {
			if (next_arg_option_value_iter != _options.end()) {
				next_arg_option_value_iter->_parsed_value = arg;
				next_arg_option_value_iter = _options.end();
				continue;
			}

			auto found_option = _options.end();

			const std::size_t len = arg.length();
			if (len == 2) {
				const char short_key = arg[1];
				found_option = find_option(short_key);
			}
			else if (len > 2) {
				if (std::strncmp(arg.c_str(), "--", 2) == 0) {
					const std::string long_key = arg.c_str() + 2;
					found_option = find_option(long_key);
				}
			}

			if (found_option == _options.end()) {
				TRACE("Unknown command line argument : {}", arg);
				return false;
			}

			found_option->_is_parsed = true;
			if (found_option->_type != option_type::SWITCH) {
				next_arg_option_value_iter = found_option;
			}
		}

		if (next_arg_option_value_iter != _options.end()) {
			TRACE("Missing value for command line argument : {}", next_arg_option_value_iter->_long_key);
			return false;
		}

		for (const auto& option : _options) {
			if (option.is_required()) {
				if (!option._is_parsed) {
					TRACE("Missing command line argument : {}", option._long_key);
					return false;
				}
			}
		}

		return true;
	}

	bool command_line_parser::is_help_set() const {
		return is_switch_set("help");
	}

	bool command_line_parser::is_switch_set(const std::string& long_key) const {
		auto option = find_option(long_key);
		IF_VERIFY(option != _options.end()) {
			return option->_is_parsed;
		}
		return false;
	}

	bool command_line_parser::is_value_set(const std::string& long_key) const {
		return !get_value(long_key).empty();
	}

	std::string command_line_parser::get_value(const std::string& long_key) const {
		auto option = find_option(long_key);
		if (option == _options.end()) {
			ALERT("command_line_parser. unexpected long_key : {}", long_key);
		}
		else {
			return option->_is_parsed ?
				option->_parsed_value :
				option->_default_value;
		}
		return "";
	}

	std::vector<command_line_parser::option>::iterator command_line_parser::find_option(char short_key) {
		return find_if(_options, [&short_key](const option& option) {
			return (option._short_key == short_key);
		});
	}

	std::vector<command_line_parser::option>::iterator command_line_parser::find_option(const std::string& long_key) {
		return find_if(_options, [&long_key](const option& option) {
			return (option._long_key == long_key);
		});		
	}

	std::vector<command_line_parser::option>::const_iterator command_line_parser::find_option(char short_key) const {
		return const_cast<command_line_parser*>(this)->find_option(short_key);
	}

	std::vector<command_line_parser::option>::const_iterator command_line_parser::find_option(const std::string& long_key) const {
		return const_cast<command_line_parser*>(this)->find_option(long_key);
	}

	void command_line_parser::verify_unique_key(char short_key, const std::string& long_key) const {
		ASSERT(find_option(short_key) == _options.end());
		ASSERT(find_option(long_key) == _options.end());
	}

	std::string command_line_parser::build_usage_string() const {
		std::ostringstream out;
		out << "Usage:\n  " << _command;
		for (const auto& option : _options) {
			out << ' ';

			if (!option.is_required()) {
				out << "[";
			}

			out << "-" << option._short_key << "|--" << option._long_key;

			if (!option.is_required()) {
				out << "]";
			}
		}
		out << '\n';
		return out.str();
	}

	std::string command_line_parser::build_help_string() const {
		std::ostringstream out;
		
		out << build_usage_string();

		static const std::size_t MAX_KEY_COLUMN_WIDTH = 27;
		std::size_t key_column_width = 11;
		for (const auto& option : _options) {
			std::size_t current_key_column_width = option._long_key.size() + 5;
			key_column_width = std::max(current_key_column_width, key_column_width);
			if (key_column_width >= MAX_KEY_COLUMN_WIDTH) {
				key_column_width = MAX_KEY_COLUMN_WIDTH;
				break;
			}
		}

		out << "\nArguments:\n";

		for (const auto& option : _options) {
			out << "  ";
			out << '-' << option._short_key << ", ";
			out << "--" << std::left << std::setw(key_column_width - 6) << option._long_key << ' ';
			out << option._description << '\n';

			if (!option._default_value.empty()) {
				out << std::string(key_column_width + 3, ' ');
				out << "(default: " << option._default_value << ")\n";
			}
		}

		return out.str();
	}

}