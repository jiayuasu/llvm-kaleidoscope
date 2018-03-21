#include "ast/NumberExprAST.h"

// Generate LLVM code for numeric literals
llvm::Value *NumberExprAST::codegen(std::unique_ptr<llvm::Module> &TheModule) {
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
}
