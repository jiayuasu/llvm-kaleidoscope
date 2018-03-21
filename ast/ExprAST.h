#ifndef __EXPR_AST_H__
#define __EXPR_AST_H__

#include "llvm/IR/BasicBlock.h"

class ExprAST {
public:
    virtual ~ExprAST() = default;

    virtual llvm::Value *codegen(std::unique_ptr<llvm::Module> &TheModule) = 0;
};

#endif
