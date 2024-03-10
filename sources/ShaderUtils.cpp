#include "../headers/ShaderUtils.hpp"
#include <iostream>
#include <fstream>

std::string readShaderSourceFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string shaderSource;
    std::string line;
    while (std::getline(file, line)) {
        shaderSource += line + "\n";
    }

    file.close();
    return shaderSource;
}
