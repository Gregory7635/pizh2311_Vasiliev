#include "parser.h"

#include <fstream>
#include <iostream>
#include <string_view>
#include <stack>
#include <algorithm>
#include <sstream>
#include <string>
#include <cctype>

using std::string;
using std::string_view;
using std::stringstream;
using std::ifstream;
using std::ofstream;

omfl::OMFL omfl::parse(const std::filesystem::path& path) {
    std::ifstream file;
    file.open(path);
    std::stringstream in_file;
    in_file << file.rdbuf();

    return parse(in_file.str());
}

omfl::OMFL omfl::parse(const std::string& str) {
    OMFL parsed;
    std::stringstream data;
    data << str;
    std::string line;
    std::string working_section;
    while (!data.eof()) {
        std::getline(data, line);
        std::string_view line_view = line;
        DeleteTrash(line_view);
        if (!line_view.empty()) {
            if (line_view[0] == '[' && line_view.back() == ']') {
                working_section = line_view.substr(1, line_view.size() - 2);
                parsed.CheckSection(working_section);
            }
            else {
                Unit my_unit = parsed.KeyWork(line_view);
                std::string s_value{ line_view };
                size_t index = std::min(s_value.find(' '), s_value.find('='));
                std::string key_ans_section;
                if (!working_section.empty()) {
                    key_ans_section = working_section + "." + s_value.substr(0, index);;
                }
                else {
                    key_ans_section = s_value.substr(0, index);
                }
                parsed.PlaceUnit(key_ans_section, my_unit);
            }
        }
    }
    return parsed;
}

bool omfl::OMFL::valid() const {
    return valid_;
}

bool omfl::Section::PlaceUnit(std::string& directory, const Unit& unit_value, omfl::Section& current_section) {
    size_t dot_position = directory.find('.');

    if (dot_position != std::string::npos) {
        std::string current_name = directory.substr(0, dot_position);
        std::string later = directory.substr(dot_position + 1);

        if (current_section.sections.count(current_name) && current_section.sections[current_name].is_key) {
            return false;
        }
        if (current_section.sections.count(current_name)) {
            bool correct = PlaceUnit(later, unit_value, current_section.sections[current_name]);

            return correct && CheckNameS(current_name);
        }
        else {
            omfl::Section new_section;
            bool is_new_key_correct = PlaceUnit(later, unit_value, new_section);
            current_section.sections[current_name] = new_section;

            return is_new_key_correct && CheckNameS(current_name);
        }

    }
    else {
        if (current_section.sections.count(directory)) {
            return false;
        }
        Section final_section;
        final_section.is_key = true;
        final_section.unit = unit_value;
        current_section.sections[directory] = final_section;

        return CheckNameS(directory);
    }
}


void omfl::OMFL::PlaceUnit(std::string& key, const Unit& unit_value) {
    valid_ &= all_sections.PlaceUnit(key, unit_value, all_sections);
}

Unit omfl::OMFL::KeyWork(std::string_view& line) {
    DeleteTrash(line);
    if (line.empty()) {
        valid_ = false;
        return Unit();
    }

    size_t index = line.find('=');
    if (index == std::string_view::npos) {
        valid_ = false;
        return Unit();
    }

    // Validate key part
    std::string_view key_part = line.substr(0, index);
    DeleteTrash(key_part);
    CheckName(key_part);

    // Skip '=' and spaces
    index = line.find_first_not_of(' ', index + 1);
    if (index == std::string_view::npos) {
        valid_ = false;
        return Unit();
    }

    // Process value part
    std::string_view value = line.substr(index);
    Unit unit;
    int quotes = 0;
    std::vector<std::pair<size_t, size_t>> brackets;
    std::stack<size_t> tmp;
    for (int i = 0; i < value.length(); ++i) {
        if (value[i] == '"') {
            quotes++;
        }
        else if (value[i] == '[' && quotes % 2 == 0) {
            tmp.push(i);
        }
        else if (value[i] == ']' && quotes % 2 == 0) {
            if (tmp.empty()) {
                valid_ = false;
            }
            else {
                brackets.emplace_back(tmp.top(), i);
                tmp.pop();
            }
        }
    }
    std::sort(brackets.begin(), brackets.end());
    if (quotes % 2 != 0) {
        valid_ = false;
    }
    if (!tmp.empty()) {
        valid_ = false;
    }
    FillUnit(unit, value, brackets, -1);
    return unit;
}

void omfl::OMFL::FillUnit(Unit& unit, std::string_view& line,
    std::vector<std::pair<size_t, size_t>>& brackets, int bracket) {
    DeleteTrash(line);
    if (line.empty()) {
        valid_ = false;
        return;
    }

    if (line[0] == '[' && line.back() == ']') {
        unit.what = kArray;
        std::string_view value = line.substr(1, line.size() - 2);
        FillUnitArray(unit, value, brackets, 0);
    }
    else {
        FillUnitValue(unit, line, brackets, 0);
    }
}

void omfl::OMFL::FillUnitValue(Unit& unit, std::string_view& value, std::vector<std::pair<size_t, size_t>>& brackets,
    int bracket) {
    if (!value.empty()) {
        if (value == "false" || value == "true") {
            unit.what = kBool;
            unit.bool_ = (value == "true");
        }
        else if (value[0] == '"' && value.back() == '"' && std::count(value.begin(), value.end(), '"') == 2) {
            unit.what = kString;
            unit.string_ = value.substr(1, value.size() - 2);
        }
        else if (BoolIntNumber(value)) {
            unit.what = kInt;
            std::string s_value{ value };
            unit.int_ = std::stoi(s_value);
        }
        else if (BoolFloatNumber(value)) {
            unit.what = kFloat;
            std::string s_value{ value };
            unit.float_ = std::stof(s_value);
        }
        else if (value[0] == '[' && value.back() == ']') {
            if (valid_) {
                unit.what = kArray;
            }
            std::string_view line = value.substr(1, value.size() - 2);
            bracket++;
            FillUnitArray(unit, line, brackets, bracket);
        }
        else {
            valid_ = false;
        }
    }
    else if (unit.what != kArray) {
        valid_ = false;
    }
}

void omfl::OMFL::FillUnitArray(Unit& unit, std::string_view& line, std::vector<std::pair<size_t, size_t>>& brackets, int bracket) {
    unit.what = kArray;
    size_t start = 0;
    int depth = 0;
    bool in_string = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"' && (i == 0 || line[i - 1] != '\\')) {
            in_string = !in_string;
        }

        if (!in_string) {
            if (c == '[') depth++;
            if (c == ']') depth--;

            if (c == ',' && depth == 0) {
                std::string_view element = line.substr(start, i - start);
                start = i + 1;
                ProcessArrayElement(unit, element, brackets);
            }
        }
    }

    // Process last element
    if (start < line.size()) {
        std::string_view element = line.substr(start);
        ProcessArrayElement(unit, element, brackets);
    }
}

void omfl::OMFL::ProcessArrayElement(Unit& unit, std::string_view element, std::vector<std::pair<size_t, size_t>>& brackets) {
    DeleteTrash(element);
    if (!element.empty()) {
        unit.array_.emplace_back();
        if (element[0] == '[' && element.back() == ']') {
            std::string_view inner = element.substr(1, element.size() - 2);
            unit.array_.back().what = kArray;
            FillUnitArray(unit.array_.back(), inner, brackets, 0);
        }
        else {
            FillUnitValue(unit.array_.back(), element, brackets, 0);
        }
    }
}

bool omfl::BoolIntNumber(std::string_view& line) {
    if (line[0] != '-' && line[0] != '+' && !std::isdigit(line[0])) {
        return false;
    }
    if (line == "-" || line == "+") {
        return false;
    }
    int index = 0;
    if (line[0] == '-' || line[0] == '+') {
        index++;
    }
    return line.find_first_not_of(kDigits, index) == std::string::npos;
}

bool omfl::BoolFloatNumber(std::string_view& line) {
    if (std::count(line.begin(), line.end(), '.') != 1) {
        return false;
    }
    size_t dot_index = line.find('.');
    if (dot_index == 0 || dot_index == line.size() - 1) {
        return false;
    }
    std::string_view sub_line = line.substr(0, dot_index);

    return (BoolIntNumber(sub_line) &&
        line.find_first_not_of(kDigits, dot_index + 1) == std::string::npos);
}

const omfl::Section& omfl::OMFL::Get(const std::string& directory) const {
    return all_sections.Get(directory);
}

const omfl::Section& omfl::Section::Get(const std::string& directory) const {
    size_t position_of_dot = directory.find('.');
    if (position_of_dot != std::string::npos) {
        std::string working_directory = directory.substr(0, position_of_dot);
        return sections.find(working_directory)->second.Get(directory.substr(position_of_dot + 1));
    }
    else {
        return sections.find(directory)->second;
    }
}

const Unit& omfl::Section::operator[](size_t index) const {
    return unit[index];
}

bool omfl::Section::IsInt() const {
    return unit.IsInt();
}

bool omfl::Section::IsFloat() const {
    return unit.IsFloat();
}

bool omfl::Section::IsString() const {
    return unit.IsString();
}

bool omfl::Section::IsBool() const {
    return unit.IsBool();
}

bool omfl::Section::IsArray() const {
    return unit.IsArray();
}

int omfl::Section::AsInt() const {
    return unit.AsInt();
}

float omfl::Section::AsFloat() const {
    return unit.AsFloat();
}

const std::string& omfl::Section::AsString() const {
    return unit.AsString();
}

bool omfl::Section::AsBool() const {
    return unit.AsBool();
}

int omfl::Section::AsIntOrDefault(int number) const {
    return unit.AsIntOrDefault(number);
}

float omfl::Section::AsFloatOrDefault(float number) const {
    return unit.AsFloatOrDefault(number);
}

const std::string omfl::Section::AsStringOrDefault(const std::string& line) const {
    return unit.AsStringOrDefault(line);
}

bool Unit::IsInt() const {
    return what == kInt;
}

bool Unit::IsFloat() const {
    return what == kFloat;
}

bool Unit::IsString() const {
    return what == kString;
}

bool Unit::IsBool() const {
    return what == kBool;
}

bool Unit::IsArray() const {
    return what == kArray;
}

int Unit::AsInt() const {
    return int_;
}

float Unit::AsFloat() const {
    return float_;
}

const std::string& Unit::AsString() const {
    return string_;
}

bool Unit::AsBool() const {
    return bool_;
}

int Unit::AsIntOrDefault(int number) const {
    if (what == kInt) {
        return int_;
    }
    return number;
}

float Unit::AsFloatOrDefault(float number) const {
    if (what == kFloat) {
        return float_;
    }
    return number;
}

const std::string Unit::AsStringOrDefault(const std::string& line) const {
    if (what == kString) {
        return string_;
    }
    return line;
}


bool omfl::CheckNameS(const std::string_view& line) {
    if (line.empty()) return false;
    
    if (!isalnum(line[0]) && line[0] != '_') {
        return false;
    }

    for (char c : line) {
        if (!isalnum(c) && c != '_' && c != '-') {
            return false;
        }
    }
    return true;
}

void omfl::OMFL::CheckName(const std::string_view& line) {
    valid_ &= CheckNameS(line);
}

void omfl::OMFL::CheckSection(const std::string_view& line) {
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos) {
        valid_ = false;
        return;
    }

    // Check for invalid characters in section name
    if (line.find(' ') != std::string::npos ||
        line.find('"') != std::string::npos ||
        line.find('\'') != std::string::npos ||
        line.find('[') != std::string::npos ||
        line.find(']') != std::string::npos) {
        valid_ = false;
        return;
    }

    // Check for leading/trailing dots or consecutive dots
    if (line.front() == '.' || line.back() == '.' ||
        line.find("..") != std::string::npos) {
        valid_ = false;
        return;
    }

    // Validate each part of the section path
    size_t start = 0;
    size_t end = line.find('.');
    while (end != std::string::npos) {
        std::string_view part = line.substr(start, end - start);
        if (!CheckNameS(part)) {
            valid_ = false;
            return;
        }
        start = end + 1;
        end = line.find('.', start);
    }

    // Check last part
    std::string_view last_part = line.substr(start);
    if (!CheckNameS(last_part)) {
        valid_ = false;
    }
}

std::string_view& omfl::DeleteTrash(std::string_view& line) {
    // First remove comments
    size_t comment_pos = line.find('#');
    if (comment_pos != std::string_view::npos) {
        line = line.substr(0, comment_pos);
    }

    // Then trim whitespace
    size_t start = line.find_first_not_of(" \t\r\n");
    if (start == std::string_view::npos) {
        line = "";
        return line;
    }

    size_t end = line.find_last_not_of(" \t\r\n");
    line = line.substr(start, end - start + 1);

    return line;
}

size_t omfl::FindComma(std::string_view& line, size_t index) {
    int quote_counter = 0;
    for (size_t i = index; i < line.size(); ++i) {
        if (line[i] == '"') {
            quote_counter++;
        }
        else if (line[i] == ',' && quote_counter % 2 == 0) {
            return i;
        }
    }
    return std::string::npos;
}

const Unit& Unit::operator[](size_t index) const {
    if (array_.size() <= index) {
        return kEmptyUnit;
    }
    return array_[index];
}

// xml

void omfl::OMFL::ToXml(const std::string& name) {
    std::ofstream file;
    file.open(name + ".xml");
    int tabulation = 0;
    file << "<" + kRoot + ">\n";
    FillXml(all_sections, file, tabulation + kSpacesInTabulation);
    file << "</" + kRoot + ">";
}

void omfl::OMFL::FillXml(const omfl::Section& section, std::ofstream& file, int tabulation) {
    std::string tab = Tabulation(tabulation);
    for (auto [key, value] : section.sections) {
        if (value.is_key) {
            UnitValueXml(value.unit, file, tabulation, key);
        }
        else {
            file << tab << "<" + key + ">" + "\n";
            FillXml(value, file, tabulation + kSpacesInTabulation);
            file << tab << "</" + key + ">\n";
        }
    }
}

void omfl::OMFL::ArrayXml(const Unit& unit, std::ofstream& file, int tabulation) {
    std::string tab = Tabulation(tabulation);
    for (const auto& i : unit.array_) {
        UnitValueXml(i, file, tabulation + kSpacesInTabulation, kIndexName);
    }
}

void omfl::OMFL::UnitValueXml(const Unit& unit, std::ofstream& file, int tabulation, const std::string& key) {
    std::string tab = Tabulation(tabulation);
    if (unit.IsArray()) {
        file << tab + "<" + kArrayName + "_" + key + ">\n";
        ArrayXml(unit, file, tabulation + kSpacesInTabulation);
        file << tab + "</" + kArrayName + "_" + key + ">\n";
    }
    else {
        file << tab + "<" + key + ">";
        switch (unit.what) {
        case kBool:
            file << unit.bool_;
            break;
        case kInt:
            file << unit.int_;
            break;
        case kFloat:
            file << unit.float_;
            break;
        case kString:
            file << unit.string_;
            break;
        default:
            break;
        }
        file << "</" + key + ">\n";
    }
}

void omfl::OMFL::ToJson(const std::string& name) {
    std::ofstream file;
    file.open(name + ".json");
    int tabulation = 0;
    file << "{\n";
    FillJson(all_sections, file, tabulation + kSpacesInTabulation);
    file << "\n}";
}

void omfl::OMFL::FillJson(const omfl::Section& section, std::ofstream& file, int tabulation) {
    std::string tab = Tabulation(tabulation);
    size_t count = section.sections.size();
    for (auto [key, value] : section.sections) {
        count--;
        if (value.is_key) {
            file << tab << '"' + key + '"' + ": ";
            UnitValueJson(value.unit, file, tabulation, key);
            if (count > 0) {
                file << ",\n";
            }
        }
        else {
            file << tab << '"' + key + '"' + ": {\n";
            FillJson(value, file, tabulation + kSpacesInTabulation);
            file << "\n" + tab + "}";
            if (count > 0) {
                file << ",\n";
            }
        }
    }
}

void omfl::OMFL::UnitValueJson(const Unit& unit, std::ofstream& file, int tabulation, const std::string& key) {
    if (unit.IsArray()) {
        file << "[ ";
        ArrayJson(unit, file, tabulation);
        file << " ]";
    }
    else {
        switch (unit.what) {
        case kBool:
            file << unit.bool_;
            break;
        case kInt:
            file << unit.int_;
            break;
        case kFloat:
            file << unit.float_;
            break;
        case kString:
            file << '"' + unit.string_ + '"';
            break;
        default:
            break;
        }
    }
}

void omfl::OMFL::ArrayJson(const Unit& unit, std::ofstream& file, int tabulation) {
    for (int i = 0; i < unit.array_.size() - 1; ++i) {
        UnitValueJson(unit.array_[i], file, tabulation + kSpacesInTabulation, kIndexName);
        file << ", ";
    }
    UnitValueJson(unit.array_.back(), file, tabulation + kSpacesInTabulation, kIndexName);
}

void omfl::OMFL::ToYaml(const std::string& name) {
    std::ofstream file;
    file.open(name + ".yaml");
    int tabulation = 0;
    FillYaml(all_sections, file, tabulation);
}

void omfl::OMFL::FillYaml(const omfl::Section& section, std::ofstream& file, int tabulation) {
    std::string tab = Tabulation(tabulation);
    for (auto [key, value] : section.sections) {
        if (value.is_key) {
            file << tab + key + ": ";
            UnitValueYaml(value.unit, file, tabulation, key);
        }
        else {
            file << tab << "- " + key + ":\n";
            FillYaml(value, file, tabulation + kSpacesInTabulation);
        }
        file << "\n";
    }
}

void omfl::OMFL::UnitValueYaml(const Unit& unit, std::ofstream& file, int tabulation, const std::string& key) {
    if (unit.IsArray()) {
        file << "[ ";
        ArrayYaml(unit, file, tabulation);
        file << " ]";
    }
    else {
        switch (unit.what) {
        case kBool:
            file << unit.bool_;
            break;
        case kInt:
            file << unit.int_;
            break;
        case kFloat:
            file << unit.float_;
            break;
        case kString:
            file << '"' + unit.string_ + '"';
            break;
        default:
            break;
        }
    }
}

void omfl::OMFL::ArrayYaml(const Unit& unit, std::ofstream& file, int tabulation) {
    for (int i = 0; i < unit.array_.size() - 1; ++i) {
        UnitValueYaml(unit.array_[i], file, tabulation + kSpacesInTabulation, kIndexName);
        file << ", ";
    }
    UnitValueYaml(unit.array_.back(), file, tabulation + kSpacesInTabulation, kIndexName);
}

std::string omfl::Tabulation(int size) {
    std::string tab;
    for (int i = 0; i < size; ++i) {
        tab += " ";
    }
    return tab;
}