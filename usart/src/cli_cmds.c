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

const shell_cmd dummyCommand =
{.cmd = "name", .desc = DESC_HELLO_WORLD, .func = hello_world};

const shell_cmd helpCommand =
{.cmd = "help", .desc = "Print this message and exit", .func = cmd_help};

const shell_cmds console_CommandRegistry = {
    .count = (char)2,
    .cmds = {dummyCommand, helpCommand}
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
