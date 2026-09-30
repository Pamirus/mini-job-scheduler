#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include <experimental/propagate_const>
#include <memory>

class Scheduler;
class CommandLineInterfacePrivate;
enum class JobPriority : uint8_t;

//! Interactive command loop on standard input and output.
class CommandLineInterface
{
public:
    //! @p scheduler must outlive the interface.
    explicit CommandLineInterface(Scheduler& scheduler);
    ~CommandLineInterface();
    CommandLineInterface(const CommandLineInterface&) = delete;
    CommandLineInterface& operator=(const CommandLineInterface&) = delete;
    CommandLineInterface(CommandLineInterface&&) = delete;
    CommandLineInterface& operator=(CommandLineInterface&&) = delete;

    //! Runs commands until @c quit or end of input, then stops the scheduler.
    void run();

private:
    std::experimental::propagate_const<std::unique_ptr<CommandLineInterfacePrivate>> d;
};

#endif // COMMANDLINEINTERFACE_H
