// lexer headers
#include <fstream>
#include "lexer/lexer.h"
#include "lexer/token.h"

// AST headers
#include "ast/BinaryExprAST.h"
#include "ast/FunctionAST.h"

#include "parser/parser.h"
#include "optimization/optimization.h"

using namespace llvm;



static void HandleDefinition(std::unique_ptr<llvm::Module> &TheModule,
                             std::unique_ptr<llvm::legacy::FunctionPassManager> &TheFPM) {
    if (auto FnAST = ParseDefinition()) {
        if (auto *FnIR = FnAST->codegen(TheModule, TheFPM)) {
            fprintf(stderr, "Read function definition:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void HandleExtern(std::unique_ptr<llvm::Module> &TheModule) {
    if (auto ProtoAST = ParseExtern()) {
        if (auto *FnIR = ProtoAST->codegen(TheModule)) {
            fprintf(stderr, "Read extern:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void HandleTopLevelExpression(std::unique_ptr<llvm::Module> &TheModule,
                                     std::unique_ptr<llvm::legacy::FunctionPassManager> &TheFPM) {
    if (auto FnAST = ParseTopLevelExpr()) {
        if (auto *FnIR = FnAST->codegen(TheModule, TheFPM)) {
            fprintf(stderr, "Read top-level expression:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        getNextToken();
    }
}

static void MainLoop(std::unique_ptr<llvm::Module> &TheModule,
                     std::unique_ptr<llvm::legacy::FunctionPassManager> &TheFPM) {
    while (true) {
        fprintf(stderr, "ready> ");

        switch (CurTok) {
            case tok_eof:
                return;
            case ';':
                getNextToken();
                break;
            case tok_def:
                HandleDefinition(TheModule, TheFPM);
                break;
            case tok_extern:
                HandleExtern(TheModule);
                break;
            default:
                HandleTopLevelExpression(TheModule, TheFPM);
                break;
        }
    }
}

int main() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;


    fprintf(stderr, "ready> ");
    getNextToken();

    std::unique_ptr<llvm::Module> TheModule = llvm::make_unique<Module>("", TheContext);

    std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM = GetFunctionPassManager(TheModule);

    MainLoop(TheModule, TheFPM);

    TheModule->print(errs(), nullptr);

    // Open a file to store the generated IR
    std::ofstream IRfile;
    IRfile.open ("output.ir");
    std::string IRstring;
    raw_string_ostream OS(IRstring);
    OS << *TheModule;
    OS.flush();
    IRfile << IRstring << "\n";
    IRfile.close();

    return 0;
}
