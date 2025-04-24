#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace fs = std::filesystem;

struct Color {
    uint8_t b, g, r;
};

Color getColor(uint64_t sand) {
    if (sand == 0) return { 255, 255, 255 }; // white
    if (sand == 1) return { 0, 255, 0 };     // green
    if (sand == 2) return { 128, 0, 128 };   // purple
    if (sand == 3) return { 0, 255, 255 };   // yellow
    return { 0, 0, 0 };                      // black
}

void saveBMP(const std::vector<std::vector<uint64_t>>& grid, const std::string& filename) {
    uint32_t width = grid.empty() ? 0 : grid[0].size();
    uint32_t height = grid.size();
    uint32_t rowSize = (3 * width + 3) & ~3;
    uint32_t fileSize = 54 + rowSize * height;

    std::ofstream out(filename, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open file: " + filename);

    uint8_t header[54] = {
        'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0,
        40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0,
        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
    };

    memcpy(&header[2], &fileSize, 4);
    memcpy(&header[18], &width, 4);
    memcpy(&header[22], &height, 4);

    out.write(reinterpret_cast<char*>(header), 54);

    for (int y = height - 1; y >= 0; --y) {
        for (uint32_t x = 0; x < width; ++x) {
            Color color = getColor(grid[y][x]);
            out.write(reinterpret_cast<char*>(&color), 3);
        }
        uint8_t padding[3] = { 0 };
        out.write(reinterpret_cast<char*>(padding), rowSize - 3 * width);
    }
}

void topple(std::vector<std::vector<uint64_t>>& grid) {
    int height = grid.size();
    if (height == 0) return;
    int width = grid[0].size();

    std::vector<std::vector<uint64_t>> newGrid = grid;
    bool changed = false;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] >= 4) {
                newGrid[y][x] -= 4;
                if (y > 0) newGrid[y - 1][x]++;
                if (y < height - 1) newGrid[y + 1][x]++;
                if (x > 0) newGrid[y][x - 1]++;
                if (x < width - 1) newGrid[y][x + 1]++;
                changed = true;
            }
        }
    }

    if (changed) {
        grid = newGrid;
    }
}

bool isStable(const std::vector<std::vector<uint64_t>>& grid) {
    for (const auto& row : grid) {
        for (auto val : row) {
            if (val >= 4) return false;
        }
    }
    return true;
}

void loadInitialState(std::vector<std::vector<uint64_t>>& grid,
    const std::string& filename,
    uint16_t width, uint16_t height) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Cannot open input file: " + filename);

    int x, y;
    uint64_t count;
    while (in >> x >> y >> count) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            throw std::out_of_range("Coordinates out of grid bounds");
        }
        grid[y][x] = count;
    }
}

int main(int argc, char* argv[]) {
    try {
    
        uint16_t width = 0, height = 0;
        std::string input, output;
        uint64_t max_iter = 0, freq = 0;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if ((arg == "-w" || arg == "--width") && i + 1 < argc) {
                width = std::stoi(argv[++i]);
                if (width == 0) throw std::invalid_argument("Width cannot be zero");
            }
            else if ((arg == "-l" || arg == "--length") && i + 1 < argc) {
                height = std::stoi(argv[++i]);
                if (height == 0) throw std::invalid_argument("Height cannot be zero");
            }
            else if ((arg == "-i" || arg == "--input") && i + 1 < argc) input = argv[++i];
            else if ((arg == "-o" || arg == "--output") && i + 1 < argc) output = argv[++i];
            else if ((arg == "-m" || arg == "--max-iter") && i + 1 < argc) max_iter = std::stoull(argv[++i]);
            else if ((arg == "-f" || arg == "--freq") && i + 1 < argc) freq = std::stoull(argv[++i]);
        }

        if (width == 0 || height == 0 || input.empty() || output.empty() || max_iter == 0) {
            throw std::invalid_argument("Missing required arguments");
        }

        fs::create_directories(output);

        std::vector<std::vector<uint64_t>> grid(height, std::vector<uint64_t>(width, 0));
        loadInitialState(grid, input, width, height);

        for (uint64_t iter = 1; iter <= max_iter; ++iter) {
            topple(grid);

            if (freq > 0 && iter % freq == 0) {
                saveBMP(grid, output + "/step_" + std::to_string(iter) + ".bmp");
            }

            if (isStable(grid)) {
                std::cout << "Stabilized after " << iter << " iterations\n";
                break;
            }
        }

        saveBMP(grid, output + "/final.bmp");

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
