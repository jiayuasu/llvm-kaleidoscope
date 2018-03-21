#include "ast/CallExprAST.h"

// Generate LLVM code for function calls
llvm::Value *CallExprAST::codegen(std::unique_ptr<llvm::Module> &TheModule) {
    llvm::Function *CalleeF = TheModule->getFunction(Callee);

    if (!CalleeF) {
        return LogErrorV("Unknown function referenced");
    }

    if (CalleeF->arg_size() != Args.size()) {
        return LogErrorV("Incorrect # arguments passed");
    }

    std::vector<llvm::Value *> ArgsV;
    auto size = Args.size();
    for (unsigned i = 0; i != size; i++) {
        ArgsV.push_back(Args[i]->codegen(TheModule));

        if (!ArgsV.back()) {
            return nullptr;
        }
    }

    return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}
