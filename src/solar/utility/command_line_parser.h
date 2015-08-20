#pragma once

#include <string>
#include <vector>

namespace solar {

	//lots of general design from Corrade\Utility\Arguments.cpp

	class command_line_parser {
	private:
		enum class option_type {
			SWITCH,
			REQUIRED_VALUE,
			OPTIONAL_VALUE
		};

		class option {
		public:
			option_type _type;
			char _short_key;
			std::string _long_key;
			std::string _description;
			std::string _default_value;
			bool _is_parsed;
			std::string _parsed_value;
			
		public:
			option(option_type type, char short_key, const std::string& long_key, const std::string& description);
			option(option_type type, char short_key, const std::string& long_key, const std::string& description, const std::string& default_value);
			bool is_required() const;
		};

	private:
		std::string _command;
		std::vector<option> _options;

	public:
		command_line_parser();

		command_line_parser& add_switch(char short_key, const std::string& long_key, const std::string& description);
		command_line_parser& add_required_value(char short_key, const std::string& long_key, const std::string& description);
		command_line_parser& add_optional_value(char short_key, const std::string& long_key, const std::string& description, const std::string& default_value);

		bool execute(int argc, wchar_t* argv[]);
		bool try_parse(const std::string& command, const std::vector<std::string>& args);

		bool is_help_set() const;
		bool is_switch_set(const std::string& long_key) const;
		bool is_value_set(const std::string& long_key) const;
		std::string get_value(const std::string& long_key) const;

		std::string build_usage_string() const;
		std::string build_help_string() const;

	private:
		void verify_unique_key(char short_key, const std::string& long_key) const;
		std::vector<option>::iterator find_option(char short_key);
		std::vector<option>::iterator find_option(const std::string& long_key);
		std::vector<option>::const_iterator find_option(char short_key) const;
		std::vector<option>::const_iterator find_option(const std::string& long_key) const;
	};

}