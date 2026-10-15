//lox.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <format>
#include <string_view>
#include "lox.h"
#include "token.h"
#include "scanner.h"

bool Lox::encountered_error = false;

void Lox::report(int line, std::string_view where, std::string_view message) {
    std::cerr << std::format("[Line {}] Error{}: {}\n", line, where, message);

    encountered_error = true;
}

void Lox::error(int line, std::string_view message) {
    report(line, "", message);
}

void Lox::run(std::string instructions) {
    Scanner scanner(std::move(instructions));
    std::vector<Token> tokens = scanner.scan_tokens();

    for (Token& token : tokens) {
        std::cout << std::format("{}\n", token.to_string());
    }
}

void Lox::run_file(const std::filesystem::path& filepath) {
    std::ifstream file(filepath);

    if (!file) {
        throw No_Input_Error(std::format("Error: Could not open \"{}\"!\n", filepath.string()));
    }

    const auto file_size = std::filesystem::file_size(filepath);

    std::string file_contents(file_size, '\0');

    file.read(file_contents.data(), file_size);

    if (!file) {
        throw IO_Error(std::format("Error: Could not read the entirety of \"{}\"!\nError: Read {} bytes, expected {} bytes!\n", filepath.string(), file.gcount(), file_size));
    }

    run(std::move(file_contents));

    if (encountered_error) {
        throw Data_Error("");
    }
}

void Lox::run_prompt() {
    std::string line;

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            break;
        }

        run(std::move(line));

        encountered_error = false;
    }
}