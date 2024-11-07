#ifndef TRANSPILER_H
#define TRANSPILER_H


#include <string>
#include <memory>
#include <sstream>
#include "node.h"

namespace tachyon {
    class Transpiler {
    public:
        std::ostringstream code;
        Transpiler();
        void visit(const std::shared_ptr<Node>& node);
        void visit_null_node(const std::shared_ptr<NullNode>& node);
        void visit_string_node(const std::shared_ptr<StringNode>& node);
        void visit_vector_node(const std::shared_ptr<VectorNode>& node);
        void visit_object_node(const std::shared_ptr<ObjectNode>& node);
        void visit_anon_func_expr_node(const std::shared_ptr<AnonFuncExprNode>& node);
        void visit_identifier_node(const std::shared_ptr<IdentifierNode>& node);
        void visit_call_expr_node(const std::shared_ptr<CallExprNode>& node);
        void visit_object_prop_node(const std::shared_ptr<ObjectPropNode>& node);
        void visit_subscript_node(const std::shared_ptr<SubscriptNode>& node);
        void visit_unary_op_node(const std::shared_ptr<UnaryOpNode>& node);
        void visit_bin_op_node(const std::shared_ptr<BinOpNode>& node);
        void visit_expr_stmt_node(const std::shared_ptr<ExprStmtNode>& node);
        void visit_var_def_stmt_node(const std::shared_ptr<VarDefStmtNode>& node);
        void visit_const_def_stmt_node(const std::shared_ptr<ConstDefStmtNode>& node);
        void visit_block_stmt_node(const std::shared_ptr<BlockStmtNode>& node);
        void visit_if_stmt_node(const std::shared_ptr<IfStmtNode>& node);
        void visit_while_stmt_node(const std::shared_ptr<WhileStmtNode>& node);
        void visit_for_stmt_node(const std::shared_ptr<ForStmtNode>& node);
        void visit_continue_stmt_node(const std::shared_ptr<ContinueStmtNode>& node);
        void visit_break_stmt_node(const std::shared_ptr<BreakStmtNode>& node);
        void visit_return_stmt_node(const std::shared_ptr<ReturnStmtNode>& node);
        void visit_func_def_stmt_node(const std::shared_ptr<FuncDefStmtNode>& node);
        void visit_throw_stmt_node(const std::shared_ptr<ThrowStmtNode>& node);
        void visit_try_catch_stmt_node(const std::shared_ptr<TryCatchStmtNode>& node);
        void visit_include_stmt_node(const std::shared_ptr<IncludeStmtNode>& node);
        // void visit_define_stmt_node(const std::shared_ptr<DefineStmtNode>& node);
        void visit_stmt_list_node(const std::shared_ptr<StmtListNode>& node);
    };
}

#endif