#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"

namespace tachyon {
    // std::unordered_map<char, char> escapes{
    //     {'n', '\n'},
    //     {'t', '\t'},
    //     {'r', '\r'},
    //     {'0', '\0'},
    //     {'"', '\"'},
    // };

    class Lexer {
    public:
        std::string filename;
        std::string text;
        int line;
        int pos;
        char current_char;
        Lexer(const std::string& filename, const std::string& text);
        void advance();
        std::vector<Token> make_tokens();
        Token make_float();
        Token make_string();
        Token make_identifier();
    };
};

#endif