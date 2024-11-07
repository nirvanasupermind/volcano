#include <string>
#include <vector>
#include <iostream>
#include "parser.h"

namespace tachyon {
    Parser::Parser(const std::string& filename, const std::vector<Token>& tokens) {
        this->filename = filename;
        this->tokens = tokens;
        this->tok_idx = -1;
        advance();
    }

    void Parser::advance() {
        tok_idx++;
        if (tok_idx < tokens.size()) {
            current_tok = tokens.at(tok_idx);
        }
    }

    // void Parser::unadvance() {
    //     tok_idx--;
    //     current_tok = tokens.at(tok_idx);
    // }

    void Parser::raise_error() {
        throw std::string(filename + ":" + std::to_string(current_tok.line) + ": unexpected '" + (current_tok.val) + "'");
    }

    std::shared_ptr<Node> Parser::parse() {
        return stmt_list(TokenType::EOF_);
    }

    std::shared_ptr<Node> Parser::factor() {
        Token tok = current_tok;
        if (tok.type == TokenType::PLUS || tok.type == TokenType::MINUS || tok.type == TokenType::NOT
            || (tok.type == TokenType::KEYWORD && tok.val == "clone")) {
            advance();
            return std::make_shared<UnaryOpNode>(UnaryOpNode(tok, expr()));
        }
        else if (tok.type == TokenType::LPAREN) {
            advance();
            std::shared_ptr<Node> inner_expr = expr();
            if (current_tok.type != TokenType::RPAREN) {
                raise_error();
            }
            advance();
            return inner_expr;
        }
        else if (tok.type == TokenType::NUMBER) {
            advance();
            return std::make_shared<NumberNode>(NumberNode(tok));
        }
        else if (tok.type == TokenType::KEYWORD && tok.val == "null") {
            advance();
            return std::make_shared<NullNode>(NullNode());
        }
        // else if (tok.type == TokenType::KEYWORD && (tok.val == "true" || tok.val == "false")) {
        //     advance();
        //     return std::make_shared<BoolNode>(BoolNode(tok));
        // }
        // else if (tok.type == TokenType::KEYWORD && tok.val == "null") {
        //     advance();
        //     return std::make_shared<NullNode>(NullNode());
        // }
        else if (tok.type == TokenType::STRING) {
            advance();
            return std::make_shared<StringNode>(StringNode(tok));
        }
        else if (tok.type == TokenType::IDENTIFIER) {
            advance();
            return std::make_shared<IdentifierNode>(IdentifierNode(tok));
        }
        else if (tok.type == TokenType::LSQUARE) {
            return vector_expr();
        }
        else if (tok.type == TokenType::LCURLY) {
            return object_expr();
        }
        else if (tok.type == TokenType::KEYWORD && tok.val == "afunc") {
            return anon_func_expr();
        }
        else {
            raise_error();
        }
    }

    std::shared_ptr<Node> Parser::vector_expr() {
        if (current_tok.type != TokenType::LSQUARE) {
            raise_error();
        }
        advance();
        std::vector<std::shared_ptr<Node> > elements;
        if (current_tok.type == TokenType::RSQUARE) {
            advance();
        }
        else {
            while (true) {
                elements.push_back(expr());
                if (current_tok.type == TokenType::RSQUARE) {
                    advance();
                    break;
                }
                else if (current_tok.type == TokenType::COMMA) {
                    advance();
                }
                else {
                    raise_error();
                }
            }
        }
        return std::make_shared<VectorNode>(VectorNode(elements));
    }

    std::shared_ptr<Node> Parser::object_expr() {
        if (current_tok.type != TokenType::LCURLY) {
            raise_error();
        }
        advance();
        std::vector<std::shared_ptr<Node> > keys;
        std::vector<std::shared_ptr<Node> > vals;
        if (current_tok.type == TokenType::RCURLY) {
            advance();
        }
        else {
            while (true) {
                keys.push_back(expr());
                if (current_tok.type != TokenType::COLON) {
                    raise_error();
                }
                advance();
                vals.push_back(expr());
                if (current_tok.type == TokenType::RCURLY) {
                    advance();
                    break;
                }
                else if (current_tok.type == TokenType::COMMA) {
                    advance();
                }
                else {
                    raise_error();
                }
            }
        }
        return std::make_shared<ObjectNode>(ObjectNode(keys, vals));
    }

    std::shared_ptr<Node> Parser::anon_func_expr() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "afunc")) {
            raise_error();
        }
        advance();
        if (current_tok.type != TokenType::LPAREN) {
            raise_error();
        }
        advance();
        std::vector<Token> arg_names;
        if (current_tok.type == TokenType::RPAREN) {
            advance();
        }
        else {
            while (true) {
                if (current_tok.type != TokenType::IDENTIFIER) {
                    raise_error();
                }
                arg_names.push_back(current_tok);
                advance();
                if (current_tok.type == TokenType::RPAREN) {
                    advance();
                    break;
                }
                else if (current_tok.type == TokenType::COMMA) {
                    advance();
                }
                else {
                    raise_error();
                }
            }
        }

        std::shared_ptr<Node> body = simple_block_stmt();
        return std::make_shared<AnonFuncExprNode>(AnonFuncExprNode(arg_names, body));
    }

    std::shared_ptr<Node> Parser::postfix_expr() {
        std::shared_ptr<Node> result = factor();

        while (true) {
            if (current_tok.type == TokenType::LPAREN) {
                advance();
                std::vector<std::shared_ptr<Node> > args;
                if (current_tok.type == TokenType::RPAREN) {
                    advance();
                }
                else {
                    while (true) {
                        args.push_back(expr());
                        if (current_tok.type == TokenType::RPAREN) {
                            advance();
                            break;
                        }
                        else if (current_tok.type == TokenType::COMMA) {
                            advance();
                        }
                        else {
                            raise_error();
                        }
                    }
                }
                result = std::make_shared<CallExprNode>(CallExprNode(result, args));
            }
            else if (current_tok.type == TokenType::PERIOD) {
                advance();
                if (current_tok.type != TokenType::IDENTIFIER) {
                    raise_error();
                }
                result = std::make_shared<ObjectPropNode>(ObjectPropNode(result, current_tok));
                advance();
            }
            else if (current_tok.type == TokenType::LSQUARE) {
                advance();
                std::shared_ptr<Node> idx_node = expr();
                if (current_tok.type != TokenType::RSQUARE) {
                    raise_error();
                }
                advance();
                result = std::make_shared<SubscriptNode>(SubscriptNode(result, idx_node));
            }
            else if (current_tok.type == TokenType::INC || current_tok.type == TokenType::DEC) {
                result = std::make_shared<UnaryOpNode>(UnaryOpNode(current_tok, result));
                advance();
            }
            else {
                // advance();
                break;
            }
        }

        return result;
    }

    std::shared_ptr<Node> Parser::multiplicative_expr() {
        return bin_op([this]() {return this->postfix_expr(); }, { TokenType::MUL, TokenType::DIV, TokenType::MOD });
    }

    std::shared_ptr<Node> Parser::additive_expr() {
        return bin_op([this]() {return this->multiplicative_expr(); }, { TokenType::PLUS, TokenType::MINUS });
    }

    std::shared_ptr<Node> Parser::shift_expr() {
        return bin_op([this]() {return this->additive_expr(); }, { TokenType::LSH, TokenType::RSH });
    }

    std::shared_ptr<Node> Parser::comp_expr() {
        return bin_op([this]() {return this->shift_expr(); }, { TokenType::LT, TokenType::GT, TokenType::LE, TokenType::GE });
    }

    std::shared_ptr<Node> Parser::eq_expr() {
        return bin_op([this]() {return this->comp_expr(); }, { TokenType::EE, TokenType::NE });
    }

    std::shared_ptr<Node> Parser::and_expr() {
        return bin_op([this]() {return this->eq_expr(); }, { TokenType::AND });
    }

    std::shared_ptr<Node> Parser::xor_expr() {
        return bin_op([this]() {return this->and_expr(); }, { TokenType::XOR });
    }

    std::shared_ptr<Node> Parser::or_expr() {
        return bin_op([this]() {return this->xor_expr(); }, { TokenType::OR });
    }

    // std::shared_ptr<Node> Parser::logical_and_expr() {
    //     return bin_op([this]() {return this->or_expr(); }, { TokenType::LOGICAL_AND });
    // }

    // std::shared_ptr<Node> Parser::logical_or_expr() {
    //     return bin_op([this]() {return this->logical_or_expr(); }, { TokenType::LOGICAL_OR });
    // }

    std::shared_ptr<Node> Parser::assign_expr() {
        return bin_op([this]() {return this->or_expr(); }, { TokenType::EQ, TokenType::PLUS_EQ, TokenType::MINUS_EQ,
        TokenType::MUL_EQ, TokenType::DIV_EQ, TokenType::MOD_EQ,
        TokenType::AND_EQ, TokenType::OR_EQ, TokenType::XOR_EQ,
        TokenType::LSH_EQ, TokenType::RSH_EQ });
    }

    std::shared_ptr<Node> Parser::bin_op(const std::function<std::shared_ptr<Node>()>& func, const std::vector<TokenType>& ops) {
        std::shared_ptr<Node> left = func();

        while (std::find(ops.begin(), ops.end(), current_tok.type) != ops.end()) {
            Token op_tok = current_tok;
            advance();
            std::shared_ptr<Node> right = func();
            left = std::make_shared<BinOpNode>(BinOpNode(left, op_tok, right));
        }

        return left;
    }

    std::shared_ptr<Node> Parser::expr() {
        return assign_expr();
    }

    std::shared_ptr<Node> Parser::expr_stmt() {
        std::shared_ptr<Node> res = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<ExprStmtNode>(ExprStmtNode(res));
    }

    std::shared_ptr<Node> Parser::var_def_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "var")) {
            raise_error();
        }
        advance();
        Token name_tok = current_tok;
        if (current_tok.type != TokenType::IDENTIFIER) {
            raise_error();
        }
        advance();
        if (current_tok.type != TokenType::EQ) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> val = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<VarDefStmtNode>(VarDefStmtNode(name_tok, val));
    }

    std::shared_ptr<Node> Parser::const_def_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "const")) {
            raise_error();
        }
        advance();
        Token name_tok = current_tok;
        if (current_tok.type != TokenType::IDENTIFIER) {
            raise_error();
        }
        advance();
        if (current_tok.type != TokenType::EQ) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> val = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<ConstDefStmtNode>(ConstDefStmtNode(name_tok, val));
    }

    std::shared_ptr<Node> Parser::block_stmt() {
        if (current_tok.type != TokenType::KEYWORD && current_tok.val == "block") {
            raise_error();
        }
        advance();
        return simple_block_stmt();
    }

    std::shared_ptr<Node> Parser::simple_block_stmt() {
        if (current_tok.type != TokenType::LCURLY) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> stmt_list_node = stmt_list(TokenType::RCURLY);
        advance();
        return std::make_shared<BlockStmtNode>(BlockStmtNode(stmt_list_node));
    }

    std::shared_ptr<Node> Parser::if_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "if")) {
            raise_error();
        }
        advance();
        std::vector<std::shared_ptr<Node> > conds;
        std::vector<std::shared_ptr<Node> > bodies;
        std::shared_ptr<Node> else_body;
        if (!(current_tok.type == TokenType::LPAREN)) {
            raise_error();
        }
        advance();
        conds.push_back(expr());
        if (!(current_tok.type == TokenType::RPAREN)) {
            raise_error();
        }
        advance();
        bodies.push_back(simple_block_stmt());
        while (current_tok.type == TokenType::KEYWORD && current_tok.val == "elif") {
            advance();

            if (!(current_tok.type == TokenType::LPAREN)) {
                raise_error();
            }
            advance();
            conds.push_back(expr());
            if (!(current_tok.type == TokenType::RPAREN)) {
                raise_error();
            }
            advance();
            bodies.push_back(simple_block_stmt());
        }

        if (current_tok.type == TokenType::KEYWORD && current_tok.val == "else") {
            advance();
            else_body = simple_block_stmt();
        }

        return std::make_shared<IfStmtNode>(IfStmtNode(conds, bodies, else_body));
    }

    std::shared_ptr<Node> Parser::while_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "while")) {
            raise_error();
        }
        advance();
        if (!(current_tok.type == TokenType::LPAREN)) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> cond = expr();
        if (!(current_tok.type == TokenType::RPAREN)) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> body = simple_block_stmt();
        return std::make_shared<WhileStmtNode>(WhileStmtNode(cond, body));
    }

    std::shared_ptr<Node> Parser::for_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "for")) {
            raise_error();
        }
        advance();
        if (current_tok.type != TokenType::LPAREN) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> init = stmt();
        std::shared_ptr<Node> cond = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> update = expr();
        if (current_tok.type != TokenType::RPAREN) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> body = simple_block_stmt();
        return std::make_shared<ForStmtNode>(ForStmtNode(init, cond, update, body));
    }

    std::shared_ptr<Node> Parser::continue_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "continue")) {
            raise_error();
        }
        advance();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<ContinueStmtNode>(ContinueStmtNode());
    }


    std::shared_ptr<Node> Parser::break_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "break")) {
            raise_error();
        }
        advance();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<BreakStmtNode>(BreakStmtNode());
    }


    std::shared_ptr<Node> Parser::return_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "return")) {
            raise_error();
        }
        advance();

        std::shared_ptr<Node> res = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<ReturnStmtNode>(ReturnStmtNode(res));
    }

    std::shared_ptr<Node> Parser::func_def_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "func")) {
            raise_error();
        }

        advance();
        if (current_tok.type != TokenType::IDENTIFIER) {
            raise_error();
        }
        Token name_tok = current_tok;
        advance();
        if (current_tok.type != TokenType::LPAREN) {
            raise_error();
        }
        advance();
        std::vector<Token> arg_names;
        if (current_tok.type == TokenType::RPAREN) {
            advance();
        }
        else {
            while (true) {
                if (current_tok.type != TokenType::IDENTIFIER) {
                    raise_error();
                }
                arg_names.push_back(current_tok);
                advance();
                if (current_tok.type == TokenType::RPAREN) {
                    advance();
                    break;
                }
                else if (current_tok.type == TokenType::COMMA) {
                    advance();
                }
                else {
                    raise_error();
                }
            }
        }
        std::shared_ptr<Node> body = simple_block_stmt();
        return std::make_shared<FuncDefStmtNode>(FuncDefStmtNode(name_tok, arg_names, body));
    }


    std::shared_ptr<Node> Parser::throw_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "throw")) {
            raise_error();
        }
        advance();

        std::shared_ptr<Node> res = expr();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<ThrowStmtNode>(ThrowStmtNode(res));
    }
    std::shared_ptr<Node> Parser::try_catch_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "try")) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> try_body = simple_block_stmt();
        advance();
        if (!(current_tok.type == TokenType::LPAREN)) {
            raise_error();
        }
        advance();
        if (!(current_tok.type == TokenType::IDENTIFIER)) {
            raise_error();
        }
        Token error = current_tok;
        advance();
        if (!(current_tok.type == TokenType::RPAREN)) {
            raise_error();
        }
        advance();
        std::shared_ptr<Node> catch_body = simple_block_stmt();
        return std::make_shared<TryCatchStmtNode>(TryCatchStmtNode(try_body, error, catch_body));
    }

    std::shared_ptr<Node> Parser::include_stmt() {
        if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "include")) {
            raise_error();
        }
        advance();
        if (!(current_tok.type == TokenType::STRING)) {
            raise_error();
        }
        Token path = current_tok;
        advance();
        if (current_tok.type != TokenType::SEMICOLON) {
            raise_error();
        }
        advance();
        return std::make_shared<IncludeStmtNode>(IncludeStmtNode(path));
    }

    // std::shared_ptr<Node> Parser::define_stmt() {
    //     if (!(current_tok.type == TokenType::KEYWORD && current_tok.val == "define")) {
    //         raise_error();
    //     }
    //     advance();
    //     if (!(current_tok.type == TokenType::IDENTIFIER)) {
    //         raise_error();
    //     }
    //     Token name = current_tok;
    //     advance();
    //     std::shared_ptr<Node> expr_node = expr();
    //     advance();
    //     if (current_tok.type != TokenType::SEMICOLON) {
    //         raise_error();
    //     }
    //     advance();
    //     return std::make_shared<DefineStmtNode>(DefineStmtNode(name, expr_node));
    // }


    std::shared_ptr<Node> Parser::stmt() {
        if (current_tok.type == TokenType::KEYWORD && current_tok.val == "block") {
            return block_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "var") {
            return var_def_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "const") {
            return const_def_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "if") {
            return if_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "while") {
            return while_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "for") {
            return for_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "continue") {
            return continue_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "break") {
            return break_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "return") {
            return return_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "func") {
            return func_def_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "throw") {
            return throw_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "try") {
            return try_catch_stmt();
        }
        else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "include") {
            return include_stmt();
        }
        // else if (current_tok.type == TokenType::KEYWORD && current_tok.val == "define") {
        //     return define_stmt();
        // }
        else {
            return expr_stmt();
        }
    }

    std::shared_ptr<Node> Parser::stmt_list(TokenType end) {
        std::vector<std::shared_ptr<Node> > stmts;
        while (current_tok.type != end) {
            stmts.push_back(stmt());
        }
        if (current_tok.type != end) {
            raise_error();
        }
        return std::make_shared<StmtListNode>(StmtListNode(stmts));
    }
};