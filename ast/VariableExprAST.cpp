#include "ast/VariableExprAST.h"

// We assume that the variable has already been emitted somewhere
llvm::Value *VariableExprAST::codegen(std::unique_ptr<llvm::Module> &TheModule) {
    llvm::Value *V = NamedValues[Name];

    if (!V) {
        LogErrorV("Unknown variable name");
    }

    return V;
}
