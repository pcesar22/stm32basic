#include "main.h"
#include "shell.h"
#include "pwm.h"

#define DESC_HELLO_WORLD "A friendly hello world description"

int cmd_pwm_start(shell_cmd_args *args)
{
    if (args->count == 1)
    {
        uint32_t duty = atoi(args->args[0].val);
        log_info("Started PWM output! DC: %d", duty);
        PWM_Start(duty);
    }
    else
    {
        log_info("Incorrect usage");
    }
    return 1;
}

int cmd_pwm_stop(shell_cmd_args *args)
{
    log_info("Stopped PWM output!");
    PWM_Stop();
    return 1;
}

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
        {.cmd = "help", .desc = "Print this message and exit", .func = cmd_help}, \
        {.cmd = "pwm-start", .desc = "Start PWM output on EXT9", .func = cmd_pwm_start}, \
        {.cmd = "pwm-stop", .desc = "Stop PWM output on EXT9", .func = cmd_pwm_stop}, \
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
