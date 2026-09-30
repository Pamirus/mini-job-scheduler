//! @file
//! @brief The CommandLineInterface class.

#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include <experimental/propagate_const>
#include <memory>

class Scheduler;
class CommandLineInterfacePrivate;
enum class JobPriority : uint8_t;

//! @brief Interactive command loop on standard input and output.
//!
//! Each input line starts with a command name. The matching Command (see
//! registerCommands()) runs with the rest of the line as its arguments.
class CommandLineInterface
{
public:
    //! Creates the interface and its commands; @p scheduler must outlive it.
    explicit CommandLineInterface(Scheduler& scheduler);
    ~CommandLineInterface();

    //! @brief Reads and runs commands until @c quit or the end of input.
    //!
    //! Stops the scheduler before returning.
    void run();

private:
    std::experimental::propagate_const<std::unique_ptr<CommandLineInterfacePrivate>> d;};

#endif // COMMANDLINEINTERFACE_H
