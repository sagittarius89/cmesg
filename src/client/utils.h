#ifndef UTILS_H
#define UTILS_H

#include <getopt.h>

struct options 
{
    char        nick[16];
    char        address[16];
    uint16_t    port;
    int         show_help;
    int         sockfd;
    int         logged_in;
};

static struct option options_args[] =
{
    {"host",    required_argument, 0, 'a'},
    {"port",    required_argument, 0, 'p'},
    {"nick",    required_argument, 0, 'n'},
    {"help",    required_argument, 0, 'h'},
    {0, 0, 0, 0}
};

void print_help();
int validate_args(struct options* parsed_options);
struct options* parse_args(char argc, char** argv);
int is_valid_ip(char *ip_str_param);
int valid_number(char *ip_str) ;
int str_to_uint16(const char *str, uint16_t *res);

#endif /* UTILS_H */