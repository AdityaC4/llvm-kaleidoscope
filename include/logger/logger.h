#pragma once

#include "ast/ExprAST.h"
#include "ast/PrototypeAST.h"
#include <memory>

/// Logerror* - These are little helpers for error handling
std::unique_ptr<ExprAST> LogError(const char *Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);
