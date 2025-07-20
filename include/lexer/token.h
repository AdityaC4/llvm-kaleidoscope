#pragma once

// The lexer returns tokens [0-255] if it is an unknows character, otherwise one
// of the following known tokens:
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5
};
