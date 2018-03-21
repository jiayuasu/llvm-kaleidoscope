#ifndef __VARIABLE_EXPR_AST_H__
#define __VARIABLE_EXPR_AST_H__

#include <utility>

#include "ast/ExprAST.h"
#include "logger/logger.h"

// Expression class for referencing a variable, like "a"
class VariableExprAST : public ExprAST {
    std::string Name;

public:
    explicit VariableExprAST(std::string Name) : Name(std::move(Name)) {}

    llvm::Value *codegen(std::unique_ptr<llvm::Module> &TheModule) override;
};

#endif
