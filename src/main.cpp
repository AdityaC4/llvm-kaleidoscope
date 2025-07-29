// lexer
#include "kaleidoscope/kaleidoscopeJIT.h"
#include "lexer/lexer.h"
#include "lexer/token.h"

#include "kaleidoscope/kaleidoscope.h"

#include "parser/parser.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include <memory>

static llvm::ExitOnError ExitOnErr;

void InitializeModuleAndManagers() {
  // open a new context and module
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("mah jit", *TheContext);
  TheModule->setDataLayout(TheJIT->getDataLayout());

  // new builder
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

  // new pass and analysis managers
  TheFPM = std::make_unique<llvm::FunctionPassManager>();
  TheLAM = std::make_unique<llvm::LoopAnalysisManager>();
  TheFAM = std::make_unique<llvm::FunctionAnalysisManager>();
  TheCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
  TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();
  ThePIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
  TheSI = std::make_unique<llvm::StandardInstrumentations>(
      *TheContext, /*debug logging*/ true);
  TheSI->registerCallbacks(*ThePIC, TheMAM.get());

  // add transform passes
  // "peephole" optimizations and bit-twiddling optimizations
  TheFPM->addPass(llvm::InstCombinePass());
  // Reassociate expressions.
  TheFPM->addPass(llvm::ReassociatePass());
}

void HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read function definition: ");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

void HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      fprintf(stderr, "Read extern: ");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

void HandleTopLevelExpression() {
  if (auto FnAST = ParseTopLevelExpr()) {
    if (auto *FnIR = FnAST->codegen()) {
      fprintf(stderr, "Read top-level expression: ");
      FnIR->print(llvm::errs());
      fprintf(stderr, "\n");

      FnIR->eraseFromParent();
    }
  } else {
    getNextToken();
  }
}

void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof:
      return;
    case ';':
      getNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

int main() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  // declare precedence
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;

  fprintf(stderr, "ready> ");
  getNextToken();

  TheJIT = ExitOnErr(llvm::orc::KaleidoscopeJIT::Create());

  InitializeModuleAndManagers();

  MainLoop();

  // TheModule->print(llvm::errs(), nullptr);

  return 0;
}
