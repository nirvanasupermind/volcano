#include <string>
#include <vector>
#include <iostream>
#include "token.h"
#include "lexer.h"

namespace tachyon {
    Lexer::Lexer(const std::string& filename, const std::string& text) {
        this->filename = filename;
        this->text = text;
        this->line = 1;
        this->pos = -1;
        this->current_char = '\0';
        advance();
    }

    void Lexer::advance() {
        pos++;
        current_char = pos < text.size() ? text.at(pos) : '\0';
        if (current_char == '\n') {
            line++;
        }
    }

    std::vector<Token> Lexer::make_tokens() {
        std::vector<Token> tokens;

        while (current_char != '\0') {
            if (current_char == ' ' || current_char == '\n' || current_char == '\t') {
                advance();
            }
            else if (isdigit(current_char)) {
                tokens.push_back(make_float());
            }
            else if (current_char == '\"') {
                tokens.push_back(make_string());
            }
            else if (isalpha(current_char) || current_char == '_') {
                tokens.push_back(make_identifier());
            }
            else if (current_char == '+') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::PLUS_EQ, "+="));
                    advance();
                }
                else if (current_char == '+') {
                    tokens.push_back(Token(line, TokenType::INC, "++"));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::PLUS, "+"));
                }
            }
            else if (current_char == '-') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::MINUS_EQ, "-="));
                    advance();
                }
                else if (current_char == '-') {
                    tokens.push_back(Token(line, TokenType::DEC, "--"));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::MINUS, "-"));
                }
            }
            else if (current_char == '*') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::MUL_EQ, "*="));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::MUL, "*"));
                }
            }
            else if (current_char == '/') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::DIV_EQ, "/="));
                    advance();
                }
                else if(current_char == '/') {
                    while(current_char != '\n' && current_char != '\0') {
                        advance();
                    }
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::DIV, "/"));
                }
            }
            else if (current_char == '%') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::MOD_EQ, "%="));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::MOD, "%"));
                }
            }
            else if (current_char == '~') {
                tokens.push_back(Token(line, TokenType::NOT, "~"));
                advance();
            }
            else if (current_char == '&') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::AND_EQ, "&="));
                    advance();
                }
                // else if(current_char == '&') {
                //     tokens.push_back(Token(line, TokenType::LOGICAL_AND, "&&"));
                // }
                else {
                    tokens.push_back(Token(old_line, TokenType::AND, "&"));
                }
            }
            else if (current_char == '|') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::OR_EQ, "|="));
                    advance();
                }
                // else if(current_char == '|') {
                //     tokens.push_back(Token(line, TokenType::LOGICAL_OR, "||"));
                // }
                else {
                    tokens.push_back(Token(old_line, TokenType::OR, "|"));
                }
            }
            else if (current_char == '^') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::XOR_EQ, "^="));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::XOR, "^"));
                }
            }
            else if (current_char == '=') {
                int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::EE, "=="));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::EQ, "="));
                }
            }
            else if (current_char == '!') {
                // int old_line = line;
                advance();
                if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::NE, "!="));
                    advance();
                }
                // else {
                //     tokens.push_back(Token(old_line, TokenType::LOGICAL_NOT, "!"));
                // }
            }
            else if (current_char == '<') {
                int old_line = line;
                advance();
                if (current_char == '<') {
                    advance();
                    if (current_char == '=') {
                        tokens.push_back(Token(line, TokenType::LSH_EQ, "<<="));
                        advance();
                    }
                    else {
                        tokens.push_back(Token(old_line, TokenType::LSH, "<<"));
                    }
                }
                else if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::LE, "<="));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::LT, "<"));
                }
            }
            else if (current_char == '>') {
                int old_line = line;
                advance();
                if (current_char == '>') {
                    advance();
                    if (current_char == '=') {
                        tokens.push_back(Token(line, TokenType::RSH_EQ, ">>="));
                        advance();
                    }
                    else {
                        tokens.push_back(Token(old_line, TokenType::RSH, ">>"));
                    }
                }
                else if (current_char == '=') {
                    tokens.push_back(Token(line, TokenType::GE, ">="));
                    advance();
                }
                else {
                    tokens.push_back(Token(old_line, TokenType::GT, ">"));
                }
            }
            else if (current_char == '(') {
                tokens.push_back(Token(line, TokenType::LPAREN, "("));
                advance();
            }
            else if (current_char == ')') {
                tokens.push_back(Token(line, TokenType::RPAREN, ")"));
                advance();
            }
            else if (current_char == '[') {
                tokens.push_back(Token(line, TokenType::LSQUARE, "["));
                advance();
            }
            else if (current_char == ']') {
                tokens.push_back(Token(line, TokenType::RSQUARE, "]"));
                advance();
            }
            else if (current_char == '{') {
                tokens.push_back(Token(line, TokenType::LCURLY, "{"));
                advance();
            }
            else if (current_char == '}') {
                tokens.push_back(Token(line, TokenType::RCURLY, "}"));
                advance();
            }
            else if (current_char == '.') {
                tokens.push_back(Token(line, TokenType::PERIOD, "."));
                advance();
            }
            else if (current_char == ',') {
                tokens.push_back(Token(line, TokenType::COMMA, ","));
                advance();

            }
            else if (current_char == ':') {
                tokens.push_back(Token(line, TokenType::COLON, ":"));
                advance();
            }
            else if (current_char == ';') {
                tokens.push_back(Token(line, TokenType::SEMICOLON, ";"));
                advance();
            }
            else {
                throw std::runtime_error(filename + ":" + std::to_string(line) + ": " + "illegal character: '" + current_char + "'");
            }
        }
        tokens.push_back(Token(line, TokenType::EOF_, "<eof>"));
        return tokens;
    }

    Token Lexer::make_float() {
        std::string num_str = "";
        int dot_count = 0;
        while (current_char != '\0' && (isdigit(current_char) || current_char == '.')) {
            if (current_char == '.') {
                dot_count += 1;
                num_str += '.';
            }
            else {
                num_str += current_char;
            }
            advance();
        }

        return Token(line, TokenType::NUMBER, num_str);
    }

    Token Lexer::make_string() {
        int old_line = line;
        advance();
        std::string str = "";
        while (current_char != '\0' && current_char != '"') {
            str += current_char;
            advance();
        }
        advance();
        return Token(old_line, TokenType::STRING, str);
    }

    Token Lexer::make_identifier() {
        std::string identifier_str = "";
        while (current_char != '\0' && (isalnum(current_char) || current_char == '_')) {
            identifier_str += current_char;
            advance();
        }
        
        if (identifier_str == "true" || identifier_str == "false" || identifier_str == "null" ||
            identifier_str == "var" || identifier_str == "const" || identifier_str == "block" 
            || identifier_str == "if" || identifier_str == "elif" || identifier_str == "else" || identifier_str == "while"
            || identifier_str == "for" || identifier_str == "return" || identifier_str == "func" || identifier_str == "afunc"
            || identifier_str == "throw"
            || identifier_str == "try" || identifier_str == "catch" || identifier_str == "include" ) {
            return Token(line, TokenType::KEYWORD, identifier_str);
        }
        else {
            return Token(line, TokenType::IDENTIFIER, identifier_str);
        }
    }
};