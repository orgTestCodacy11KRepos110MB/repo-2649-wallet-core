// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <boost/algorithm/hex.hpp>

#include <array>
#include <sstream>
#include <string>
#include <tuple>

namespace TW {

inline bool is_hex_encoded(const std::string& s)
{
    bool with_0x = s.compare(0, 2, "0x") == 0
           && s.size() > 2
           && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
    bool without_0x = s.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
    return with_0x || without_0x;
}

std::tuple<uint8_t, bool> value(uint8_t c);

/// Converts a range of bytes to a hexadecimal string representation.
template <typename Iter>
inline std::string hex(const Iter begin, const Iter end) {
    static constexpr std::array<char, 16> hexmap = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    std::string result;
    result.reserve((end - begin) * 2);

    for (auto it = begin; it < end; ++it) {
        auto val = static_cast<uint8_t>(*it);
        result.push_back(hexmap[val >> 4]);
        result.push_back(hexmap[val & 0x0f]);
    }

    return result;
}

/// Converts a collection of bytes to a hexadecimal string representation.
template <typename T>
inline std::string hex(const T& collection) {
    return hex(std::begin(collection), std::end(collection));
}

/// same as hex, with 0x prefix
template <typename T>
inline std::string hexEncoded(const T& collection) {
    return hex(std::begin(collection), std::end(collection)).insert(0, "0x");
}

/// Converts a `uint64_t` value to a hexadecimal string.
inline std::string hex(uint64_t value) {
    auto bytes = reinterpret_cast<const uint8_t*>(&value);
    return hex(std::reverse_iterator<const uint8_t*>(bytes + sizeof(value)),
               std::reverse_iterator<const uint8_t*>(bytes));
}

/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not
/// valid hexadecimal.
template <typename Iter>
inline Data parse_hex(const Iter begin, const Iter end) {
    auto it = begin;

    // Skip `0x`
    if (end - begin >= 2 && *begin == '0' && *(begin + 1) == 'x') {
        it += 2;
    }
    try {
        std::string temp;
        boost::algorithm::unhex(it, end, std::back_inserter(temp));
        return Data(temp.begin(), temp.end());
    } catch (...) {
        return {};
    }
}

/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not
/// valid hexadecimal.
inline Data parse_hex(const std::string& string, bool padLeft = false) {
    if (string.size() % 2 != 0 && padLeft) {
        std::string temp = string;
        if (temp.compare(0, 2, "0x") == 0) {
            temp.erase(0, 2);
        }
        temp.insert(0, 1, '0');
        return parse_hex(temp.begin(), temp.end());
    }
    return parse_hex(string.begin(), string.end());
}

inline const char* hex_char_to_bin(char c) {
    switch (toupper(c)) {
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'A':
        return "1010";
    case 'B':
        return "1011";
    case 'C':
        return "1100";
    case 'D':
        return "1101";
    case 'E':
        return "1110";
    case 'F':
        return "1111";
    default:
        return "";
    }
}

inline std::string hex_str_to_bin_str(const std::string& hex) {
    std::stringstream ss;
    for (auto&& c: hex) {
        ss << hex_char_to_bin(c);
    }
    return ss.str();
}

} // namespace TW
