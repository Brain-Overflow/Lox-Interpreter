//scanner.h
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "token.h"

class Scanner {
    private:
        std::string source;
        std::vector<Token> tokens;
        std::size_t start = 0;
        std::size_t current = 0;
        int line = 1;

        static const std::unordered_map<std::string, Token_Type> keywords;

        bool is_finished() const;    
        
        void scan_token();

        void identifier();

        void number();

        void string();

        void block_comment();

        char advance();

        std::string advance(std::size_t character_count);

        void add_token(Token_Type type);

        void add_token(Token_Type type, Value literal);

        bool match(char expected_character);

        char peek() const;

        char peek_next() const;

        static bool is_alpha(char character);

        static bool is_digit(char character);

        static bool is_alphanumeric(char character);

    public:
        std::vector<Token> scan_tokens();

        explicit Scanner(std::string source);
};