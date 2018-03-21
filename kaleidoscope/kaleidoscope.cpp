#include "kaleidoscope.h"

// This is an object that owns LLVM core data structures
llvm::LLVMContext TheContext;

// This is a helper object that makes easy to generate LLVM instructions
llvm::IRBuilder<> Builder(TheContext);

// This map keeps track of which values are defined in the current scope
std::map<std::string, llvm::Value *> NamedValues;