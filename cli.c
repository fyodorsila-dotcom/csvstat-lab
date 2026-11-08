#include "cli.h"
#include <stdio.h>
#include <string.h>

void cli_print_help(void) {
    printf("Usage:\n");
    printf("  csvstat schema <file.csv>\n");
    printf("  csvstat stats <file.csv>\n");
    printf("  csvstat --help\n");
}

int cli_parse(int argc, char **argv, CliOptions *out) {
    out->command = CMD_NONE;
    out->filename = NULL;

    if (argc < 2) {
        fprintf(stderr, "Error: no command provided\n");
        return 2;
    }

    if (strcmp(argv[1], "--help") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Error: extra arguments for --help\n");
            return 2;
        }
        out->command = CMD_HELP;
        return 0;
    }

    if (strcmp(argv[1], "schema") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: 'schema' requires exactly one file\n");
            return 2;
        }
        out->command = CMD_SCHEMA;
        out->filename = argv[2];
        return 0;
    }

    if (strcmp(argv[1], "stats") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: 'stats' requires exactly one file\n");
            return 2;
        }
        out->command = CMD_STATS;
        out->filename = argv[2];
        return 0;
    }

    fprintf(stderr, "Error: unknown command '%s'\n", argv[1]);
    return 2;
}

