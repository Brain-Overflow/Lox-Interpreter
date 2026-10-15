//token.cpp
#include <string>
#include <format>
#include "token.h"
#include "token_type.h"
#include "value.h"

Token::Token(Token_Type type, std::string lexeme, Value literal, int line) : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {

}

std::string Token::to_string() const {
    return std::format("{} {} {}", ::to_string(type), lexeme, ::to_string(literal));
}