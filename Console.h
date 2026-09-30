//! @file
//! @brief Thread-safe writing to standard output.

#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

//! @brief Writes @p text to standard output in one piece and flushes it.
//!
//! Safe to call from any thread. The CLI and the workers print at the same
//! time, and this keeps their messages from mixing.
void print(const std::string& text);

#endif // CONSOLE_H
