#include "ArgParser.h"
#include <iostream>

namespace ArgumentParser {

    ArgParser::ArgParser(const std::string& name) : name_(name) {}

    ArgParser& ArgParser::AddStringArgument(const std::string& long_name) {
        arguments_[long_name] = Argument{ 0, long_name, "", false, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        return *this;
    }

    ArgParser& ArgParser::AddStringArgument(char short_name, const std::string& long_name) {
        arguments_[long_name] = Argument{ short_name, long_name, "", false, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        short_to_long_[short_name] = long_name;
        return *this;
    }

    ArgParser& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
        arguments_[long_name] = Argument{ 0, long_name, description, false, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        return *this;
    }

    ArgParser& ArgParser::AddIntArgument(const std::string& long_name) {
        arguments_[long_name] = Argument{ 0, long_name, "", false, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        return *this;
    }

    ArgParser& ArgParser::AddIntArgument(char short_name, const std::string& long_name) {
        arguments_[long_name] = Argument{ short_name, long_name, "", false, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        short_to_long_[short_name] = long_name;
        return *this;
    }

    ArgParser& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
        arguments_[long_name] = Argument{ 0, long_name, description, false, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        return *this;
    }

    ArgParser& ArgParser::AddFlag(const std::string& long_name) {
        arguments_[long_name] = Argument{ 0, long_name, "", true, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        return *this;
    }

    ArgParser& ArgParser::AddFlag(char short_name, const std::string& long_name) {
        arguments_[long_name] = Argument{ short_name, long_name, "", true, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        short_to_long_[short_name] = long_name;
        return *this;
    }

    ArgParser& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
        arguments_[long_name] = Argument{ 0, long_name, description, true, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, false };
        return *this;
    }

    ArgParser& ArgParser::Default(const std::string& value) {
        arguments_.rbegin()->second.default_value = value;
        arguments_.rbegin()->second.has_default = true;
        return *this;
    }

    ArgParser& ArgParser::StoreValue(std::string& value) {
        arguments_.rbegin()->second.string_store = &value;
        return *this;
    }

    ArgParser& ArgParser::StoreValues(std::vector<int>& values) {
        arguments_.rbegin()->second.int_store = &values;
        return *this;
    }

    ArgParser& ArgParser::StoreValue(bool& value) {
        arguments_.rbegin()->second.bool_store = &value;
        return *this;
    }

    ArgParser& ArgParser::MultiValue(size_t min_args_count) {
        arguments_.rbegin()->second.is_multi_value = true;
        arguments_.rbegin()->second.min_args_count = min_args_count;
        return *this;
    }

    ArgParser& ArgParser::Positional() {
        arguments_.rbegin()->second.is_positional = true;
        return *this;
    }

    ArgParser& ArgParser::AddHelp(char short_name, const std::string& long_name, const std::string& description) {
        arguments_[long_name] = Argument{ short_name, long_name, description, true, false, false, 0, "", {}, 0, {}, false, nullptr, nullptr, nullptr, "", false, true };
        short_to_long_[short_name] = long_name;
        return *this;
    }

    bool ArgParser::Parse(int argc, char** argv) {
        std::vector<std::string> args;
        for (int i = 1; i < argc; ++i) {
            args.push_back(argv[i]);
        }
        return Parse(args);
    }

    bool ArgParser::Parse(const std::vector<std::string>& args) {
        std::vector<std::string> values;

        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i].substr(0, 2) == "--") {
                if (args[i].size() == 2) { // Handle "--" as positional argument marker
                    for (size_t j = i + 1; j < args.size(); ++j) {
                        positional_args_.push_back(args[j]);
                    }
                    break;
                }
                ParseArgument(args[i], values);
            }
            else if (args[i][0] == '-') {
                if (args[i].size() == 1) { // Handle "-" as positional argument
                    positional_args_.push_back(args[i]);
                    continue;
                }
                ParseArgument(args[i], values);
            }
            else {
                positional_args_.push_back(args[i]);
            }
        }

        HandlePositionalArguments(positional_args_);

        // Check required arguments
        for (const auto& [name, arg] : arguments_) {
            if (!arg.is_flag && !arg.has_default && !arg.is_positional && arg.string_value.empty() && arg.int_values.empty()) {
                if (!arg.is_multi_value || (arg.is_multi_value && arg.string_values.empty() && arg.min_args_count > 0)) {
                    return false;
                }
            }
        }

        return true;
    }

    void ArgParser::ParseArgument(const std::string& arg, std::vector<std::string>& values) {
        // ќбработка флагов (--flag или -f)
        if (arg.substr(0, 2) == "--" && arg.find('=') == std::string::npos) {
            std::string flag_name = arg.substr(2);
            if (IsArgumentExists(flag_name)) {
                Argument& argument = GetArgument(flag_name);
                if (argument.is_flag) {
                    argument.bool_value = true;
                    if (argument.bool_store) {
                        *argument.bool_store = true;
                    }
                    if (argument.is_help) {
                        help_requested_ = true;
                    }
                }
            }
            return;
        }
        size_t equal_pos = arg.find('=');
        std::string name_part = (equal_pos == std::string::npos) ? arg : arg.substr(0, equal_pos);
        std::string value_part = (equal_pos == std::string::npos) ? "" : arg.substr(equal_pos + 1);

        if (name_part.substr(0, 2) == "--") {
            std::string long_name = name_part.substr(2);
            if (IsArgumentExists(long_name)) {
                Argument& argument = GetArgument(long_name);
                if (argument.is_flag) {
                    argument.bool_value = true;
                    if (argument.bool_store) {
                        *argument.bool_store = true;
                    }
                    if (argument.is_help) {
                        help_requested_ = true;
                    }
                }
                else {
                    if (!value_part.empty()) {
                        StoreValue(argument, value_part);
                    }
                }
            }
        }
        else if (name_part[0] == '-') {
            std::string short_names = name_part.substr(1);
            for (size_t i = 0; i < short_names.size(); ++i) {
                char short_name = short_names[i];
                if (IsShortArgumentExists(short_name)) {
                    Argument& argument = GetArgument(short_to_long_[short_name]);
                    if (argument.is_flag) {
                        argument.bool_value = true;
                        if (argument.bool_store) {
                            *argument.bool_store = true;
                        }
                        if (argument.is_help) {
                            help_requested_ = true;
                        }
                    }
                    else {
                        if (i == short_names.size() - 1 && !value_part.empty()) {
                            StoreValue(argument, value_part);
                        }
                        else if (i == short_names.size() - 1 && value_part.empty() && equal_pos != std::string::npos) {
                            // Handle case like -p=value
                            StoreValue(argument, value_part);
                        }
                    }
                }
            }
        }
    }

    void ArgParser::HandlePositionalArguments(const std::vector<std::string>& args) {
        size_t arg_index = 0;

        for (auto& [name, argument] : arguments_) {
            if (argument.is_positional) {
                for (const auto& val : args) {
                    // ≈сли дл€ позиционного аргумента предусмотрено несколько значений
                    if (argument.is_multi_value) {
                        argument.string_values.push_back(val);
                        try {
                            argument.int_values.push_back(std::stoi(val));
                        }
                        catch (...) {
                            argument.int_values.push_back(0);
                        }
                        if (argument.int_store) {
                            try {
                                argument.int_store->push_back(std::stoi(val));
                            }
                            catch (...) {
                                argument.int_store->push_back(0);
                            }
                        }
                    }
                    else {
                        // ≈сли только одно значение
                        StoreValue(argument, val);
                        break;
                    }
                }
            }
        }
    }



    void ArgParser::StoreValue(Argument& arg, const std::string& value) {
        if (arg.is_flag) {
            arg.bool_value = true;
            if (arg.bool_store) {
                *arg.bool_store = true;
            }
        }
        else {
            if (arg.is_multi_value) {
                arg.string_values.push_back(value);
                try {
                    arg.int_values.push_back(std::stoi(value));
                }
                catch (...) {
                    // ≈сли преобразование в int не удалось, сохран€ем 0
                    arg.int_values.push_back(0);
                }

                if (arg.int_store) {
                    try {
                        arg.int_store->push_back(std::stoi(value));
                    }
                    catch (...) {
                        arg.int_store->push_back(0);
                    }
                }
            }
            else {
                arg.string_value = value;
                try {
                    arg.int_value = std::stoi(value);
                }
                catch (...) {
                    arg.int_value = 0;
                }

                if (arg.string_store) {
                    *arg.string_store = value;
                }
            }
        }
    }

    std::string ArgParser::GetStringValue(const std::string& name, size_t index) const {
        if (!IsArgumentExists(name)) {
            throw std::runtime_error("Argument not found: " + name);
        }

        const Argument& arg = arguments_.at(name);
        if (arg.is_multi_value) {
            if (index >= arg.string_values.size()) {
                throw std::runtime_error("Index out of range for argument: " + name);
            }
            return arg.string_values[index];
        }

        if (arg.string_value.empty() && arg.has_default) {
            return arg.default_value;
        }

        return arg.string_value;
    }

    int ArgParser::GetIntValue(const std::string& name, size_t index) const {
        if (!IsArgumentExists(name)) {
            throw std::runtime_error("Argument not found: " + name);
        }

        const Argument& arg = arguments_.at(name);
        if (arg.is_multi_value) {
            if (index >= arg.int_values.size()) {
                throw std::runtime_error("Index out of range for argument: " + name);
            }
            return arg.int_values[index];
        }

        // ≈сли значение не было установлено и есть значение по умолчанию
        if (arg.int_value == 0 && arg.has_default && !arg.string_value.empty()) {
            try {
                return std::stoi(arg.default_value);
            }
            catch (...) {
                return 0;
            }
        }

        return arg.int_value;
    }

    bool ArgParser::GetFlag(const std::string& name) const {
        if (!IsArgumentExists(name)) {
            throw std::runtime_error("Argument not found: " + name);
        }
        return arguments_.at(name).bool_value;
    }

    bool ArgParser::Help() const {
        return help_requested_;
    }

    std::string ArgParser::HelpDescription() const {
        std::stringstream ss;
        ss << name_ << "\n";

        // Find help description
        for (const auto& [name, arg] : arguments_) {
            if (arg.is_help && !arg.description.empty()) {
                ss << arg.description << "\n";
                break;
            }
        }

        ss << "\n";

        // Add arguments
        for (const auto& [name, arg] : arguments_) {
            if (arg.is_help) continue;

            if (arg.short_name != 0) {
                ss << "-" << arg.short_name << ", ";
            }
            else {
                ss << "    ";
            }

            ss << "--" << arg.long_name;

            if (!arg.is_flag) {
                ss << "=<" << (arg.int_store ? "int" : "string") << ">";
            }

            if (!arg.description.empty()) {
                ss << ",  " << arg.description;
            }

            if (arg.has_default) {
                ss << " [default = " << arg.default_value << "]";
            }

            if (arg.is_multi_value) {
                ss << " [repeated";
                if (arg.min_args_count > 0) {
                    ss << ", min args = " << arg.min_args_count;
                }
                ss << "]";
            }

            ss << "\n";
        }

        // Add help
        for (const auto& [name, arg] : arguments_) {
            if (arg.is_help) {
                ss << "\n";
                ss << "-" << arg.short_name << ", --" << arg.long_name << " Display this help and exit\n";
                break;
            }
        }

        return ss.str();
    }

    ArgParser::Argument& ArgParser::GetArgument(const std::string& name) {
        return arguments_[name];
    }

    bool ArgParser::IsArgumentExists(const std::string& name) const {
        return arguments_.find(name) != arguments_.end();
    }

    bool ArgParser::IsShortArgumentExists(char short_name) const {
        return short_to_long_.find(short_name) != short_to_long_.end();
    }

} // namespace ArgumentParser