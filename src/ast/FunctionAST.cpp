#include "ast/FunctionAST.h"
#include "kaleidoscope/kaleidoscope.h"
#include "logger/logger.h"
#include "llvm/IR/Verifier.h"
#include <string>

llvm::Function *FunctionAST::codegen() {
  // // Check if previously declared with extern
  // llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());
  //
  // if (!TheFunction)
  //   TheFunction = Proto->codegen();
  //
  // if (!TheFunction)
  //   return nullptr;
  //
  // if (!TheFunction->empty())
  //   return (llvm::Function *)LogErrorV("Function cannot be redefined");

  // Transfer ownership of the prototype to the FunctionProtos map
  // keep a reference to it for use
  auto &P = *Proto;
  FunctionProtos[Proto->getName()] = std::move(Proto);
  llvm::Function *TheFunction = getFunction(P.getName());
  if (!TheFunction)
    return nullptr;

  // new basic block to start insertion
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
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

    // Optimize the function
    TheFPM->run(*TheFunction, *TheFAM);

    return TheFunction;
  }

  // Error reading body, delete the function
  TheFunction->eraseFromParent();
  return nullptr;
}
