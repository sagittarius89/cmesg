#include "utils.h"

void print_help()
{
    printf("Usage: client [options]\n");
    printf("  -a, --host       Setup server ip address\n");
    printf("  -p, --port       Setup server port (default 8889)\n");
    printf("  -n, --nick       Setup your nick name (max 15 symbols)\n");
    printf("  -h, --help       Print help\n");
}

struct options* parse_args(char argc, char** argv)
{
    struct options* parsed_options = malloc(sizeof(struct options));
    int option_index = 0;
    int opt;
    int show_help = 0;

    memset(parsed_options, 0, sizeof(struct options));

    parsed_options->port = DEFAULT_PORT;
    parsed_options->show_help = 0;

    while ((opt = getopt_long(argc, argv, "ha:p:n:", options_args, &option_index)) != -1) {
        switch (opt) {
        case 'a':
            strcpy(parsed_options->address, optarg);
            break;
        case 'n':
            if(strlen(optarg)>15)
            {
                printf("nick to long\n");
                show_help = 1;
                break;
            }
            strcpy(parsed_options->nick, optarg);
            break;
        case 'p':
            if (valid_number(optarg)) {
                parsed_options->port = (unsigned short)atoi(optarg);
            } else {
                show_help = 1;
                break;
            }
            break;
        case 'h':
            show_help = 1;
            break;
        }
    }

    parsed_options->show_help = show_help;

    return parsed_options;
}

int validate_args(struct options* parsed_options)
{
    if(strlen(parsed_options->nick)==0)
    {
        printf("You have to setup your nick name\n");
        print_help();

        return 0;
    }
    if (!is_valid_ip(parsed_options->address)) {
        printf("invalid ip address\n");
        print_help();

        return 0;
    }

    return 1;
}

/* return 1 if string contain only digits, else return 0 */
int valid_number(char* ip_str)
{
    while (*ip_str) {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return 0;
    }
    return 1;
}

/* return 1 if IP string is valid, else return 0 */
int is_valid_ip(char* ip_str_param)
{
    int i, num, dots = 0;
    char* ptr;
    char ip_str[16];

    strcpy(ip_str, ip_str_param);

    if (ip_str == NULL)
        return 0;

    ptr = strtok(ip_str, ".");

    if (ptr == NULL)
        return 0;

    while (ptr) {

        /* after parsing string, it must contain only digits */
        if (!valid_number(ip_str))
            return 0;

        num = atoi(ip_str);

        /* check for valid IP */
        if (num >= 0 && num <= 255) {
            /* parse remaining string */
            ptr = strtok(NULL, ".");
            if (ptr != NULL)
                ++dots;
        } else
            return 0;
    }

    /* valid IP string must contain 3 dots */
    if (dots != 3)
        return 0;

    return 1;
}