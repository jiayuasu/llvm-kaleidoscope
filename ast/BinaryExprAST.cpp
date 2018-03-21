#include "ast/BinaryExprAST.h"

// Generate LLVM code for binary expressions
llvm::Value *BinaryExprAST::codegen(std::unique_ptr<llvm::Module> &TheModule) {
    llvm::Value *L = LHS->codegen(TheModule);
    llvm::Value *R = RHS->codegen(TheModule);

    if (!L || !R) {
        return nullptr;
    }

    switch (Op) {
        case '+':
            return Builder.CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder.CreateFSub(L, R, "subtmp");
        case '*':
            return Builder.CreateFMul(L, R, "multmp");
        case '<':
            L = Builder.CreateFCmpULT(L, R, "cmptmp");
            return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
        default:
            return LogErrorV("Invalid binary operator");
    }
}
