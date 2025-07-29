#include "kaleidoscope/kaleidoscope.h"
#include "ast/PrototypeAST.h"
#include <memory>

// Object that owns the LLVM core data structures such as the type and constant
// value tables.
std::unique_ptr<llvm::LLVMContext> TheContext;

// Helper to make easy instruction generation
// IRBuilder class template keeps track of current place to insert instructions
// and has methods to create new instructions
std::unique_ptr<llvm::IRBuilder<>> Builder;

/// Top-level setructure LLVM IR uses to contain code.
/// contains functions and gloval variables.
/// it will own the memory for all the IR generated, hence codegen() returns a
/// raw Value* rather than unique_ptr
std::unique_ptr<llvm::Module> TheModule;

/// Basically a symbol table with scope
/// keeps track of which values are defined in the current scope and what their
/// LLVM representation is.
std::map<std::string, llvm::Value *> NamedValues;

std::unique_ptr<llvm::FunctionPassManager> TheFPM;
std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
std::unique_ptr<llvm::StandardInstrumentations> TheSI;
std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

llvm::Function *getFunction(std::string Name) {
  // check if the function has already been added to current module
  if (auto *F = TheModule->getFunction(Name))
    return F;

  // if not, check if we can codegen the declaration from existing prototype
  auto FI = FunctionProtos.find(Name);
  if (FI != FunctionProtos.end())
    return FI->second->codegen();

  // if no existing prototype
  return nullptr;
}
