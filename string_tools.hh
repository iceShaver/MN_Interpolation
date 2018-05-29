//
// Created by kamil on 28.05.18.
//

#ifndef MN_INTERPOLATION_STRING_TOOLS_HH
#define MN_INTERPOLATION_STRING_TOOLS_HH

#include <memory>
#include <vector>
#include <sstream>

template<typename CharT>
using tstring = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

template<typename CharT>
using tstringstream = std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT>>;


template<typename CharT>
std::vector<tstring<CharT>> split(tstring<CharT> text, CharT const delimiter) {
    auto sstr = tstringstream<CharT>{text};
    auto tokens = std::vector<tstring<CharT>>{};
    auto token = tstring<CharT>{};
    while (std::getline(sstr, token, delimiter)) {
        if (!token.empty()) { tokens.push_back(token); }
    }
    return tokens;
}

#endif //MN_INTERPOLATION_STRING_TOOLS_HH
