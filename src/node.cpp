#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace tachyon {
    uint64_t pack_float(float x) {
        return (*(uint64_t*)(&x) << 2) + 1;
    }

    float unpack_float(uint64_t x) {
        uint64_t temp = x >> 2;
        return *(float*)(&temp);
    }

    uint64_t Node::jit_val() const {
        return 0ULL;
    }
    
    FloatNode::FloatNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType FloatNode::get_type() const {
        return NodeType::FLOAT;
    }

    std::string FloatNode::to_string() const {
        return "(FloatNode " + tok.to_string() + ")";
    }

    uint64_t FloatNode::jit_val() const {
        return pack_float(std::stof(tok.val));
    }

    BoolNode::BoolNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType BoolNode::get_type() const {
        return NodeType::BOOL;
    }

    std::string BoolNode::to_string() const {
        return "(BoolNode " + tok.to_string() + ")";
    }

    uint64_t BoolNode::jit_val() const {
        return (tok.val == "true") ? 10 : 2;
    }

    NullNode::NullNode() {

    }

    NodeType NullNode::get_type() const {
        return NodeType::NULL_;
    }

    std::string NullNode::to_string() const {
        return "(NullNode)";
    }

    uint64_t NullNode::jit_val() const {
        return 6;
    }

   StringNode::StringNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType StringNode::get_type() const {
        return NodeType::STRING;
    }

    std::string StringNode::to_string() const {
        return "(StringNode " + tok.to_string() + ")";
    }
    
    VectorNode::VectorNode(const std::vector<std::shared_ptr<Node> >& elements) {
        this->elements = elements;
    }

    NodeType VectorNode::get_type() const {
        return NodeType::VECTOR;
    }

    std::string VectorNode::to_string() const {
        std::string result = "(VectorNode ";
        for(int i = 0; i < elements.size(); i++) {
            result += elements.at(i)->to_string() + " ";
        }
        result = result.substr(0, result.size() - 1);
        result += ")";
        return result;
    }

    UnorderedMapNode::UnorderedMapNode(const std::vector<std::shared_ptr<Node> >& keys, const std::vector<std::shared_ptr<Node> >& vals) {
        this->keys = keys;
        this->vals = vals;
    }

    NodeType UnorderedMapNode::get_type() const {
        return NodeType::UNORDERED_MAP;
    }

    std::string UnorderedMapNode::to_string() const {
        std::string result = "(UnorderedMap (";
        for(int i = 0; i < keys.size(); i++) {
            result += keys.at(i)->to_string() + " ";
        }
        result = result.substr(0, result.size() - 1);
        result += ") (";
        for(int i = 0; i < vals.size(); i++) {
            result += vals.at(i)->to_string() + " ";
        }
        result = result.substr(0, result.size() - 1);
        result += "))";
        return result;
    }

    IdentifierNode::IdentifierNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType IdentifierNode::get_type() const {
        return NodeType::IDENTIFIER;
    }

    std::string IdentifierNode::to_string() const {
        return "(IdentifierNode " + tok.to_string() + ")";
    }

    LambdaExprNode::LambdaExprNode(const std::vector<Token>& arg_names, const std::shared_ptr<Node>& body) {
        this->arg_names = arg_names;
        this->body = body;
    }

    NodeType LambdaExprNode::get_type() const {
        return NodeType::LAMBDA_EXPR;
    }

    std::string LambdaExprNode::to_string() const {
        std::string result = "(LambdaExprNode ";
            for(int i = 0; i < arg_names.size(); i++) {
            result += arg_names.at(i).to_string() + " ";
        }
        result += body->to_string();
        return result;
    }

    CallExprNode::CallExprNode(const std::shared_ptr<Node>& callee, const std::vector<std::shared_ptr<Node> >& args) {
        this->callee = callee;
        this->args = args;
    }

    NodeType CallExprNode::get_type() const {
        return NodeType::CALL_EXPR;
    }

    std::string CallExprNode::to_string() const {
        std::string result = "(CallExprNode ";
        for(int i = 0; i < args.size(); i++) {
            result += args.at(i)->to_string() + " ";
        }
        result = result.substr(0, result.size() - 1);
        result += ")";
        return result;
    }

    UnaryOpNode::UnaryOpNode(const Token& op_tok, const std::shared_ptr<Node>& right_node) {
        this->op_tok = op_tok;
        this->right_node = right_node;
    }

    NodeType UnaryOpNode::get_type() const {
        return NodeType::UNARY_OP;
    }

    std::string UnaryOpNode::to_string() const {
        return "(UnaryOpNode " + this->op_tok.to_string() + " " + this->right_node->to_string() + ")";
    }

    uint64_t UnaryOpNode::jit_val() const {
        uint64_t right = right_node->jit_val();
        if(right == 0) {
            return 0;
        } else if(op_tok.type == TokenType::PLUS) {
            return right;
        } else if(op_tok.type == TokenType::MINUS) {
            return pack_float(-unpack_float(right));
        } else if(op_tok.type == TokenType::NOT) {
            return pack_float(~(int32_t)(unpack_float(right)));
        } else if(op_tok.type == TokenType::LOGICAL_NOT) {
            return (right == 10) ? 2 : 10;
        } else {
            return 0;
        }
    }

    BinOpNode::BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node) {
        this->left_node = left_node;
        this->op_tok = op_tok;
        this->right_node = right_node;
    }

    NodeType BinOpNode::get_type() const {
        return NodeType::BIN_OP;
    }

    std::string BinOpNode::to_string() const {
        return "(BinOpNode " + this->op_tok.to_string() + " " + this->left_node->to_string() + " " + this->right_node->to_string() + ")";
    }

    uint64_t BinOpNode::jit_val() const {
        uint64_t left = left_node->jit_val();
        uint64_t right = right_node->jit_val();

        if(left == 0 || right == 0) {
            return 0;
        }
        
        switch(op_tok.type) {
        case TokenType::PLUS:
            return pack_float(unpack_float(left) + unpack_float(right));
        case TokenType::MINUS:
            return pack_float(unpack_float(left) - unpack_float(right));
        case TokenType::MUL:
            return pack_float(unpack_float(left) * unpack_float(right));
        case TokenType::DIV:
            return pack_float(unpack_float(left) / unpack_float(right));
        case TokenType::MOD:
            return pack_float(std::fmod(unpack_float(left), unpack_float(right)));
        case TokenType::AND:
            return pack_float((int32_t)(unpack_float(left)) & (int32_t)(unpack_float(right)));
        case TokenType::LT:
            return (unpack_float(left) < unpack_float(right)) ? 10 : 2;
        case TokenType::EQ:
            return (left == right) ? 10 : 2;
        default:
            return 0;
        }
    }

    VarDefStmtNode::VarDefStmtNode(const Token& name_tok, const std::shared_ptr<Node>& val) {
        this->name_tok = name_tok;
        this->val = val;
    }

    NodeType VarDefStmtNode::get_type() const {
        return NodeType::VAR_DEF_STMT;
    }

    std::string VarDefStmtNode::to_string() const {
        return "(VarDefNode " + name_tok.to_string() + " " + val->to_string() + ")";
    }

    ConstDefStmtNode::ConstDefStmtNode(const Token& name_tok, const std::shared_ptr<Node>& val) {
        this->name_tok = name_tok;
        this->val = val;
    }

    NodeType ConstDefStmtNode::get_type() const {
        return NodeType::CONST_DEF_STMT;
    }

    std::string ConstDefStmtNode::to_string() const {
        return "(ConstDefStmtNode " + name_tok.to_string() + " " + val->to_string() + ")";
    }

    // VarAssignNode::VarAssignNode(const Token& name_tok, const std::shared_ptr<Node>& val) {
    //     this->name_tok = name_tok;
    //     this->val = val;
    // }

    // NodeType VarAssignNode::get_type() const {
    //     return NodeType::VAR_ASSIGN;
    // }

    // std::string VarAssignNode::to_string() const {
    //     return "(VarAssignNode " + name_tok.to_string() + " " + val->to_string() + ")";
    // }


    ExprStmtNode::ExprStmtNode(const std::shared_ptr<Node>& expr_node) {
        this->expr_node = expr_node;
    }

    NodeType ExprStmtNode::get_type() const {
        return NodeType::EXPR_STMT;
    }

    std::string ExprStmtNode::to_string() const {
        return "(ExprStmtNode " + expr_node->to_string() + ")";
    }

    BlockStmtNode::BlockStmtNode(const std::shared_ptr<Node>& stmt_list_node) {
        this->stmt_list_node = stmt_list_node;
    }

    NodeType BlockStmtNode::get_type() const {
        return NodeType::BLOCK_STMT;
    }

    std::string BlockStmtNode::to_string() const {
        return "(BlockStmtNode " + stmt_list_node->to_string() + ")";
    }

    IfStmtNode::IfStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& body) {
        this->cond = cond;
        this->body = body;
    }

    NodeType IfStmtNode::get_type() const {
        return NodeType::IF_STMT;
    }

    std::string IfStmtNode::to_string() const {
        return "(IfStmtNode " + cond->to_string() + " " + body->to_string() + ")";
    }

    IfElseStmtNode::IfElseStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& if_body, const std::shared_ptr<Node>& else_body) {
        this->cond = cond;
        this->if_body = if_body;
        this->else_body = else_body;
    }

    NodeType IfElseStmtNode::get_type() const {
        return NodeType::IF_ELSE_STMT;
    }

    std::string IfElseStmtNode::to_string() const {
        return "(IfElseStmtNode " + cond->to_string() + " " + if_body->to_string() + " " + else_body->to_string()  + ")";
    }

    WhileStmtNode::WhileStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& body) {
        this->cond = cond;
        this->body = body;
    }

    NodeType WhileStmtNode::get_type() const {
        return NodeType::WHILE_STMT;
    }

    std::string WhileStmtNode::to_string() const {
        return "(WhileStmtNode " + cond->to_string() + " " + body->to_string() + ")";
    }

    ForStmtNode::ForStmtNode(const std::shared_ptr<Node>& init, const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& update, const std::shared_ptr<Node>& body) {
        this->init = init;
        this->cond = cond;
        this->update = update;
        this->body = body;
    }

    NodeType ForStmtNode::get_type() const {
        return NodeType::FOR_STMT;
    }

    std::string ForStmtNode::to_string() const {
        return "(ForStmtNode " + init->to_string() + " " + cond->to_string()  + " " + update->to_string()  + " " + body->to_string() + ")";
    }

    ReturnStmtNode::ReturnStmtNode(const std::shared_ptr<Node>& expr_node) {
        this->expr_node = expr_node;
    }

    NodeType ReturnStmtNode::get_type() const {
        return NodeType::RETURN_STMT;
    }

    std::string ReturnStmtNode::to_string() const {
        return "(ReturnStmtNode " + expr_node->to_string() + ")";
    }
    
    FuncDefStmtNode::FuncDefStmtNode(const Token& name_tok, const std::vector<Token>& arg_names, const std::shared_ptr<Node>& body) {
        this->name_tok = name_tok;
        this->arg_names = arg_names;
        this->body = body;
    }

    NodeType FuncDefStmtNode::get_type() const {
        return NodeType::FUNC_DEF_STMT;
    }

    std::string FuncDefStmtNode::to_string() const {
        std::string result = "(FuncDefStmtNode " + name_tok.to_string() + " ";
            for(int i = 0; i < arg_names.size(); i++) {
            result += arg_names.at(i).to_string() + " ";
        }
        result += body->to_string();
        return result;
    }


    ObjectMemberNode::ObjectMemberNode(const std::shared_ptr<Node>& obj, const Token& prop) {
        this->obj = obj;
        this->prop = prop;
    }

    NodeType ObjectMemberNode::get_type() const {
        return NodeType::OBJECT_PROP;
    }

    std::string ObjectMemberNode::to_string() const {
        return "(ObjectPropNode  " + obj->to_string() + " " + prop.to_string() + ")";
    }

    TryCatchStmtNode::TryCatchStmtNode(const std::shared_ptr<Node>& try_body, const Token& error, const std::shared_ptr<Node>& catch_body) {
        this->try_body = try_body;
        this->error = error;
        this->catch_body = catch_body;
    }

    NodeType TryCatchStmtNode::get_type() const {
        return NodeType::TRY_CATCH_STMT;
    }

    std::string TryCatchStmtNode::to_string() const {
        return "(TryCatchStmtNode  " + try_body->to_string() + " " + error.to_string() + " " + catch_body->to_string() + ")";
    }

    IncludeStmtNode::IncludeStmtNode(const Token& path) {
        this->path = path;
    }

    NodeType IncludeStmtNode::get_type() const {
        return NodeType::INCLUDE_STMT;
    }

    std::string IncludeStmtNode::to_string() const {
        return "(IncludeStmtNode " + path.to_string() + ")";
    }

    StmtListNode::StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts) {
        this->stmts = stmts;
    }

    NodeType StmtListNode::get_type() const {
        return NodeType::STMT_LIST;
    }

    std::string StmtListNode::to_string() const {
        std::string result = "(StmtListNode ";
        for(int i = 0; i < stmts.size(); i++) {
            result += stmts.at(i)->to_string() + " ";
        }
        result = result.substr(0, result.size() - 1);
        result += ")";
        return result;
    }
};

