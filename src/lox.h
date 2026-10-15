//lox.h
#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <filesystem>

class No_Input_Error : public std::runtime_error {
    public:
        No_Input_Error(const std::string& message) : std::runtime_error(message) {

        }
};

class IO_Error : public std::runtime_error {
    public:
        IO_Error(const std::string& message) : std::runtime_error(message) {

        }
};

class Data_Error : public std::runtime_error {
    public:
        Data_Error(const std::string& message) : std::runtime_error(message) {

        }
};

class Lox {
    private:
        static bool encountered_error;

        static void report(int line, std::string_view where, std::string_view message);
    
    public:
        static void error(int line, std::string_view message);

        static void run(std::string instructions);

        static void run_file(const std::filesystem::path& filepath);

        static void run_prompt();
};