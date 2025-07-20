#include "ast/VariableExprAST.h"
#include "kaleidoscope/kaleidoscope.h"
#include "logger/logger.h"

llvm::Value *VariableExprAST::codegen() {
  llvm::Value *V = NamedValues[Name];
  if (!V)
    LogErrorV("Unknown variable name");
  return V;
}
