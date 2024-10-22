#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <vector>
#include "token.h"

namespace tachyon {
    enum class NodeType {
        FLOAT,
        BOOL,
        NULL_,
        STRING,
        VECTOR,
        UNORDERED_MAP,
        IDENTIFIER,
        LAMBDA_EXPR,
        CALL_EXPR,
        OBJECT_PROP,
        UNARY_OP,
        BIN_OP,
        VAR_DEF_STMT,
        CONST_DEF_STMT,
        EXPR_STMT,
        BLOCK_STMT,
        IF_STMT,
        IF_ELSE_STMT,
        WHILE_STMT,
        FOR_STMT,
        RETURN_STMT,
        FUNC_DEF_STMT,
        TRY_CATCH_STMT,
        INCLUDE_STMT,
        STMT_LIST
    };

    uint64_t pack_float(float x);
    float unpack_float(uint64_t x);

    class Node {
    public:
        virtual NodeType get_type() const = 0;
        virtual std::string to_string() const = 0;
        virtual uint64_t jit_val() const;
    };

    class FloatNode: public Node {
    public:
        Token tok;
        FloatNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
        uint64_t jit_val() const;
    };

    class BoolNode: public Node {
    public:
        Token tok;
        BoolNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
        uint64_t jit_val() const;
    };

    class NullNode: public Node {
    public:
        NullNode();
        NodeType get_type() const;
        std::string to_string() const;
        uint64_t jit_val() const;
    };

    class StringNode: public Node {
    public:
        Token tok;
        StringNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class VectorNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > elements;
        VectorNode(const std::vector<std::shared_ptr<Node> >& elements);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class UnorderedMapNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > keys;
        std::vector<std::shared_ptr<Node> > vals;
        UnorderedMapNode(const std::vector<std::shared_ptr<Node> >& keys, const std::vector<std::shared_ptr<Node> >& vals);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class IdentifierNode: public Node {
    public:
        Token tok;
        IdentifierNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class LambdaExprNode: public Node {
    public:
        std::vector<Token> arg_names;
        std::shared_ptr<Node> body;
        LambdaExprNode(const std::vector<Token>& arg_names, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class CallExprNode: public Node {
    public:
        std::shared_ptr<Node> callee;
        std::vector<std::shared_ptr<Node> > args;
        CallExprNode(const std::shared_ptr<Node>& callee, const std::vector<std::shared_ptr<Node> >& args);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class ObjectMemberNode: public Node {
    public:
        std::shared_ptr<Node> obj;
        Token prop;
        ObjectMemberNode(const std::shared_ptr<Node>& obj, const Token& prop);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class UnaryOpNode: public Node {
    public:
        Token op_tok;
        std::shared_ptr<Node> right_node;
        UnaryOpNode(const Token& op_tok, const std::shared_ptr<Node>& right_node);
        NodeType get_type() const;
        std::string to_string() const;
        uint64_t jit_val() const;
    };

    class BinOpNode: public Node {
    public:
        std::shared_ptr<Node> left_node;
        Token op_tok;
        std::shared_ptr<Node> right_node;
        BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node);
        NodeType get_type() const;
        std::string to_string() const;
        uint64_t jit_val() const;
    };

    class VarDefStmtNode: public Node {
    public:
        Token name_tok;
        std::shared_ptr<Node> val;
        VarDefStmtNode(const Token& name_tok, const std::shared_ptr<Node>& val);
        NodeType get_type() const;
        std::string to_string() const;
    };


    class ConstDefStmtNode: public Node {
    public:
        Token name_tok;
        std::shared_ptr<Node> val;
        ConstDefStmtNode(const Token& name_tok, const std::shared_ptr<Node>& val);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class ExprStmtNode: public Node {
    public:
        std::shared_ptr<Node> expr_node;
        ExprStmtNode(const std::shared_ptr<Node>& expr_node);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class BlockStmtNode: public Node {
    public:
        std::shared_ptr<Node> stmt_list_node;
        BlockStmtNode(const std::shared_ptr<Node>& stmt_list_node);
        NodeType get_type() const;
        std::string to_string() const;
    };
    
    class IfStmtNode: public Node {
    public:
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> body;
        IfStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class IfElseStmtNode: public Node {
    public:
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> if_body;
        std::shared_ptr<Node> else_body;
        IfElseStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& if_body,  const std::shared_ptr<Node>& else_body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class WhileStmtNode: public Node {
    public:
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> body;
        WhileStmtNode(const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class ForStmtNode: public Node {
    public:
        std::shared_ptr<Node> init;
        std::shared_ptr<Node> cond;
        std::shared_ptr<Node> update;
        std::shared_ptr<Node> body;
        ForStmtNode(const std::shared_ptr<Node>& init, const std::shared_ptr<Node>& cond, const std::shared_ptr<Node>& update, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class ReturnStmtNode: public Node {
    public:
        std::shared_ptr<Node> expr_node;
        ReturnStmtNode(const std::shared_ptr<Node>& expr_node);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class FuncDefStmtNode: public Node {
    public:
        Token name_tok;
        std::vector<Token> arg_names;
        std::shared_ptr<Node> body;
        FuncDefStmtNode(const Token& name_tok, const std::vector<Token>& arg_names, const std::shared_ptr<Node>& body);
        NodeType get_type() const;
        std::string to_string() const;
    };


    class TryCatchStmtNode: public Node {
    public:
        std::shared_ptr<Node> try_body;
        Token error;
        std::shared_ptr<Node> catch_body;
        TryCatchStmtNode(const std::shared_ptr<Node>& try_body, const Token& error,  const std::shared_ptr<Node>& catch_body);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class IncludeStmtNode: public Node {
    public:
        Token path;
        IncludeStmtNode(const Token& path);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class StmtListNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts);
        NodeType get_type() const;
        std::string to_string() const;
    };


}

#endif