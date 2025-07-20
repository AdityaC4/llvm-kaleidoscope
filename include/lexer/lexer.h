#pragma once

#include "token.h"
#include <string>

extern std::string IdentifierStr;
extern double NumVal;
extern int CurTok;

/// gettok - Return the next token from standard input.
int gettok();

/// getNextToken - Read another token from the standard input and put in CurTok
int getNextToken();
