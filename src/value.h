//value.h
#pragma once

#include <variant>
#include <string>

using Value = std::variant<std::monostate, double, std::string>;

std::string to_string(const Value& value);