#include "ast/NumberExprAST.h"
#include "kaleidoscope/kaleidoscope.h"

llvm::Value *NumberExprAST::codegen() {
  return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}
