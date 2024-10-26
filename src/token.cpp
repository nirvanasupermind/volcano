#include <string>
#include "token.h"

namespace tachyon {
    Token::Token() {
        this->line = 0;
        this->type = TokenType::EOF_;
        this->val = "<eof>";
    }

    Token::Token(int line, TokenType type, const std::string& val) {
        this->line = line;
        this->type = type;
        this->val = val;
    }

    std::string Token::to_string() const {
        std::string result = "";
        switch (type) {
        case TokenType::NUMBER:
            result += "FLOAT:";
            break;
        case TokenType::STRING:
            result += "STRING:";
            break;
        case TokenType::IDENTIFIER:
            result += "IDENTIFIER:";
            break;
        case TokenType::KEYWORD:
            result += "KEYWORD:";
            break;
        case TokenType::PLUS:
            result += "PLUS:";
            break;
        case TokenType::MINUS:
            result += "MINUS:";
            break;
        case TokenType::MUL:
            result += "MUL:";
            break;
        case TokenType::DIV:
            result += "DIV:";
            break;
        case TokenType::MOD:
            result += "MOD:";
            break;
        case TokenType::NOT:
            result += "NOT:";
            break;
        case TokenType::AND:
            result += "AND:";
            break;
        case TokenType::OR:
            result += "OR:";
            break;
        case TokenType::XOR:
            result += "XOR:";
            break;
        case TokenType::LSH:
            result += "LSH:";
            break;
        case TokenType::RSH:
            result += "RSH:";
            break;
        // case TokenType::LOGICAL_NOT:
        //     result += "LOGICAL_NOT:";
        //     break;
        // case TokenType::LOGICAL_AND:
        //     result += "LOGICAL_OR:";
        //     break;
        // case TokenType::LOGICAL_OR:
        //     result += "LOGICAL_OR:";
        //     break;
        case TokenType::EE:
            result += "EE:";
            break;
        case TokenType::NE:
            result += "NE:";
            break;
        case TokenType::LT:
            result += "LT:";
            break;
        case TokenType::GT:
            result += "GT:";
            break;
        case TokenType::LE:
            result += "LE:";
            break;
        case TokenType::GE:
            result += "GE:";
            break;
        case TokenType::EQ:
            result += "EQ:";
            break;
        case TokenType::PLUS_EQ:
            result += "PLUS_EQ:";
            break;
        case TokenType::MINUS_EQ:
            result += "MINUS_EQ:";
            break;
        case TokenType::MUL_EQ:
            result += "MUL_EQ:";
            break;
        case TokenType::DIV_EQ:
            result += "DIV_EQ:";
            break;
        case TokenType::MOD_EQ:
            result += "MOD_EQ:";
            break;
        case TokenType::AND_EQ:
            result += "AND_EQ:";
            break;
        case TokenType::OR_EQ:
            result += "OR_EQ:";
            break;
        case TokenType::XOR_EQ:
            result += "XOR_EQ:";
            break;
        case TokenType::LSH_EQ:
            result += "LSH_EQ:";
            break;
        case TokenType::RSH_EQ:
            result += "RSH_EQ:";
            break;
        case TokenType::INC:
            result += "INC:";
            break;
        case TokenType::DEC:
            result += "DEC:";
            break;
        case TokenType::LPAREN:
            result += "LPAREN:";
            break;
        case TokenType::RPAREN:
            result += "RPAREN:";
            break;
        case TokenType::LSQUARE:
            result += "LSQUARE:";
            break;
        case TokenType::RSQUARE:
            result += "RSQUARE:";
            break;
        case TokenType::LCURLY:
            result += "LCURLY:";
            break;
        case TokenType::RCURLY:
            result += "RCURLY:";
            break;
        case TokenType::PERIOD:
            result += "PERIOD:";
            break;
        case TokenType::COMMA:
            result += "COMMA:";
            break;  
        case TokenType::COLON:
            result += "COLON:";
            break;   
        case TokenType::SEMICOLON:
            result += "SEMICOLON:";
            break;   
        case TokenType::EOF_:
            result += "EOF:";
            break;
        }
        result += val;
        return result;
    }
};