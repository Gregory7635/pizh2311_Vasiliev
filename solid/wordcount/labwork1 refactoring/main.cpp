#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <memory>
#include <sstream>



struct FileStats {
    size_t lines = 0;
    size_t words = 0;
    size_t bytes = 0;
    size_t chars = 0;
    std::string filename;
};

// Interfaces (ISP)
class IFileReader {
public:
    virtual std::vector<char> read(const std::string& path) = 0;
    virtual ~IFileReader() = default;
};

class IStatCounter {
public:
    virtual FileStats count(const std::vector<char>& content, const std::string& filename) = 0;
    virtual ~IStatCounter() = default;
};

class IFormatter {
public:
    virtual std::string format(const FileStats& stats,
        bool showLines,
        bool showWords,
        bool showBytes,
        bool showChars,
        bool showFilename) = 0;
    virtual ~IFormatter() = default;
};

class IPrinter {
public:
    virtual void print(const std::string& output) = 0;
    virtual ~IPrinter() = default;
};

// Concrete implementations
class FileReader : public IFileReader {
public:
    std::vector<char> read(const std::string& path) override {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }
        return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    }
};

class FileStatsCounter : public IStatCounter {
public:
    FileStats count(const std::vector<char>& content, const std::string& filename) override {
        FileStats stats;
        stats.filename = filename;
        bool inWord = false;
        char prev = '\0';
        for (char ch : content) {
            stats.bytes++;
            stats.chars++;
            if (ch == '\n') {
                if (prev != '\r') stats.lines++;
            }
            else if (ch == '\r') {
                stats.lines++;
            }
            if (std::isspace(static_cast<unsigned char>(ch))) {
                if (inWord) { stats.words++; inWord = false; }
            }
            else {
                inWord = true;
            }
            prev = ch;
        }
        if (inWord) stats.words++;
        if (stats.bytes > 0 && prev != '\n' && prev != '\r') stats.lines++;
        return stats;
    }
};

class ConsoleFormatter : public IFormatter {
public:
    std::string format(const FileStats& stats,
        bool showLines,
        bool showWords,
        bool showBytes,
        bool showChars,
        bool showFilename) override {
        std::ostringstream oss;
        oss << std::setw(10);
        if (showLines)  oss << "Lines: " << std::setw(6) << stats.lines << "  ";
        if (showWords)  oss << "Words: " << std::setw(6) << stats.words << "  ";
        if (showBytes)  oss << "Bytes: " << std::setw(6) << stats.bytes << "  ";
        if (showChars)  oss << "Chars: " << std::setw(6) << stats.chars << "  ";
        if (showFilename) oss << "File: " << stats.filename;
        return oss.str();
    }
};

class ConsolePrinter : public IPrinter {
public:
    void print(const std::string& output) override {
        std::cout << output << std::endl;
    }
};

class FilePrinter : public IPrinter {
public:
    void print(const std::string& output) override {
        std::ofstream("output.txt", std::ios::app) << output;
    }
};

// Argument parsing (SRP)
struct ProgramOptions {
    std::vector<std::string> files;
    bool showLines{ false }, showWords{ false }, showBytes{ false }, showChars{ false };
};

ProgramOptions parseArgs(int argc, char* argv[], const std::string& helpMessage) {
    ProgramOptions opts;
    bool defaultMode = true;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            std::cout << helpMessage << std::endl;
            std::exit(0);
        }
        else if (arg == "-l" || arg == "--lines") {
            opts.showLines = true; defaultMode = false;
        }
        else if (arg == "-w" || arg == "--words") {
            opts.showWords = true; defaultMode = false;
        }
        else if (arg == "-c" || arg == "--bytes") {
            opts.showBytes = true; defaultMode = false;
        }
        else if (arg == "-m" || arg == "--chars") {
            opts.showChars = true; defaultMode = false;
        }
        else if (arg.rfind("-", 0) != std::string::npos) {
            for (size_t j = 1; j < arg.size(); ++j) {
                switch (arg[j]) {
                case 'l': opts.showLines = true; defaultMode = false; break;
                case 'w': opts.showWords = true; defaultMode = false; break;
                case 'c': opts.showBytes = true; defaultMode = false; break;
                case 'm': opts.showChars = true; defaultMode = false; break;
                default:
                    std::cerr << "Invalid option: " << arg[j] << std::endl;
                    std::exit(1);
                }
            }
        }
        else {
            opts.files.push_back(arg);
        }
    }
    if (defaultMode) opts.showLines = opts.showWords = opts.showBytes = true;
    if (opts.files.empty()) {
        std::cerr << helpMessage << std::endl;
        std::exit(1);
    }
    return opts;
}

int main(int argc, char* argv[]) {
    const std::string helpMsg =
        "Usage: WordCount [OPTION]... [FILE]...\n"
        "Print line, word, byte, and character counts for each FILE.\n\n"
        "Options:\n"
        "  -l, --lines    print the line counts\n"
        "  -w, --words    print the word counts\n"
        "  -c, --bytes    print the byte counts\n"
        "  -m, --chars    print the character counts\n"
        "  --help         display this help and exit\n";

    // Parse arguments
    ProgramOptions opts = parseArgs(argc, argv, helpMsg);

    // Create components
    std::unique_ptr<IFileReader>    reader = std::make_unique<FileReader>();
    std::unique_ptr<IStatCounter>   counter = std::make_unique<FileStatsCounter>();
    std::unique_ptr<IFormatter>     formatter = std::make_unique<ConsoleFormatter>();
    std::unique_ptr<IPrinter>       printer = std::make_unique<FilePrinter>();

    // Process each file
    for (const auto& file : opts.files) {
        try {
            auto content = reader->read(file);
            FileStats stats = counter->count(content, file);
            std::string outStr = formatter->format(stats,
                opts.showLines,
                opts.showWords,
                opts.showBytes,
                opts.showChars,
                opts.files.size() > 1);
            printer->print(outStr);
        }
        catch (const std::exception& ex) {
            std::cerr << "Error processing " << file << ": " << ex.what() << std::endl;
        }
    }
    return 0;
}
