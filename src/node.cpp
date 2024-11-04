#include <string>
#include <memory>
#include "token.h"
#include "node.h"

namespace tachyon {
    double Node::get_double() const {
        return NAN;
    }
    
    NumberNode::NumberNode(const Token& tok) {
        this->tok = tok;
    }

    NodeType NumberNode::get_type() const {
        return NodeType::NUMBER;
    }

    std::string NumberNode::to_string() const {
        return "(NumberNode " + tok.to_string() + ")";
    }

    double NumberNode::get_double() const {
        return std::stod(tok.val);
    }

        
    NullNode::NullNode() {
    }

    NodeType NullNode::get_type() const {
        return NodeType::NULL_;
    }

    std::string NullNode::to_string() const {
        return "(NullNode)";
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

    ObjectNode::ObjectNode(const std::vector<std::shared_ptr<Node> >& keys, const std::vector<std::shared_ptr<Node> >& vals) {
        this->keys = keys;
        this->vals = vals;
    }

    NodeType ObjectNode::get_type() const {
        return NodeType::OBJECT;
    }

    std::string ObjectNode::to_string() const {
        std::string result = "(Object (";
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

    AnonFuncExprNode::AnonFuncExprNode(const std::vector<Token>& arg_names, const std::shared_ptr<Node>& body) {
        this->arg_names = arg_names;
        this->body = body;
    }

    NodeType AnonFuncExprNode::get_type() const {
        return NodeType::ANON_FUNC_EXPR;
    }

    std::string AnonFuncExprNode::to_string() const {
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
        this->operand_node = right_node;
    }

    NodeType UnaryOpNode::get_type() const {
        return NodeType::UNARY_OP;
    }

    std::string UnaryOpNode::to_string() const {
        return "(UnaryOpNode " + this->op_tok.to_string() + " " + this->operand_node->to_string() + ")";
    }

    double UnaryOpNode::get_double() const {
        uint64_t right = operand_node->get_double();
        if(std::isnan(right)) {
            return NAN;
        } else if(op_tok.type == TokenType::PLUS) {
            return right;
        } else if(op_tok.type == TokenType::MINUS) {
            return -right;
        } else if(op_tok.type == TokenType::NOT) {
            return ~(int32_t)right;
        } else {
            return NAN;
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

    double BinOpNode::get_double() const {
        double left = left_node->get_double();
        double right = right_node->get_double();

        if(std::isnan(left) || std::isnan(right)) {
            return NAN;
        }
        
        switch(op_tok.type) {
        case TokenType::PLUS:
            return left + right;
        case TokenType::MINUS:
            return left - right;
        case TokenType::MUL:
            return left * right;
        case TokenType::DIV:
            return left / right;
        case TokenType::MOD:
            return std::fmod(left, right);
        case TokenType::AND:
            return (int32_t)left & (int32_t)right;
        case TokenType::OR:
            return (int32_t)left | (int32_t)right;
        case TokenType::XOR:
            return (int32_t)left ^ (int32_t)right;
        case TokenType::LSH:
            return (int32_t)left << (int32_t)right;
        case TokenType::RSH:
            return (int32_t)left >> (int32_t)right;
        case TokenType::EE:
            return left == right;
        case TokenType::NE:
            return left != right;
        case TokenType::LT:
            return left < right;
        case TokenType::LE:
            return left <= right;
        case TokenType::GT:
            return left >= right;
        case TokenType::GE:
            return left >= right;
        default:
            return NAN;
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

    IfStmtNode::IfStmtNode(const std::vector<std::shared_ptr<Node> >& conds, const std::vector<std::shared_ptr<Node> >& bodies, const std::shared_ptr<Node>& else_body) {
        this->conds = conds;
        this->bodies = bodies;
        this->else_body = else_body;
    }

    NodeType IfStmtNode::get_type() const {
        return NodeType::IF_STMT;
    }

    std::string IfStmtNode::to_string() const {
        std::string result = "(IfStmtNode (";
        for(int i = 0; i < conds.size(); i++) {
            result += conds.at(i)->to_string();
            result += " ";
            result += bodies.at(i)->to_string();
            result += ") ";
        }
        if(else_body) {
            result += else_body->to_string();
        }
        result += ")";
        return result;
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

    ThrowStmtNode::ThrowStmtNode(const std::shared_ptr<Node>& expr_node) {
        this->expr_node = expr_node;
    }

    NodeType ThrowStmtNode::get_type() const {
        return NodeType::THROW_STMT;
    }

    std::string ThrowStmtNode::to_string() const {
        return "(ThrowStmtNode " + expr_node->to_string() + ")";
    }

    ObjectPropNode::ObjectPropNode(const std::shared_ptr<Node>& obj, const Token& prop) {
        this->obj = obj;
        this->prop = prop;
    }

    NodeType ObjectPropNode::get_type() const {
        return NodeType::OBJECT_PROP;
    }

    std::string ObjectPropNode::to_string() const {
        return "(ObjectPropNode  " + obj->to_string() + " " + prop.to_string() + ")";
    }

    SubscriptNode::SubscriptNode(const std::shared_ptr<Node>& obj, const std::shared_ptr<Node>& idx) {
        this->base = obj;
        this->idx = idx;
    }

    NodeType SubscriptNode::get_type() const {
        return NodeType::SUBSCRIPT;
    }

    std::string SubscriptNode::to_string() const {
        return "(IndexExprNode  " + base->to_string() + " " + idx->to_string() + ")";
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

    // DefineStmtNode::DefineStmtNode(const Token& name, const std::shared_ptr<Node>& expr_node) {
    //     this->name = name;
    //     this->expr_node = expr_node;
    // }

    // NodeType DefineStmtNode::get_type() const {
    //     return NodeType::DEFINE_STMT;
    // }

    // std::string DefineStmtNode::to_string() const {
    //     return "(DefineStmtNode " + name.val + " " + expr_node->to_string() + ")";
    // }

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

