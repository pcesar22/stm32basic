#include "main.h"
#include "shell.h"

#define DESC_HELLO_WORLD "A friendly hello world description"

int hello_world(shell_cmd_args *args)
{
    if (args->count == 0)
    {
        log_info("Hello!!!!");
    }
    else
    {
        for (uint32_t i =0; i < args->count; i++)
        {
            log_info("args[%d]: %s", i, args->args[i].val);
        }
    }
    return 1;
}

int cmd_help(shell_cmd_args *args);

/* const shell_cmd dummyCommand = */
/* {.cmd = "name", .desc = DESC_HELLO_WORLD, .func = hello_world}; */

/* const shell_cmd helpCommand = */
/* {.cmd = "help", .desc = "Print this message and exit", .func = cmd_help}; */

// WHY DO I NEED THIS HACK
#define COMMANDS_AVAILABLE {                                            \
        {.cmd = "name", .desc = DESC_HELLO_WORLD, .func = hello_world}, \
        {.cmd = "help", .desc = "Print this message and exit", .func = cmd_help} \
    }
const shell_cmd commands_Available [] = COMMANDS_AVAILABLE;

const shell_cmds console_CommandRegistry = {
    .count = sizeof(commands_Available)/sizeof(shell_cmd),
    .cmds = COMMANDS_AVAILABLE
};

int cmd_help(shell_cmd_args *args)
{
    log_raw("\nAvailable commands\r\n");
    log_raw("~~~~~~~~~~~~~~~~~~~~~~\r\n");
    for (uint32_t i = 0; i < console_CommandRegistry.count; i++)
    {
        log_raw("%s\n\t%s",
                console_CommandRegistry.cmds[i].cmd,
                console_CommandRegistry.cmds[i].desc);
        log_raw("\r\n");
    }
    return 1;
}
