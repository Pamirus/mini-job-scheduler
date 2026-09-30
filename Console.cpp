#include "Console.h"

#include <iostream>
#include <mutex>

void print(const std::string& text)
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << text << std::flush;
}
