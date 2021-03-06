#include "ast/FunctionAST.h"

// Generates LLVM code for functions declarations
llvm::Function *FunctionAST::codegen(std::unique_ptr<llvm::Module> &TheModule,
                                     std::unique_ptr<llvm::legacy::FunctionPassManager> &TheFPM) {
    llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction) {
        TheFunction = Proto->codegen(TheModule);
    }

    if (!TheFunction) {
        return nullptr;
    }

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", TheFunction);
    Builder.SetInsertPoint(BB);
    NamedValues.clear();
    for (auto &Arg : TheFunction->args()) {
        NamedValues[Arg.getName()] = &Arg;
    }

    if (llvm::Value *RetVal = Body->codegen(TheModule)) {
        Builder.CreateRet(RetVal);
        verifyFunction(*TheFunction);

        // Optimize the function.
        TheFPM->run(*TheFunction);

        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}
