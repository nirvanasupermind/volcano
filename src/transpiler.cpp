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
            code << "tachyon_internal::Val(" << d << ")";
        }
        else {
            switch (node->get_type()) {
            case NodeType::STRING:
                visit_string_node(std::static_pointer_cast<StringNode>(node));
                break;
            case NodeType::VECTOR:
                visit_vector_node(std::static_pointer_cast<VectorNode>(node));
                break;
            case NodeType::MAP:
                visit_map_node(std::static_pointer_cast<MapNode>(node));
                break;
            case NodeType::ANON_FUNC_EXPR:
                visit_anon_func_expr_node(std::static_pointer_cast<AnonFuncExprNode>(node));
                break;
            case NodeType::IDENTIFIER:
                visit_identifier_node(std::static_pointer_cast<IdentifierNode>(node));
                break;
            case NodeType::CALL_EXPR:
                visit_call_expr_node(std::static_pointer_cast<CallExprNode>(node));
                break;
            case NodeType::OBJECT_PROP:
                visit_object_prop_node(std::static_pointer_cast<ObjectPropNode>(node));
                break;
            case NodeType::UNARY_OP:
                visit_unary_op_node(std::static_pointer_cast<UnaryOpNode>(node));
                break;
            case NodeType::BIN_OP:
                visit_bin_op_node(std::static_pointer_cast<BinOpNode>(node));
                break;
            case NodeType::EXPR_STMT:
                visit_expr_stmt_node(std::static_pointer_cast<ExprStmtNode>(node));
                break;
            case NodeType::VAR_DEF_STMT:
                visit_var_def_stmt_node(std::static_pointer_cast<VarDefStmtNode>(node));
                break;
            case NodeType::CONST_DEF_STMT:
                visit_const_def_stmt_node(std::static_pointer_cast<ConstDefStmtNode>(node));
                break;
            case NodeType::BLOCK_STMT:
                visit_block_stmt_node(std::static_pointer_cast<BlockStmtNode>(node));
                break;
            case NodeType::IF_STMT:
                visit_if_stmt_node(std::static_pointer_cast<IfStmtNode>(node));
                break;
            case NodeType::IF_ELSE_STMT:
                visit_if_else_stmt_node(std::static_pointer_cast<IfElseStmtNode>(node));
                break;
            case NodeType::WHILE_STMT:
                visit_while_stmt_node(std::static_pointer_cast<WhileStmtNode>(node));
                break;
            case NodeType::FOR_STMT:
                visit_for_stmt_node(std::static_pointer_cast<ForStmtNode>(node));
                break;
            case NodeType::RETURN_STMT:
                visit_return_stmt_node(std::static_pointer_cast<ReturnStmtNode>(node));
                break;
            case NodeType::FUNC_DEF_STMT:
                visit_func_def_stmt_node(std::static_pointer_cast<FuncDefStmtNode>(node));
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

    void Transpiler::visit_string_node(const std::shared_ptr<StringNode>& node) {
        code << "tachyon_internal::make_obj({},String.obj,new std::string(\"" << node->tok.val << "\"))";
    }

    void Transpiler::visit_vector_node(const std::shared_ptr<VectorNode>& node) {
        code << "tachyon_internal::make_obj({},Vector.obj,new std::vector<tachyon_internal::Val>({";
        if (node->elements.empty()) {
            code << "}))";
        }
        else {
            for (int i = 0; i < node->elements.size(); i++) {
                visit(node->elements.at(i));
                if (i == node->elements.size() - 1) {
                    code << "}))";
                }
                else {
                    code << ",";
                }
            }
        }
    }

    void Transpiler::visit_map_node(const std::shared_ptr<MapNode>& node) {
        code << "tachyon_internal::make_objObject({},Map.obj,new std::unordered_map<std::string, tachyon_internal::Val>({";
        if (node->keys.empty()) {
            code << "}))";
        }
        else {
            for (int i = 0; i < node->keys.size(); i++) {
                code << "{*(std::string*)(tachyon_internal::to_object(";
                visit(node->keys.at(i));
                code << ")).hidden_data,";
                visit(node->vals.at(i));
                if (i == node->keys.size() - 1) {
                    code << "}}))";
                }
                else {
                    code << "},";
                }
            }
        }
    }

    void Transpiler::visit_anon_func_expr_node(const std::shared_ptr<AnonFuncExprNode>& node) {
        code << "tachyon_internal::make_obj({},Function.obj,new tachyon_internal::func_type([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {\n";
        for (int i = 0; i < node->arg_names.size(); i++) {
            code << "tachyon_internal::Val " << node->arg_names.at(i).val << "= _args.at(" << i << ");\n";
        }
        visit(node->body);
        code << "}))";
    }

    void Transpiler::visit_identifier_node(const std::shared_ptr<IdentifierNode>& node) {
        code << node->tok.val;
    }

    void Transpiler::visit_call_expr_node(const std::shared_ptr<CallExprNode>& node) {
        code << "(*(tachyon_internal::func_type*)((";
        visit(node->callee);
        code << ").obj->hidden_data))({";
        if(node->callee->get_type() == NodeType::OBJECT_PROP) {
            visit(std::static_pointer_cast<ObjectPropNode>(node->callee)->obj);
            if(!node->args.empty()) {
                code << ",";
            }
        }
        for (int i = 0; i < node->args.size(); i++) {
            visit(node->args.at(i));
            if(i != node->args.size() - 1) {
                code << ",";
            }
        }
        code << "})";
    }

    void Transpiler::visit_object_prop_node(const std::shared_ptr<ObjectPropNode>& node) {
        visit(node->obj);
        code << ".obj->get(\"" << node->prop.val << "\")";
    }

    void Transpiler::visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node) {
        if (node->op_tok.type == TokenType::PLUS) {
            visit(node->right_node);
        }
        else if (node->op_tok.type == TokenType::MINUS) {
            code << "tachyon_internal::Val(";
            code << "-(";
            visit(node->right_node);
            code << "))";
        }
        else if (node->op_tok.type == TokenType::NOT) {
            code << "tachyon_internal::Val(";
            code << "~(int32_t)(";
            visit(node->right_node);
            code << ").num)";
        }
        else if (node->op_tok.type == TokenType::INC || node->op_tok.type == TokenType::DEC) {
            visit(node->right_node);
            code << ".num" << node->op_tok.val;
        }
    }

    void Transpiler::visit_bin_op_node(const std::shared_ptr<BinOpNode>& node) {
        if (node->op_tok.type == TokenType::EQ) {
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->left_node);
                code << "(";
                visit(temp->obj);
                code << ").obj->set(\"" << temp->prop.val << "\",";
                visit(node->right_node);
                code << ")";
            }
            else {
                visit(node->left_node);
                code << "=";
                visit(node->right_node);
            }
        }
        else if (node->op_tok.type == TokenType::PLUS_EQ || node->op_tok.type == TokenType::MINUS_EQ || node->op_tok.type == TokenType::MUL_EQ
            || node->op_tok.type == TokenType::DIV_EQ || node->op_tok.type == TokenType::MOD_EQ) {
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->left_node);
                code << "(";
                visit(temp->obj);
                code << ").obj->set(\"" << temp->prop.val << "\",";
                visit(node->left_node);
                code << node->op_tok.val;
                visit(node->right_node);
                code << ")";
            }
            else {
                code << node->op_tok.val;
                code << node->op_tok.val;
                code << "(";
                visit(node->right_node);
                code << ").num";
            }
        }
        else if (node->op_tok.type == TokenType::AND_EQ || node->op_tok.type == TokenType::OR_EQ || node->op_tok.type == TokenType::XOR_EQ
            || node->op_tok.type == TokenType::LSH_EQ || node->op_tok.type == TokenType::RSH_EQ) {
            code << "(";
            visit(node->left_node);
            code << ").num";
            code << "=";
            code << "(int32_t)(";
            visit(node->left_node);
            code << ").num";
            code << node->op_tok.val.at(0);
            code << "(int32_t)(";
            visit(node->right_node);
            code << ").num";
        }
        else if (node->op_tok.type == TokenType::AND || node->op_tok.type == TokenType::OR || node->op_tok.type == TokenType::XOR
            || node->op_tok.type == TokenType::LSH || node->op_tok.type == TokenType::RSH) {
            code << "tachyon_internal::Val((int32_t)(";
            visit(node->left_node);
            code << ").num";
            code << node->op_tok.val;
            code << "(int32_t)(";
            visit(node->right_node);
            code << ").num)";
        }
        else {
            code << "tachyon_internal::Val((";
            visit(node->left_node);
            code << ").num";
            code << node->op_tok.val;
            code << "(";
            visit(node->right_node);
            code << ").num)";
        }
    }

    void Transpiler::visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node) {
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_var_def_stmt_node(const std::shared_ptr<VarDefStmtNode>& node) {
        code << "tachyon_internal::Val " << node->name_tok.val << "=";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_const_def_stmt_node(const std::shared_ptr<ConstDefStmtNode>& node) {
        code << "const tachyon_internal::Val " << node->name_tok.val << "=";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_block_stmt_node(const std::shared_ptr<BlockStmtNode>& node) {
        code << "{\n";
        visit(node->stmt_list_node);
        code << "}";
    }

    void Transpiler::visit_if_stmt_node(const std::shared_ptr<IfStmtNode>& node) {
        code << "if((";
        visit(node->cond);
        code << ").num)";
        visit(node->body);
    }

    void Transpiler::visit_if_else_stmt_node(const std::shared_ptr<IfElseStmtNode>& node) {
        code << "if((";
        visit(node->cond);
        code << ").num)";
        visit(node->if_body);
        code << "else";
        visit(node->else_body);
    }

    void Transpiler::visit_while_stmt_node(const std::shared_ptr<WhileStmtNode>& node) {
        code << "while((";
        visit(node->cond);
        code << ").num)";
        visit(node->body);
    }

    void Transpiler::visit_for_stmt_node(const std::shared_ptr<ForStmtNode>& node) {
        code << "for(";
        visit(node->init);
        code << "(";
        visit(node->cond);
        code << ").num";
        code << ";";
        visit(node->update);
        code << ") {";
        visit(node->body);
        code << "}";
    }


    void Transpiler::visit_return_stmt_node(const std::shared_ptr<ReturnStmtNode>& node) {
        code << "return ";
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_func_def_stmt_node(const std::shared_ptr<FuncDefStmtNode>& node) {
        code << "tachyon_internal::Val " << node->name_tok.val << " = tachyon_internal::make_obj({},Function.obj,new tachyon_internal::func_type([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {\n";
        for (int i = 0; i < node->arg_names.size(); i++) {
            code << "tachyon_internal::Val " << node->arg_names.at(i).val << "= _args.at(" << i << ");\n";
        }
        visit(node->body);
        code << "}))";
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