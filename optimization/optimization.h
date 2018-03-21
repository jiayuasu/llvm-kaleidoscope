#include "llvm/IR/LegacyPassManager.h"

#ifndef LLVM_KALEIDOSCOPE_OPTIMIZATION_H
#define LLVM_KALEIDOSCOPE_OPTIMIZATION_H

std::unique_ptr<llvm::legacy::FunctionPassManager> GetFunctionPassManager(std::unique_ptr<llvm::Module> &TheModule);

#endif //LLVM_KALEIDOSCOPE_OPTIMIZATION_H
