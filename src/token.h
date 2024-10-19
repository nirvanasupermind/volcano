#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace tachyon {
    enum class TokenType {
        FLOAT,
        STRING,
        IDENTIFIER,
        KEYWORD,
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        NOT,
        AND,
        OR,
        XOR,
        LSH,
        RSH,
        LOGICAL_AND,
        LOGICAL_OR,
        EE,
        NE,
        LT,
        GT,
        LE,
        GE,
        EQ,
        PLUS_EQ,
        MINUS_EQ,
        MUL_EQ,
        DIV_EQ,
        MOD_EQ,
        AND_EQ,
        OR_EQ,
        XOR_EQ,
        LSH_EQ,
        RSH_EQ,
        INC,
        DEC,
        LPAREN,
        RPAREN,
        LSQUARE,
        RSQUARE,
        LCURLY,
        RCURLY,
        PERIOD,
        COMMA,
        COLON,
        SEMICOLON,
        EOF_
    };

    class Token {
    public:
        int line;
        TokenType type;
        std::string val;
        Token();
        Token(int line, TokenType type, const std::string& val);
        std::string to_string() const;
    };
};

#endif