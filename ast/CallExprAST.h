#ifndef __CALL_EXPR_AST_H__
#define __CALL_EXPR_AST_H__

#include <utility>

#include "ast/ExprAST.h"
#include "llvm/IR/IRBuilder.h"
#include "logger/logger.h"
#include "kaleidoscope/kaleidoscope.h"

// Expression class for function calls
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(std::string Callee, std::vector<std::unique_ptr<ExprAST>> Args) : Callee(std::move(Callee)),
                                                                                  Args(std::move(Args)) {}

    llvm::Value *codegen(std::unique_ptr<llvm::Module> &TheModule) override;
};

#endif
