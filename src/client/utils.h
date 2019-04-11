#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>

struct options 
{
    char nick[16];
    char address[16];
    char port[8];
    char show_help;
};

static struct option options_args[] =
{
    {"host",  required_argument, 0, 'a'},
    {"port",  required_argument, 0, 'p'},
    {"nick",    required_argument, 0, 'n'},
    {"help",    required_argument, 0, 'h'},
    {0, 0, 0, 0}
};

void print_help();
int validate_args(struct options* parsed_options);
struct options* parse_args(char argc, char** argv);

#endif /* UTILS_H */