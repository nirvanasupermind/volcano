#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <vector>
#include "token.h"

namespace tachyon {
    enum class NodeType {
        NUMBER,
        STRING,
        VECTOR,
        OBJECT,
        IDENTIFIER,
        ANON_FUNC_EXPR,
        CALL_EXPR,
        OBJECT_PROP,
        SUBSCRIPT,
        UNARY_OP,
        BIN_OP,
        VAR_DEF_STMT,
        CONST_DEF_STMT,
        EXPR_STMT,
        BLOCK_STMT,
        IF_STMT,
        WHILE_STMT,
        FOR_STMT,
        RETURN_STMT,
        FUNC_DEF_STMT,
        THROW_STMT,
        TRY_CATCH_STMT,
        INCLUDE_STMT,
        // DEFINE_STMT,
        STMT_LIST
    };

    class Node {
    public:
        virtual NodeType get_type() const = 0;
        virtual std::string to_string() const = 0;
        virtual double get_double() const;
    };

    class NumberNode: public Node {
    public:
        Token tok;
        NumberNode(const Token& tok);
        NodeType get_type() const;
        std::string to_string() const;
        double get_double() const;
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

    class ObjectNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > keys;
        std::vector<std::shared_ptr<Node> > vals;
        ObjectNode(const std::vector<std::shared_ptr<Node> >& keys, const std::vector<std::shared_ptr<Node> >& vals);
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

    class AnonFuncExprNode: public Node {
    public:
        std::vector<Token> arg_names;
        std::shared_ptr<Node> body;
        AnonFuncExprNode(const std::vector<Token>& arg_names, const std::shared_ptr<Node>& body);
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

    class ObjectPropNode: public Node {
    public:
        std::shared_ptr<Node> obj;
        Token prop;
        ObjectPropNode(const std::shared_ptr<Node>& obj, const Token& prop);
        NodeType get_type() const;
        std::string to_string() const;
    };

    class SubscriptNode: public Node {
    public:
        std::shared_ptr<Node> base;
        std::shared_ptr<Node> idx;        
        SubscriptNode(const std::shared_ptr<Node>& obj, const std::shared_ptr<Node>& idx);
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
        double get_double() const;
    };

    class BinOpNode: public Node {
    public:
        std::shared_ptr<Node> left_node;
        Token op_tok;
        std::shared_ptr<Node> right_node;
        BinOpNode(const std::shared_ptr<Node>& left_node, const Token& op_tok, const std::shared_ptr<Node>& right_node);
        NodeType get_type() const;
        std::string to_string() const;
        double get_double() const;
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
        std::vector<std::shared_ptr<Node> > conds;
        std::vector<std::shared_ptr<Node> > bodies;
        std::shared_ptr<Node> else_body;
        IfStmtNode(const std::vector<std::shared_ptr<Node> >& conds, const std::vector<std::shared_ptr<Node> >& bodies,  const std::shared_ptr<Node>& else_body);
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

    class ThrowStmtNode: public Node {
    public:
        std::shared_ptr<Node> expr_node;
        ThrowStmtNode(const std::shared_ptr<Node>& expr_node);
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

    // class DefineStmtNode: public Node {
    // public:
    //     Token name;
    //     std::shared_ptr<Node> expr_node;
    //     DefineStmtNode(const Token& name, const std::shared_ptr<Node>& expr_node);
    //     NodeType get_type() const;
    //     std::string to_string() const;
    // };

    class StmtListNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        StmtListNode(const std::vector<std::shared_ptr<Node> >& stmts);
        NodeType get_type() const;
        std::string to_string() const;
    };


}

#endif