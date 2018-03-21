#ifndef __PROTOTYPE_AST_H__
#define __PROTOTYPE_AST_H__

#include <utility>

#include "ast/ExprAST.h"
#include "llvm/IR/IRBuilder.h"
#include "kaleidoscope/kaleidoscope.h"

// Represents the "prototype" for a function,
// which captures its name, and its argument names
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(std::string name, std::vector<std::string> Args) : Name(std::move(name)), Args(std::move(Args)) {}

    llvm::Function *codegen(std::unique_ptr<llvm::Module> &TheModule);

    const std::string &getName() const { return Name; }
};

#endif
