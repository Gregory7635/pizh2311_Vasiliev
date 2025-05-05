#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace ArgumentParser {

    class ArgParser {
    private:
        struct Argument {
            char short_name;
            std::string long_name;
            std::string description;
            bool is_flag;
            bool is_multi_value;
            bool is_positional;
            size_t min_args_count;
            std::string string_value;
            std::vector<std::string> string_values;
            int int_value;
            std::vector<int> int_values;
            bool bool_value;
            bool* bool_store;
            std::string* string_store;
            std::vector<int>* int_store;
            std::string default_value;
            bool has_default;
            bool is_help;
        };

        std::string name_;
        std::map<std::string, Argument> arguments_;
        std::map<char, std::string> short_to_long_;
        std::vector<std::string> positional_args_;
        bool help_requested_ = false;

        Argument& GetArgument(const std::string& name);
        bool IsArgumentExists(const std::string& name) const;
        bool IsShortArgumentExists(char short_name) const;
        void ParseArgument(const std::string& arg, std::vector<std::string>& values);
        void HandlePositionalArguments(const std::vector<std::string>& args);
        void StoreValue(Argument& arg, const std::string& value);

    public:
        explicit ArgParser(const std::string& name);

        // String arguments
        ArgParser& AddStringArgument(const std::string& long_name);
        ArgParser& AddStringArgument(char short_name, const std::string& long_name);
        ArgParser& AddStringArgument(const std::string& long_name, const std::string& description);
        ArgParser& Default(const std::string& value);
        ArgParser& StoreValue(std::string& value);
        ArgParser& MultiValue(size_t min_args_count = 0);
        ArgParser& Positional();

        // Int arguments
        ArgParser& AddIntArgument(const std::string& long_name);
        ArgParser& AddIntArgument(char short_name, const std::string& long_name);
        ArgParser& AddIntArgument(const std::string& long_name, const std::string& description);
        ArgParser& StoreValues(std::vector<int>& values);

        // Flag arguments
        ArgParser& AddFlag(const std::string& long_name);
        ArgParser& AddFlag(char short_name, const std::string& long_name);
        ArgParser& AddFlag(const std::string& long_name, const std::string& description);
        ArgParser& StoreValue(bool& value);

        // Help
        ArgParser& AddHelp(char short_name, const std::string& long_name, const std::string& description);

        // Parsing
        bool Parse(int argc, char** argv);
        bool Parse(const std::vector<std::string>& args);

        // Getters
        std::string GetStringValue(const std::string& name, size_t index = 0) const;
        int GetIntValue(const std::string& name, size_t index = 0) const;
        bool GetFlag(const std::string& name) const;

        // Help
        bool Help() const;
        std::string HelpDescription() const;
    };

} // namespace ArgumentParser

#endif // ARGPARSER_H