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
        if(node->jit_val() != 0) {
            code << node->jit_val() << "ULL";
        } else {
        switch (node->get_type()) {
        // case NodeType::FLOAT:
        //     visit_number_node(std::static_pointer_cast<FloatNode>(node));
        //     break;
        // case NodeType::BOOL:
        //     visit_bool_node(std::static_pointer_cast<BoolNode>(node));
        //     break;
        // case NodeType::NULL_:
        //     visit_null_node(std::static_pointer_cast<NullNode>(node));
        //     break;
        case NodeType::STRING:
            visit_string_node(std::static_pointer_cast<StringNode>(node));
            break;
        case NodeType::VECTOR:
            visit_vector_node(std::static_pointer_cast<VectorNode>(node));
            break;
        case NodeType::UNORDERED_MAP:
            visit_unordered_map_node(std::static_pointer_cast<UnorderedMapNode>(node));
            break;;
        case NodeType::IDENTIFIER:
            visit_identifier_node(std::static_pointer_cast<IdentifierNode>(node));
            break;
        case NodeType::LAMBDA_EXPR:
            visit_lambda_expr_node(std::static_pointer_cast<LambdaExprNode>(node));
            break;
        case NodeType::CALL_EXPR:
            visit_call_expr_node(std::static_pointer_cast<CallExprNode>(node));
            break;
        case NodeType::OBJECT_PROP:
            visit_object_member_node(std::static_pointer_cast<ObjectMemberNode>(node));
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
        case NodeType::TRY_CATCH_STMT:
            visit_try_catch_stmt_node(std::static_pointer_cast<TryCatchStmtNode>(node));
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
    // void Transpiler::visit_number_node(const std::shared_ptr<FloatNode>& node) {
    //     float x = std::stof(node->tok.val);;
    //     code << (*(uint64_t*)(&x) << 2) + 1 << "ULL";
    // }

    // void Transpiler::visit_bool_node(const std::shared_ptr<BoolNode>& node) {
    //     if(node->tok.val == "false") {
    //         code << "2ULL";
    //     } else {
    //         code << "10ULL";
    //     }
    // }

    // void Transpiler::visit_null_node(const std::shared_ptr<NullNode>& node) {
    //     code << "6ULL";
    // }

    void Transpiler::visit_string_node(const std::shared_ptr<StringNode>& node) {
        code << "create_object(new std::map<std::string, uint64_t>({{\"prototype\",String}}),new std::string(\"" + node->tok.val << "\"))";
    }

    void Transpiler::visit_vector_node(const std::shared_ptr<VectorNode>& node) {
        code << "create_object(new std::map<std::string, uint64_t>({{\"prototype\",Vector}}), new std::vector<uint64_t>({";
        if (node->elements.size() == 0) {
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

    void Transpiler::visit_unordered_map_node(const std::shared_ptr<UnorderedMapNode>& node) {
        code << "create_object(new std::map<std::string, uint64_t>({{\"prototype\",UnorderedMap}})," << "new std::map<std::string, uint64_t>({";
        if (node->keys.size() == 0) {
            code << "}))";
        }
        else {
            for (int i = 0; i < node->keys.size(); i++) {
                code << "{";
                if(node->keys.at(i)->get_type() == NodeType::STRING) {
                    code << "\"" << std::static_pointer_cast<StringNode>(node->keys.at(i))->tok.val << "\"";
                } else {
                    code << "*(std::string*)(unpack_object(";
                    visit(node->keys.at(i));
                    code << ")->hidden_data)";
                }

                code << ",";

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

    void Transpiler::visit_identifier_node(const std::shared_ptr<IdentifierNode>& node) {
        code << node->tok.val;
    }

    void Transpiler::visit_call_expr_node(const std::shared_ptr<CallExprNode>& node) {
        code << "(*(func_ptr*)(unpack_object(";
        visit(node->callee);
        code << ")->hidden_data))({";
        if (node->callee->get_type() == NodeType::OBJECT_PROP) {
            std::shared_ptr<ObjectMemberNode> obj_prop_node = std::static_pointer_cast<ObjectMemberNode>(node->callee);
            if (node->args.size() == 0) {
                visit(obj_prop_node->obj);
                code << "})";
            }
            else {
                visit(obj_prop_node->obj);
                code << ",";
                for (int i = 0; i < node->args.size(); i++) {
                    visit(node->args.at(i));
                    if (i == node->args.size() - 1) {
                        code << "})";
                    }
                    else {
                        code << ",";
                    }
                }

            }
        } else {

        if (node->args.size() == 0) {
            code << "})";
        }
        else {
            for (int i = 0; i < node->args.size(); i++) {
                visit(node->args.at(i));
                if (i == node->args.size() - 1) {
                    code << "})";
                }
                else {
                    code << ",";
                }
            }
        }
    }
    }

    void Transpiler::visit_object_member_node(const std::shared_ptr<ObjectMemberNode>& node) {
        code << "unpack_object(";
        visit(node->obj);
        code << ")->get(\"" << node->prop.val << "\")";
    }

    void Transpiler::visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node) {
        if (node->op_tok.type == TokenType::INC) {
            if (node->right_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectMemberNode> obj_prop_node = std::static_pointer_cast<ObjectMemberNode>(node->right_node);
                code << "(*(unpack_object(";
                visit(obj_prop_node->obj);
                code << ")->props))[\"" << obj_prop_node->prop.val << "\"] =";
            }
            else {
                visit(node->right_node);
                code << " = ";
            }
            code << "pack_number(unpack_number(";
            visit(node->right_node);
            code << ") + 1.0f)";
        }
        else if (node->op_tok.type == TokenType::DEC) {
            if (node->right_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectMemberNode> obj_prop_node = std::static_pointer_cast<ObjectMemberNode>(node->right_node);
                code << "(*(unpack_object(";
                visit(obj_prop_node->obj);
                code << ")->props))[\"" << obj_prop_node->prop.val << "\"] =";
            }
            else {
                visit(node->right_node);
                code << " = ";
            }
            code << "pack_number(unpack_number(";
            visit(node->right_node);
            code << ") + 1.0f)";
        }
        else {
            code << "pack_number(";
            code << node->op_tok.val;
            code << "unpack_number(";
            visit(node->right_node);
            code << "))";
        }
    }

    void Transpiler::visit_bin_op_node(const std::shared_ptr<BinOpNode>& node) {
        if (node->op_tok.type == TokenType::EQ) {
            if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
                std::shared_ptr<ObjectMemberNode> obj_prop_node = std::static_pointer_cast<ObjectMemberNode>(node->left_node);
                code << "(*(unpack_object(";
                visit(obj_prop_node->obj);
                code << ")->props))[\"" << obj_prop_node->prop.val << "\"] =";
                visit(node->right_node);
            }
            else {
                visit(node->left_node);
                code << node->op_tok.val;
                visit(node->right_node);
            }
        }
        else if (node->op_tok.type == TokenType::PLUS_EQ) {
            visit_in_place_bin_op_node(node, "+");
        }
        else if (node->op_tok.type == TokenType::MINUS_EQ) {
            visit_in_place_bin_op_node(node, "-");
        }
        else if (node->op_tok.type == TokenType::MUL_EQ) {
            visit_in_place_bin_op_node(node, "*");
        }
        else if (node->op_tok.type == TokenType::DIV_EQ) {
            visit_in_place_bin_op_node(node, "/");
        }
        else if (node->op_tok.type == TokenType::MOD_EQ) {
            visit_in_place_bin_op_node(node, "%");
        }
        else if (node->op_tok.type == TokenType::AND_EQ) {
            visit_in_place_bin_op_node(node, "&");
        }
        else if (node->op_tok.type == TokenType::OR_EQ) {
            visit_in_place_bin_op_node(node, "|");
        }
        else if (node->op_tok.type == TokenType::XOR_EQ) {
            visit_in_place_bin_op_node(node, "^");
        }
        else if (node->op_tok.type == TokenType::LSH_EQ) {
            visit_in_place_bin_op_node(node, "<<");
        }
        else if (node->op_tok.type == TokenType::RSH_EQ) {
            visit_in_place_bin_op_node(node, ">>");
        }
        else if (node->op_tok.type == TokenType::AND || node->op_tok.type == TokenType::OR || node->op_tok.type == TokenType::XOR
            || node->op_tok.type == TokenType::LSH || node->op_tok.type == TokenType::RSH) {
            code << "pack_number((float)((int32_t)unpack_number(";
            visit(node->left_node);
            code << ")" << node->op_tok.val << "(int32_t)unpack_number(";
            visit(node->right_node);
            code << ")))";
        }
        else if(node->op_tok.type == TokenType::EE || node->op_tok.type == TokenType::NE) {
             code << "((";
            visit(node->left_node);
            code << node->op_tok.val;
            visit(node->right_node);
            code << ") ? 10ULL : 2ULL)";   
        }
        else if(node->op_tok.type == TokenType::LT || node->op_tok.type == TokenType::LE
            || node->op_tok.type == TokenType::GT || node->op_tok.type == TokenType::GE) {
             code << "(unpack_number(";
            visit(node->left_node);
            code << ")";
            code << node->op_tok.val;
            code << "unpack_number(";
            visit(node->right_node);
            code << ") ? 10ULL : 2ULL)";   
        }
        else {
            code << "pack_number(unpack_number(";
            visit(node->left_node);
            code << ")" << node->op_tok.val << "unpack_number(";
            visit(node->right_node);
            code << "))";
        }
    }

    void Transpiler::visit_in_place_bin_op_node(const std::shared_ptr<BinOpNode>& node, const std::string& op) {
        if (node->left_node->get_type() == NodeType::OBJECT_PROP) {
            std::shared_ptr<ObjectMemberNode> obj_prop_node = std::static_pointer_cast<ObjectMemberNode>(node->left_node);
            code << "(*(unpack_object(";
            visit(obj_prop_node->obj);
            code << ")->props))[\"" << obj_prop_node->prop.val << "\"] =";
        }
        else {
            visit(node->left_node);
            code << "=";
        }
        if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>") {
            code << "pack_number((float)((int32_t)unpack_number(";
            visit(node->left_node);
            code << ") " << op << " (int32_t)unpack_number(";
            visit(node->right_node);
            code << ")))";
        }
        else if (op == "%") {
            code << "pack_number(std::fmod(unpack_number(";
            visit(node->left_node);
            code << "), unpack_number(";
            visit(node->right_node);
            code << ")))";
        }
        else {
            code << "pack_number(unpack_number(";
            visit(node->left_node);
            code << ") " << op << " unpack_number(";
            visit(node->right_node);
            code << "))";
        }
    }


    void Transpiler::visit_lambda_expr_node(const std::shared_ptr<LambdaExprNode>& node) {
        code << "create_object(new std::map<std::string, uint64_t>({}), new func_ptr([=] (const std::vector<uint64_t>& _args) -> uint64_t {\n";
        for (int i = 0; i < node->arg_names.size(); i++) {
            code << "uint64_t " << node->arg_names.at(i).val << "= _args.at(" << i << ");\n";
        }
        visit(node->body);
        code << "\nreturn 1ULL;\n}))";
    }

   void Transpiler::visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node) {
        visit(node->expr_node);
        code << ";";
    }
    
    void Transpiler::visit_var_def_stmt_node(const std::shared_ptr<VarDefStmtNode>& node) {
        code << "uint64_t " << node->name_tok.val << " = ";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_const_def_stmt_node(const std::shared_ptr<ConstDefStmtNode>& node) {
        code << "const uint64_t " << node->name_tok.val << " = ";
        visit(node->val);
        code << ";";
    }

    void Transpiler::visit_block_stmt_node(const std::shared_ptr<BlockStmtNode>& node) {
        code << "{\n";
        visit(node->stmt_list_node);
        code << "}";
    }

    void Transpiler::visit_if_stmt_node(const std::shared_ptr<IfStmtNode>& node) {
        if(node->cond->jit_val() != 2) {
            code << "if((";
            visit(node->cond);
            code << ") != 2ULL)";
            visit(node->body);
        }
    }

    void Transpiler::visit_if_else_stmt_node(const std::shared_ptr<IfElseStmtNode>& node) {
        if(node->cond->jit_val() != 2) {
            code << "if((";
            visit(node->cond);
            code << ") != 2ULL)";
            visit(node->if_body);
            code << "else";
        }
        visit(node->else_body);
    }

    void Transpiler::visit_while_stmt_node(const std::shared_ptr<WhileStmtNode>& node) {
        if(node->cond->jit_val() != 2) {
            code << "while((";
            visit(node->cond);
            code << ") != 2ULL)";
            visit(node->body);
        }
    }

    void Transpiler::visit_for_stmt_node(const std::shared_ptr<ForStmtNode>& node) {
        code << "for(";
        visit(node->init);
        code << "(";
        visit(node->cond);
        code << ") != 2ULL;";
        visit(node->update);
        code << ")";
        visit(node->body);
    }

    void Transpiler::visit_return_stmt_node(const std::shared_ptr<ReturnStmtNode>& node) {
        code << "return ";
        visit(node->expr_node);
        code << ";";
    }

    void Transpiler::visit_func_def_stmt_node(const std::shared_ptr<FuncDefStmtNode>& node) {
        code << "uint64_t " << node->name_tok.val << " = ";
        code << "create_object(new std::map<std::string, uint64_t>({}), new func_ptr([=] (const std::vector<uint64_t>& _args) {\n";
        for (int i = 0; i < node->arg_names.size(); i++) {
            code << "uint64_t " << node->arg_names.at(i).val << "= _args.at(" << i << ");\n";
        }
        visit(node->body);
        code << "\nreturn 1ULL;\n}));";
    }

 void Transpiler::visit_try_catch_stmt_node(const std::shared_ptr<TryCatchStmtNode>& node) {
        code << "try {\n";
        visit(node->try_body);
        code << "\n} catch(const std::exception& _err) {\n";
        code << "uint64_t msg = create_object(new std::map<std::string, uint64_t>({{\"prototype\",String}}), new std::string(_err.what()));\n";
        code << "uint64_t " << node->error.val << "= create_object(new std::map<std::string, uint64_t>({{\"prototype\",Error},{\"msg\",msg}}));\n";
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

    void Transpiler::visit_stmt_list_node(const std::shared_ptr<StmtListNode>& node) {
        for (int i = 0; i < node->stmts.size(); i++) {
            visit(node->stmts.at(i));
            code << '\n';
        }
    }
}