//scanner.cpp
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include "scanner.h"
#include "token.h"
#include "lox.h"

const std::unordered_map<std::string, Token_Type> Scanner::keywords = {
    {"and",    Token_Type::AND},
    {"class",  Token_Type::CLASS},
    {"else",   Token_Type::ELSE},
    {"false",  Token_Type::FALSE},
    {"for",    Token_Type::FOR},
    {"fun",    Token_Type::FUN},
    {"if",     Token_Type::IF},
    {"nil",    Token_Type::NIL},
    {"or",     Token_Type::OR},
    {"print",  Token_Type::PRINT},
    {"return", Token_Type::RETURN},
    {"super",  Token_Type::SUPER},
    {"this",   Token_Type::THIS},
    {"true",   Token_Type::TRUE},
    {"var",    Token_Type::VAR},
    {"while",  Token_Type::WHILE}
};

bool Scanner::is_finished() const {
    return current >= source.size();
} 

void Scanner::scan_token() {
    char character = advance();

    switch (character) {
        case '(': add_token(Token_Type::LEFT_PAREN); break;
        case ')': add_token(Token_Type::RIGHT_PAREN); break;
        case '{': add_token(Token_Type::LEFT_BRACE); break;
        case '}': add_token(Token_Type::RIGHT_BRACE); break;
        case ',': add_token(Token_Type::COMMA); break;
        case '.': add_token(Token_Type::DOT); break;
        case '-': add_token(Token_Type::MINUS); break;
        case '+': add_token(Token_Type::PLUS); break;
        case ';': add_token(Token_Type::SEMICOLON); break;
        case '*': add_token(Token_Type::STAR); break;  

        case '!':
            add_token(match('=') ? Token_Type::BANG_EQUAL : Token_Type::BANG);
            break;
        case '=':
            add_token(match('=') ? Token_Type::EQUAL_EQUAL : Token_Type::EQUAL);
            break;
        case '<':
            add_token(match('=') ? Token_Type::LESS_EQUAL : Token_Type::LESS);
            break;
        case '>':
            add_token(match('=') ? Token_Type::GREATER_EQUAL : Token_Type::GREATER);
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !is_finished()) {
                    advance();
                }
            }
            else if (match('*')) {
                block_comment();
            }
            else {
                add_token(Token_Type::SLASH);
            }
            break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            ++line;
            break;

        case '"':
            string();
            break;

        default:
            if (is_digit(character)) {
                number();
            }
            else if (is_alpha(character)) {
                identifier();
            }
            else {
                Lox::error(line, "Unexpected character.");
            }
            
            break;
    }
}

void Scanner::identifier() {
    while (is_alphanumeric(peek())) {
        advance();
    }

    std::string identifier = source.substr(start, current - start);

    auto iterator = keywords.find(identifier);

    if (iterator != keywords.end()) {
        add_token(iterator->second);
    }
    else {
        add_token(Token_Type::IDENTIFIER);
    } 
}

void Scanner::number() {
    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.' && is_digit(peek_next())) {
        advance();

        while (is_digit(peek())) {
            advance();
        }
    }

    add_token(Token_Type::NUMBER, std::stod(source.substr(start, current - start)));
}

void Scanner::string() {
    while (peek() != '"' && !is_finished()) {
        if (peek() == '\n') {
            ++line;
        }
        advance();
    }

    if (is_finished()) {
        Lox::error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string string = source.substr(start + 1, (current - 1) - (start + 1));
    add_token(Token_Type::STRING, std::move(string));
}

void Scanner::block_comment() {
    while (!((peek() == '*' && peek_next() == '/') || is_finished())) {
        if (peek() == '\n') {
            ++line;
        }

        advance();
    }

    if (is_finished()) {
        Lox::error(line, "Unterminated block comment.");
        return;
    }

    advance(2);
}

char Scanner::advance() {
    return source.at(current++);
}

std::string Scanner::advance(std::size_t character_count) {
    std::string return_value = source.substr(current, character_count);

    current += return_value.size();

    return return_value;
}

void Scanner::add_token(Token_Type type) {
    add_token(type, Value{});
}

void Scanner::add_token(Token_Type type, Value literal) {
    std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, std::move(lexeme), std::move(literal), line);
}

bool Scanner::match(char expected_character) {
    if (is_finished()) {
        return false;
    }

    if (source.at(current) != expected_character) {
        return false;
    }

    ++current;
    return true;
}

char Scanner::peek() const {
    if (is_finished()) {
        return '\0';
    }

    return source.at(current);
}

char Scanner::peek_next() const {
    if (current + 1 >= source.size()) {
        return '\0';
    }

    return source.at(current + 1);
}

bool Scanner::is_alpha(char character) {
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') || character == '_';
}

bool Scanner::is_digit(char character) {
    return character >= '0' && character <= '9';
}

bool Scanner::is_alphanumeric(char character) {
    return is_alpha(character) || is_digit(character);
}

std::vector<Token> Scanner::scan_tokens() {
    while (!is_finished()) {
        start = current;
        scan_token();
    }

    tokens.emplace_back(Token_Type::END_OF_FILE, "", Value{}, line);
    return std::move(tokens);
}

Scanner::Scanner(std::string source) : source(std::move(source)) {

}