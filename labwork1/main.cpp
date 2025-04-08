/**
 * @file WordCount.cpp
 * @brief Утилита для подсчета строк, слов, байт и символов в файле.
 *
 * Аналог команды `wc` в Unix. Поддерживает аргументы командной строки:
 * -l (lines), -w (words), -c (bytes), -m (chars), а также --help.
 *
 * Реализует основные операции с файловыми потоками, парсинг аргументов и подсчет статистики.
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>

 /**
  * @struct FileStats
  * @brief Структура для хранения статистики по файлу.
  */
struct FileStats {
    size_t lines = 0;          ///< Количество строк
    size_t words = 0;          ///< Количество слов
    size_t bytes = 0;          ///< Количество байт
    size_t chars = 0;          ///< Количество символов
    std::string filename;      ///< Имя файла
};

/// @brief Сообщение помощи, выводимое при использовании --help
const std::string kHelpMessage =
"Usage: WordCount [OPTION]... [FILE]...\n"
"Print line, word, byte, and character counts for each FILE.\n\n"
"Options:\n"
"  -l, --lines    print the line counts\n"
"  -w, --words    print the word counts\n"
"  -c, --bytes    print the byte counts\n"
"  -m, --chars    print the character counts\n"
"  --help         display this help and exit\n";

/**
 * @brief Подсчитывает строки, слова, байты и символы в заданном файле.
 *
 * @param filename Имя файла для анализа.
 * @return FileStats Структура с результатами подсчета.
 */
FileStats CountFileStats(const std::string& filename) {
    FileStats stats;
    stats.filename = filename;

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "WordCount: " << filename << ": No such file or directory\n";
        return stats;
    }

    char ch;
    char prev = '\0';
    bool in_word = false;

    while (file.get(ch)) {
        stats.bytes++;
        stats.chars++;

        if (ch == '\n') {
            if (prev != '\r') {
                stats.lines++;
            }
        }
        else if (ch == '\r') {
            stats.lines++;
        }

        if (isspace(static_cast<unsigned char>(ch))) {
            if (in_word) {
                stats.words++;
                in_word = false;
            }
        }
        else {
            in_word = true;
        }

        prev = ch;
    }

    if (in_word) {
        stats.words++;
    }

    if (stats.bytes > 0 && prev != '\n' && prev != '\r') {
        stats.lines++;
    }

    return stats;
}

/**
 * @brief Печатает статистику по файлу в соответствии с выбранными флагами.
 *
 * @param stats Статистика файла.
 * @param show_lines Выводить ли количество строк.
 * @param show_words Выводить ли количество слов.
 * @param show_bytes Выводить ли количество байт.
 * @param show_chars Выводить ли количество символов.
 * @param show_filename Выводить ли имя файла.
 */
void PrintStats(const FileStats& stats, bool show_lines, bool show_words,
    bool show_bytes, bool show_chars, bool show_filename) {
    std::cout << std::setw(10);
    if (show_lines)  std::cout << "Lines: " << std::setw(6) << stats.lines << "  ";
    if (show_words)  std::cout << "Words: " << std::setw(6) << stats.words << "  ";
    if (show_bytes)  std::cout << "Bytes: " << std::setw(6) << stats.bytes << "  ";
    if (show_chars)  std::cout << "Chars: " << std::setw(6) << stats.chars << "  ";
    if (show_filename) std::cout << "File: " << stats.filename;
    std::cout << "\n";
}

/**
 * @brief Точка входа в программу.
 *
 * Парсит аргументы командной строки, вызывает подсчет статистики и выводит результат.
 *
 * @param argc Количество аргументов.
 * @param argv Массив аргументов.
 * @return int Код завершения (0 — успешно, 1 — ошибка).
 */
int main(int argc, char** argv) {
    std::vector<std::string> filenames;
    bool show_lines = false;
    bool show_words = false;
    bool show_bytes = false;
    bool show_chars = false;
    bool default_mode = true;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            std::cout << kHelpMessage;
            return 0;
        }
        else if (arg == "-l" || arg == "--lines") {
            show_lines = true;
            default_mode = false;
        }
        else if (arg == "-w" || arg == "--words") {
            show_words = true;
            default_mode = false;
        }
        else if (arg == "-c" || arg == "--bytes") {
            show_bytes = true;
            default_mode = false;
        }
        else if (arg == "-m" || arg == "--chars") {
            show_chars = true;
            default_mode = false;
        }
        else if (arg[0] == '-') {
            // Обработка коротких флагов (например, -lwc)
            for (size_t j = 1; j < arg.size(); ++j) {
                switch (arg[j]) {
                case 'l': show_lines = true; default_mode = false; break;
                case 'w': show_words = true; default_mode = false; break;
                case 'c': show_bytes = true; default_mode = false; break;
                case 'm': show_chars = true; default_mode = false; break;
                default:
                    std::cerr << "WordCount: invalid option -- '" << arg[j] << "'\n";
                    return 1;
                }
            }
        }
        else {
            filenames.push_back(arg);
        }
    }

    if (default_mode) {
        show_lines = true;
        show_words = true;
        show_bytes = true;
    }

    if (filenames.empty()) {
        std::cerr << kHelpMessage;
        return 1;
    }

    for (const auto& filename : filenames) {
        FileStats stats = CountFileStats(filename);
        PrintStats(stats, show_lines, show_words, show_bytes, show_chars, filenames.size() > 1);
    }

    return 0;
}
