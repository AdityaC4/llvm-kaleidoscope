#include "ast/FunctionAST.h"
#include "kaleidoscope/kaleidoscope.h"
#include "logger/logger.h"
#include <llvm-14/llvm/IR/Verifier.h>
#include <string>

llvm::Function *FunctionAST::codegen() {
  // Check if previously declared with extern
  llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;

  if (!TheFunction->empty())
    return (llvm::Function *)LogErrorV("Function cannot be redefined");

  // new basic block to start insertion
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  // record function arguments in the symbol table
  NamedValues.clear();
  for (auto &Arg : TheFunction->args())
    NamedValues[std::string(Arg.getName())] = &Arg;

  if (llvm::Value *RetVal = Body->codegen()) {
    // finish the function
    Builder->CreateRet(RetVal);

    // validate the code
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, delete the function
  TheFunction->eraseFromParent();
  return nullptr;
}
