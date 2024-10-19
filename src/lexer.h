#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

namespace tachyon {
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