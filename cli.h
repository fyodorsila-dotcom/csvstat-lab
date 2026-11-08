#ifndef CLI_H
#define CLI_H

typedef enum {
    CMD_NONE,
    CMD_HELP,
    CMD_SCHEMA,
    CMD_STATS,
} Command;

typedef struct {
    Command command;
    const char *filename;
} CliOptions;

int cli_parse(int argc, char **argv, CliOptions *out);
void cli_print_help(void);

#endif