#pragma once

#include "token.h"
#include <string>

static std::string IdentifierStr;
static double NumVal;
static int CurTok;

/// gettok - Return the next token from standard input.
static int gettok();

/// getNextToken - Read another token from the standard input and put in CurTok
static int getNextToken();
