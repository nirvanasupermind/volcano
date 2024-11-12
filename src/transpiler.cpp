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
        if(d == 0.0) {
            code << "0.0";
        } else {
            std::ostringstream out;
            out.precision(17 - std::floor(std::log10(std::abs(d))));
            out << std::fixed << d;
            code << out.str();
        }
        }
        else {
            switch (node->get_type()) {
            case NodeType::NULL_:
                visit_null_node(std::static_pointer_cast<NullNode>(node));
                break;
            case NodeType::STRING:
                visit_string_node(std::static_pointer_cast<StringNode>(node));
                break;
            case NodeType::VECTOR:
                visit_vector_node(std::static_pointer_cast<VectorNode>(node));
                break;
            case NodeType::OBJECT:
                visit_object_node(std::static_pointer_cast<ObjectNode>(node));
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
            case NodeType::SUBSCRIPT:
                visit_subscript_node(std::static_pointer_cast<SubscriptNode>(node));
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
                // case NodeType::IF_ELSE_STMT:
                //     visit_if_else_stmt_node(std::static_pointer_cast<IfElseStmtNode>(node));
                //     break;
            case NodeType::WHILE_STMT:
                visit_while_stmt_node(std::static_pointer_cast<WhileStmtNode>(node));
                break;
            case NodeType::FOR_STMT:
                visit_for_stmt_node(std::static_pointer_cast<ForStmtNode>(node));
                break;
            case NodeType::CONTINUE_STMT:
                visit_continue_stmt_node(std::static_pointer_cast<ContinueStmtNode>(node));
                break;
            case NodeType::BREAK_STMT:
                visit_break_stmt_node(std::static_pointer_cast<BreakStmtNode>(node));
                break;
            case NodeType::RETURN_STMT:
                visit_return_stmt_node(std::static_pointer_cast<ReturnStmtNode>(node));
                break;
            case NodeType::FUNC_DEF_STMT:
                visit_func_def_stmt_node(std::static_pointer_cast<FuncDefStmtNode>(node));
                break;
            case NodeType::THROW_STMT:
                visit_throw_stmt_node(std::static_pointer_cast<ThrowStmtNode>(node));
                break;
            case NodeType::TRY_CATCH_STMT:
                visit_try_catch_stmt_node(std::static_pointer_cast<TryCatchStmtNode>(node));
                break;
            case NodeType::INCLUDE_STMT:
                visit_include_stmt_node(std::static_pointer_cast<IncludeStmtNode>(node));
                break;
                // case NodeType::DEFINE_STMT:
                //     visit_define_stmt_node(std::static_pointer_cast<DefineStmtNode>(node));
                //     break;
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
        code << "tachyon_internal::make_str(new std::string(\"" << node->tok.val << "\"))";
    }


    void Transpiler::visit_null_node(const std::shared_ptr<NullNode>& node) {
        code << "tachyon_internal::null";
    }

    void Transpiler::visit_vector_node(const std::shared_ptr<VectorNode>& node) {
        code << "tachyon_internal::make_vec(new std::vector<double>({";
        for (int i = 0; i < node->elements.size(); i++) {
            visit(node->elements[i]);
            if (i != node->elements.size() - 1) {
                code << ",";
            }
        }
        code << "}))";
    }

    void Transpiler::visit_object_node(const std::shared_ptr<ObjectNode>& node) {
        code << "tachyon_internal::make_obj(new TACHYON_OBJ({";
        for (int i = 0; i < node->keys.size(); i++) {
            code << "{";
            if (node->keys[i]->get_type() == NodeType::STRING) {
                std::shared_ptr<StringNode> str_node = std::static_pointer_cast<StringNode>(node->keys[i]);
                code << "\"" << str_node->tok.val << "\"";
            }
            else {
                code << "*tachyon_internal::decode_str(";
                visit(node->keys[i]);
                code << ")";
            }

            code << ",";
            visit(node->vals[i]);
            if (i != node->keys.size() - 1) {
                code << "},";
            }
        }
        if (!node->keys.empty()) {
            code << "}";
        }
        code << "}))";
    }

    void Transpiler::visit_subscript_node(const std::shared_ptr<SubscriptNode>& node) {
        code << "tachyon_internal::get_subscript(";
        visit(node->base);
        code << ",";
        visit(node->idx);
        code << ")";
    }

    void Transpiler::visit_anon_func_expr_node(const std::shared_ptr<AnonFuncExprNode>& node) {
        code << "tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {\n";
        for (int i = 0; i < node->arg_names.size(); i++) {
            code << "double " << node->arg_names[i].val << "= _args[" << i << "];\n";
        }
        visit(node->body);
        code << "\nreturn tachyon_internal::null;\n}))";
    }

    void Transpiler::visit_identifier_node(const std::shared_ptr<IdentifierNode>& node) {
        code << node->tok.val;
    }

    void Transpiler::visit_call_expr_node(const std::shared_ptr<CallExprNode>& node) {
        code << "(*tachyon_internal::decode_func(";
        visit(node->callee);
        code << "))({";
        if (node->callee->get_type() == NodeType::OBJECT_PROP) {
            visit(std::static_pointer_cast<ObjectPropNode>(node->callee)->obj);
            if (!node->args.empty()) {
                code << ",";
            }
        }
        for (int i = 0; i < node->args.size(); i++) {
            visit(node->args[i]);
            if (i != node->args.size() - 1) {
                code << ",";
            }
        }
        code << "})";
    }

    void Transpiler::visit_object_prop_node(const std::shared_ptr<ObjectPropNode>& node) {
        code << "tachyon_internal::get_prop(tachyon_internal::decode_obj(";
        visit(node->obj);
        code << "),\"" << node->prop.val << "\")";
    }

    void Transpiler::visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node) {
        if (node->op_tok.type == TokenType::PLUS) {
            visit(node->operand_node);
        }
        else if (node->op_tok.type == TokenType::MINUS) {
            code << "-(";
            visit(node->operand_node);
            code << ")";
        }
        else if (node->op_tok.type == TokenType::NOT) {
            code << "(double)(~(int32_t)(";
            visit(node->operand_node);
            code << "))";
        }
        else if (node->op_tok.type == TokenType::INC || node->op_tok.type == TokenType::DEC) {
            if (node->operand_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->operand_node);
                code << "tachyon_internal::set_prop(tachyon_internal::decode_obj(";
                visit(temp->obj);
                code << "),\"" << temp->prop.val << "\",(";
                visit(node->operand_node);
                code << node->op_tok.val[0];
                code << "1.0))";
            }
            else if (node->operand_node->get_type() == NodeType::SUBSCRIPT) {
                std::shared_ptr<SubscriptNode> temp = std::static_pointer_cast<SubscriptNode>(node->operand_node);
                code << "tachyon_internal::set_subscript(";
                visit(temp->base);
                code << ",";
                visit(temp->idx);
                code << ",(";
                visit(node->operand_node);
                code << node->op_tok.val[0];
                code << "1.0))";
            }
            else {
                visit(node->operand_node);
                code << node->op_tok.val;
            }
        }
    }

    void Transpiler::visit_bin_op_node(const std::shared_ptr<BinOpNode>& node) {
        if (node->op_tok.type == TokenType::EQ) {
            // (*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(obj),"set")))({obj,key,val});
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->left_node);
                code << "tachyon_internal::set_prop(tachyon_internal::decode_obj(";
                visit(temp->obj);
                code << "),\"" << temp->prop.val << "\",(";
                visit(node->right_node);
                code << "))";
            }
            else if (node->left_node->get_type() == NodeType::SUBSCRIPT) {
                std::shared_ptr<SubscriptNode> temp = std::static_pointer_cast<SubscriptNode>(node->left_node);
                code << "tachyon_internal::set_subscript(";
                visit(temp->base);
                code << ",";
                visit(temp->idx);
                code << ",(";
                visit(node->right_node);
                code << "))";
            }
            else {
                visit(node->left_node);
                code << "=";
                visit(node->right_node);
            }
        }
        else if (node->op_tok.type == TokenType::PLUS_EQ || node->op_tok.type == TokenType::MINUS_EQ || node->op_tok.type == TokenType::MUL_EQ
            || node->op_tok.type == TokenType::DIV_EQ) {
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->left_node);
                code << "tachyon_internal::set_prop(tachyon_internal::decode_obj(";
                visit(temp->obj);
                code << "),\"" << temp->prop.val << "\",";
                visit(node->left_node);
                code << node->op_tok.val[0];
                code << "(";
                visit(node->right_node);
                code << "))";
            }
            else if (node->left_node->get_type() == NodeType::SUBSCRIPT) {
                std::shared_ptr<SubscriptNode> temp = std::static_pointer_cast<SubscriptNode>(node->left_node);
                code << "tachyon_internal::set_subscript(";
                visit(temp->base);
                code << ",";
                visit(temp->idx);
                code << ",(";
                visit(node->left_node);
                code << ")";
                code << node->op_tok.val[0];
                code << "(";
                visit(node->right_node);
                code << "))";
            }
            else {
                visit(node->left_node);
                code << node->op_tok.val;
                code << "(";
                visit(node->right_node);
                code << ")";
            }
        }
                else if (node->op_tok.type == TokenType::MOD_EQ) {
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->left_node);
                code << "tachyon_internal::set_prop(tachyon_internal::decode_obj(";
                visit(temp->obj);
                code << "),\"" << temp->prop.val << "\",std::fmod(";
                visit(node->left_node);
                code << ",(";
                visit(node->right_node);
                code << ")))";
            }
            else if (node->left_node->get_type() == NodeType::SUBSCRIPT) {
                std::shared_ptr<SubscriptNode> temp = std::static_pointer_cast<SubscriptNode>(node->left_node);
                code << "tachyon_internal::set_subscript(";
                visit(temp->base);
                code << ",";
                visit(temp->idx);
                code << ",std::fmod((";
                visit(node->left_node);
                code << "),(";
                visit(node->right_node);
                code << ")))";
            }
            else {
                visit(node->left_node);
                code << "=std::fmod(";
                visit(node->left_node);
                code << ",(";
                visit(node->right_node);
                code << "))";
            }
        }
        else if (node->op_tok.type == TokenType::AND_EQ || node->op_tok.type == TokenType::OR_EQ || node->op_tok.type == TokenType::XOR_EQ
            || node->op_tok.type == TokenType::LSH_EQ || node->op_tok.type == TokenType::RSH_EQ) {
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectPropNode> temp = std::static_pointer_cast<ObjectPropNode>(node->left_node);
                code << "tachyon_internal::set_prop(tachyon_internal::decode_obj(";
                visit(temp->obj);
                code << "),\"" << temp->prop.val << "\",(int32_t)(";
                visit(node->left_node);
                code << ")";
                code << node->op_tok.val[0];
                code << "(int32_t)((";
                visit(node->right_node);
                code << ")))";
            }
            else if (node->left_node->get_type() == NodeType::SUBSCRIPT) {
                std::shared_ptr<SubscriptNode> temp = std::static_pointer_cast<SubscriptNode>(node->left_node);
                code << "tachyon_internal::set_subscript(";
                visit(temp->base);
                code << ",";
                visit(temp->idx);
                code << ",(int32_t)(";
                visit(node->left_node);
                code << ")";
                code << node->op_tok.val[0];
                code << "(int32_t)(";
                visit(node->right_node);
                code << "))";
            }
            else {
                code << "(";
                visit(node->left_node);
                code << ")=";
                code << "(int32_t)(";
                visit(node->left_node);
                code << ")" << node->op_tok.val[0] << "(int32_t)(";
                visit(node->right_node);
                code << ")";
            }
        }
        else if (node->op_tok.type == TokenType::AND || node->op_tok.type == TokenType::OR || node->op_tok.type == TokenType::XOR
            || node->op_tok.type == TokenType::LSH || node->op_tok.type == TokenType::RSH) {
            code << "(double)((int32_t)(";
            visit(node->left_node);
            code << ")";
            code << node->op_tok.val;
            code << "(int32_t)(";
            visit(node->right_node);
            code << "))";
        }
        else if (node->op_tok.type == TokenType::EE) {
            code << "tachyon_internal::is_eq((";
            visit(node->left_node);
            code << "),";
            code << "(";
            visit(node->right_node);
            code << "))";
        }
        else if (node->op_tok.type == TokenType::NE) {
            code << "tachyon_internal::is_ne((";
            visit(node->left_node);
            code << "),";
            code << "(";
            visit(node->right_node);
            code << "))";
        }
        else if (node->op_tok.type == TokenType::LT || node->op_tok.type == TokenType::LE
            || node->op_tok.type == TokenType::GT || node->op_tok.type == TokenType::GE) {
            code << "(double)((";
            visit(node->left_node);
            code << ")";
            code << node->op_tok.val;
            code << "(";
            visit(node->right_node);
            code << "))";
        }
        else if (node->op_tok.type == TokenType::MOD) {
            code << "std::fmod((";
            visit(node->left_node);
            code << ")";
            code << ",";
            code << "(";
            visit(node->right_node);
            code << "))";
        }
        else {
            code << "(";
            visit(node->left_node);
            code << ")";
            code << node->op_tok.val;
            code << "(";
            visit(node->right_node);
            code << ")";
        }
    }

    void Transpiler::visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node) {
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_var_def_stmt_node(const std::shared_ptr<VarDefStmtNode>& node) {
        code << "double " << node->name_tok.val << "=";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_const_def_stmt_node(const std::shared_ptr<ConstDefStmtNode>& node) {
        code << "const double " << node->name_tok.val << "=";
        visit(node->val);
        code << ";";
    }


    void Transpiler::visit_block_stmt_node(const std::shared_ptr<BlockStmtNode>& node) {
        code << "{\n";
        visit(node->stmt_list_node);
        code << "}";
    }

    void Transpiler::visit_if_stmt_node(const std::shared_ptr<IfStmtNode>& node) {
        if (node->conds[0]->get_double() != 0.0) {
            code << "if(";
            visit(node->conds[0]);
            code << ")";
            visit(node->bodies[0]);
            for (int i = 1; i < node->conds.size(); i++) {
                code << "else if(";
                visit(node->conds[i]);
                code << ")";
                visit(node->bodies[i]);
            }
            if (node->else_body) {
                code << "else";
                visit(node->else_body);
            }
        }
    }

    // void Transpiler::visit_if_else_stmt_node(const std::shared_ptr<IfElseStmtNode>& node) {
    //     code << "if(";
    //     visit(node->cond);
    //     code << ")";
    //     visit(node->if_body);
    //     code << "else";
    //     visit(node->else_body);
    // }

    void Transpiler::visit_while_stmt_node(const std::shared_ptr<WhileStmtNode>& node) {
        if (node->cond->get_double() != 0.0) {
            code << "while(";
            visit(node->cond);
            code << ")";
            visit(node->body);
        }
    }


    void Transpiler::visit_for_stmt_node(const std::shared_ptr<ForStmtNode>& node) {
        code << "for(";
        visit(node->init);
        code << "(";
        visit(node->cond);
        code << ")";
        code << ";";
        visit(node->update);
        code << ") {";
        visit(node->body);
        code << "}";
    }



    void Transpiler::visit_continue_stmt_node(const std::shared_ptr<ContinueStmtNode>& node) {
        code << "continue;";
    }

    void Transpiler::visit_break_stmt_node(const std::shared_ptr<BreakStmtNode>& node) {
        code << "break;";
    }

    void Transpiler::visit_return_stmt_node(const std::shared_ptr<ReturnStmtNode>& node) {
        code << "return ";
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_func_def_stmt_node(const std::shared_ptr<FuncDefStmtNode>& node) {
        code << "double " << node->name_tok.val << "= tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {\n";
        for (int i = 0; i < node->arg_names.size(); i++) {
            code << "double " << node->arg_names[i].val << "= _args[" << i << "];\n";
        }
        visit(node->body);
        code << "\nreturn tachyon_internal::null;\n}));";
    }

    void Transpiler::visit_throw_stmt_node(const std::shared_ptr<ThrowStmtNode>& node) {
        code << "throw std::runtime_error(*tachyn_internal::decode_str(";
        visit(node->expr_node);
        code << "));";
    }

    void Transpiler::visit_try_catch_stmt_node(const std::shared_ptr<TryCatchStmtNode>& node) {
        code << "try";
        visit(node->try_body);
        code << "catch(const std::exception& _err) {\ndouble " << node->error.val
            << "=tachyon_internal::make_str(new std::string(_err.what()));\n";
        visit(node->catch_body);
        code << "\n}";
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

    // void Transpiler::visit_define_stmt_node(const std::shared_ptr<DefineStmtNode>& node) {
    //     code << "#define " << node->name.val << " ";
    //     visit(node->expr_node);
    // }

    void Transpiler::visit_stmt_list_node(const std::shared_ptr<StmtListNode>& node) {
        for (int i = 0; i < node->stmts.size(); i++) {
            visit(node->stmts[i]);
            code << '\n';
        }
    }
}