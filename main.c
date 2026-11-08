#include "split.h"
#include "value.h"
#include "schema.h"
#include "stat.h"
#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int cmd_schema(const char *filename) {
    Schema *s = schema_build(filename);
    if (s == NULL) {
        fprintf(stderr, "Error: cannot build schema from '%s'\n", filename);
        return 1;
    }
    schema_print(s, stdout);
    schema_free(s);
    return 0;
}


static int cmd_stats(const char *filename) {
    Schema *s = schema_build(filename);
    if (s == NULL) {
        fprintf(stderr, "Error: cannot build schema from '%s'\n", filename);
        return 1;
    }
    ColumnStats *st = stats_compute(s, filename);
    if (st == NULL) {
        fprintf(stderr, "Error: cannot compute stats for '%s'\n", filename);
        schema_free(s);
        return 1;
    }
    stats_print(s, st, stdout);
    stats_free(st);
    schema_free(s);
    return 0;
}

int main(int argc, char **argv) {
    CliOptions opts;
    int rc = cli_parse(argc, argv, &opts);
    if(rc != 0){
        return rc;
    }

    if(opts.command == CMD_HELP){
        cli_print_help();
        return 0;
    }
    else if(opts.command == CMD_SCHEMA){
        return cmd_schema(opts.filename);
    }
    else if(opts.command == CMD_STATS){
        return cmd_stats(opts.filename);
    }
}