#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

//! Writes @p text in one piece and flushes it; safe to call from any thread.
void print(const std::string& text);

#endif // CONSOLE_H
