#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "token.h"
#include "node.h"

namespace tachyon {
    class Parser {
    public:
        std::string filename;
        std::vector<Token> tokens;
        int tok_idx;
        Token current_tok;
        Parser(const std::string& filename, const std::vector<Token>& tokens);
        void advance();
        // void unadvance();
        void raise_error();
        std::shared_ptr<Node> parse();
        std::shared_ptr<Node> factor();
        std::shared_ptr<Node> vector_expr();
        std::shared_ptr<Node> map_expr();
        std::shared_ptr<Node> anon_func_expr();
        std::shared_ptr<Node> postfix_expr();
        std::shared_ptr<Node> multiplicative_expr();
        std::shared_ptr<Node> additive_expr();
        std::shared_ptr<Node> shift_expr();
        std::shared_ptr<Node> comp_expr();
        std::shared_ptr<Node> eq_expr();
        std::shared_ptr<Node> and_expr();
        std::shared_ptr<Node> xor_expr();
        std::shared_ptr<Node> or_expr();
        std::shared_ptr<Node> logical_and_expr();
        std::shared_ptr<Node> logical_or_expr();
        std::shared_ptr<Node> assign_expr();
        std::shared_ptr<Node> bin_op(const std::function<std::shared_ptr<Node>()>& func, const std::vector<TokenType>& ops);
        std::shared_ptr<Node> expr();
        std::shared_ptr<Node> expr_stmt();
        std::shared_ptr<Node> var_def_stmt();
        std::shared_ptr<Node> const_def_stmt();
        std::shared_ptr<Node> block_stmt();
        std::shared_ptr<Node> simple_block_stmt();
        std::shared_ptr<Node> if_stmt();
        std::shared_ptr<Node> while_stmt();
        std::shared_ptr<Node> for_stmt();
        std::shared_ptr<Node> return_stmt();
        std::shared_ptr<Node> func_def_stmt();
        std::shared_ptr<Node> try_catch_stmt();
        std::shared_ptr<Node> include_stmt();

        std::shared_ptr<Node> stmt();
        std::shared_ptr<Node> stmt_list(TokenType end);
    };
};

#endif