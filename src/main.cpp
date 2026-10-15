//main.cpp
#include <iostream>
#include <filesystem>
#include "lox.h"

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: Lox <filepath>\n";
        return 64;
    }
    else if (argc == 2) {
        try {
            Lox::run_file(argv[1]);
        }
        catch (const No_Input_Error& exception) {
            std::cerr << exception.what();
            return 66;
        }
        catch (const IO_Error& exception) {
            std::cerr << exception.what();
            return 74;
        }
        catch (const Data_Error& exception) {
            std::cerr << exception.what();
            return 65;
        }
        catch (const std::filesystem::filesystem_error& exception) {
            std::cerr << exception.what();
            return 74;
        }
        catch (const std::exception& exception) {
            std::cerr << exception.what();
            return 70;
        }
    }
    else if (argc == 1) {
        Lox::run_prompt();
    }

    return 0;
}