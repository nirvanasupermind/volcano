#include <string>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include "token.h"
#include "lexer.h"
#include "node.h"
#include "parser.h"
#include "transpiler.h"

namespace tachyon {
    Transpiler::Transpiler() {
        this->code = std::ostringstream();
    }

    void Transpiler::visit(const std::shared_ptr<Node>& node) {
        double d = node->get_double();
        if (!std::isnan(d)) {
            code << "TachyonVal(" << d << ")";
        }
        else {
            switch (node->get_type()) {
            case NodeType::UNARY_OP:
                visit_unary_op_node(std::static_pointer_cast<UnaryOpNode>(node));
                break;
            case NodeType::EXPR_STMT:
                visit_expr_stmt_node(std::static_pointer_cast<ExprStmtNode>(node));
                break;
            case NodeType::INCLUDE_STMT:
                visit_include_stmt_node(std::static_pointer_cast<IncludeStmtNode>(node));
                break;
            case NodeType::STMT_LIST:
                visit_stmt_list_node(std::static_pointer_cast<StmtListNode>(node));
                break;
            default:
                std::cout << "UNKNOWN NODE: " << node->to_string() << '\n';
                break;
            }
        }
    }

    void Transpiler::visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node) {
        if (node->op_tok.type == TokenType::PLUS) {
            visit(node->right_node);
        }
        else if (node->op_tok.type == TokenType::MINUS) {
            code << "TachyonVal(";
            code << "-(";
            visit(node->right_node);
            code << "))";
        }
         else if (node->op_tok.type == TokenType::NOT) {
            code << "TachyonVal(";
            code << "~(int32_t)(";
            visit(node->right_node);
            code << ").val)";
        }
        else if (node->op_tok.type == TokenType::INC || node->op_tok.type == TokenType::DEC) {
            visit(node->right_node);
            code << ".val" << node->op_tok.val;
        }
    }

    void Transpiler::visit_bin_op_node(const std::shared_ptr<BinOpNode>& node) {
        if(node->op_tok.type == TokenType::EQ) {
        visit(node->left_node);
        code << "=";
        visit(node->right_node);
        }
        else if(node->op_tok.type == TokenType::AND || node->op_tok.type == TokenType::OR || node->op_tok.type == TokenType::XOR
        || node->op_tok.type == TokenType::LSH || node->op_tok.type == TokenType::RSH) {
                    code << "TachyonVal((int32_t)(";
        visit(node->left_node);
        code << ").val";
        code << node->op_tok.val;
        code << "(int32_t)(";
        visit(node->right_node);
        code << ").val)";
        } else {
        code << "TachyonVal((";
        visit(node->left_node);
        code << ").val";
        code << node->op_tok.val;
        code << "(";
        visit(node->right_node);
        code << ").val)";
        }
    }

    void Transpiler::visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node) {
        visit(node->expr_node);
        code << ";";
    }



    void Transpiler::visit_include_stmt_node(const std::shared_ptr<IncludeStmtNode>& node) {
        std::string filename = node->path.val;
        std::ifstream in_file;

        in_file.open(filename);

        std::stringstream strStream;
        strStream << in_file.rdbuf();

        std::string text = strStream.str();

        Lexer lexer(filename, text);
        std::vector<Token> tokens = lexer.make_tokens();
        Parser parser(filename, tokens);
        std::shared_ptr<Node> tree = parser.parse();
        Transpiler transpiler;
        transpiler.visit(tree);

        code << transpiler.code.str();
    }

    void Transpiler::visit_stmt_list_node(const std::shared_ptr<StmtListNode>& node) {
        for (int i = 0; i < node->stmts.size(); i++) {
            visit(node->stmts.at(i));
            code << '\n';
        }
    }
}