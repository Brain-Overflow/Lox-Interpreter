//token.h
#pragma once

#include <string>
#include "token_type.h"
#include "value.h"

class Token {
    private:
        Token_Type type;
        std::string lexeme;
        Value literal;
        int line;

    public:
        Token(Token_Type type, std::string lexeme, Value literal, int line);

        std::string to_string() const;
};