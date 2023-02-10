#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>
#include "token.h"

namespace eris {

    enum class NodeKind {
        NIL,
        NUMBER,
        TRUE,
        FALSE,
        CHAR,
        PAREN_EXPR,
        UNARY_EXPR,
        BINARY_EXPR,
        EXPR_STMT,
        STMT_LIST
    };

    class Node {
    public:
        int line;
        virtual NodeKind kind() const = 0;
        virtual ~Node() = default;
    };

    class NilNode: public Node {
    public:
        explicit NilNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class NumberNode: public Node {
    public:
        double val;
        explicit NumberNode(double val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class TrueNode: public Node {
    public:
        explicit TrueNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };
    
    class FalseNode: public Node {
    public:
        explicit FalseNode(int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class CharNode: public Node {
    public:
        char val;
        explicit CharNode(char val, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ParenExprNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ParenExprNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class UnaryExprNode: public Node {
    public:
        Token op;
        std::shared_ptr<Node> node;
        explicit UnaryExprNode(Token op, std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class BinaryExprNode: public Node {
    public:
        Token op;
        std::shared_ptr<Node> node_a;
        std::shared_ptr<Node> node_b;
        explicit BinaryExprNode(Token op, std::shared_ptr<Node> node_a, std::shared_ptr<Node> node_b, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class ExprStmtNode: public Node {
    public:
        std::shared_ptr<Node> node;
        explicit ExprStmtNode(std::shared_ptr<Node> node, int line);
        NodeKind kind() const;
        std::string str() const;
    };

    class StmtListNode: public Node {
    public:
        std::vector<std::shared_ptr<Node> > stmts;
        explicit StmtListNode(std::vector<std::shared_ptr<Node> > stmts, int line);
        NodeKind kind() const;
        std::string str() const;
    };
} // namespace eris

#endif // NODE_H