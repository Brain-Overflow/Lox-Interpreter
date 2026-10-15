//value.cpp
#include <string>
#include <format>
#include <type_traits>
#include "value.h"

std::string to_string(const Value& value) {
    return std::visit([](const auto& variant_value) -> std::string {
        using stripped_type = std::decay_t<decltype(variant_value)>;

        if constexpr (std::is_same_v<stripped_type, std::monostate>) {
            return "nil";
        }
        else if constexpr (std::is_same_v<stripped_type, double>) {
            return std::format("{}", variant_value);
        }
        else if constexpr (std::is_same_v<stripped_type, std::string>) {
            return variant_value;
        }
    }, value);
}