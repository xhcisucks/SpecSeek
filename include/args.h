#pragma once
#include <specseek.h>

typedef struct {
    int no_ansi;
    int verbose;
    int pause;
    int feature_mode;
    int feature_count;
    const char **features;
} specseek_args;

specseek_args args_parse(int argc, char **argv);
void args_run_feature_query(const specseek_args *args);
void args_wait_for_exit(void);