#include "kaleidoscope/kaleidoscope.h"

// Object that owns the LLVM core data structures such as the type and constant
// value tables.
llvm::LLVMContext TheContext;

// Helper to make easy instruction generation
// IRBuilder class template keeps track of current place to insert instructions
// and has methods to create new instructions
llvm::IRBuilder<> Builder(TheContext);

/// Top-level setructure LLVM IR uses to contain code.
/// contains functions and gloval variables.
/// it will own the memory for all the IR generated, hence codegen() returns a
/// raw Value* rather than unique_ptr
std::unique_ptr<llvm::Module> TheModule;

/// Basically a symbol table with scope
/// keeps track of which values are defined in the current scope and what their
/// LLVM representation is.
std::map<std::string, llvm::Value *> NamedValues;
