#ifndef __NUMBER_EXPR_AST_H__
#define __NUMBER_EXPR_AST_H__

#include "ast/ExprAST.h"
#include "kaleidoscope/kaleidoscope.h"

// Expression class for numeric literals like "1.0"
class NumberExprAST : public ExprAST {
    double Val;

public:
    explicit NumberExprAST(double Val) : Val(Val) {}

    llvm::Value *codegen(std::unique_ptr<llvm::Module> &TheModule) override;
};

#endif
