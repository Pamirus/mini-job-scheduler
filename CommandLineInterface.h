#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include <experimental/propagate_const>
#include <memory>

class Scheduler;
class CommandLineInterfacePrivate;
enum class JobPriority : uint8_t;

class CommandLineInterface
{
public:
    explicit CommandLineInterface(Scheduler& scheduler);
    ~CommandLineInterface();

    void run();

private:
    std::experimental::propagate_const<std::unique_ptr<CommandLineInterfacePrivate>> d;};

#endif // COMMANDLINEINTERFACE_H
