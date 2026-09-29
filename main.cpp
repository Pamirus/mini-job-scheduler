#include <iostream>

#include "Scheduler.h"
#include "CommandLineInterface.h"

int main()
{
    Scheduler scheduler(2);
    CommandLineInterface cli(scheduler);
    cli.run();

    return 0;
}
