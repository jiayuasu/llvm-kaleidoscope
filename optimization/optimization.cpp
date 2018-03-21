#include "optimization.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

std::unique_ptr<llvm::legacy::FunctionPassManager> GetFunctionPassManager(std::unique_ptr<llvm::Module> &TheModule) {
    // Create a new pass manager attached to Module
    std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM =
            llvm::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    TheFPM->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add(llvm::createCFGSimplificationPass());

    TheFPM->doInitialization();

    return TheFPM;
}
