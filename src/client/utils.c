#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "utils.h"



void print_help()
{
    printf("Usage: client [options]\n");
    printf("  -a, --host       Setup server ip address\n");
    printf("  -p, --port       Setup server port (default 8888)\n");
    printf("  -n, --nick       Setup your nick name (max 15 symbols)\n");
    printf("  -h, --help       Print help\n");
}

struct options* parse_args(char argc, char** argv)
{
    struct options* parsed_options = malloc(sizeof(struct options));   
    int option_index = 0;
    int opt;

    strcpy(parsed_options->port,"8888");
    parsed_options->show_help=0;

    while((opt = getopt_long(argc, argv, "ha:p:n:",option_index,&options_args)) != -1)  
    {  
        switch(opt)  
        {  
            case 'a':
                strcpy(parsed_options->address, optarg);
            break;
            case 'n':
                strncpy(parsed_options->nick, optarg, sizeof(optarg));
                parsed_options->nick[sizeof(optarg)+1]='\0';
            break;
            case 'p':
                strcpy(parsed_options->port, optarg);
            break;
            case 'h':
                parsed_options->show_help=1;
            break;
        }  
    }

    return parsed_options; //0x555555758260
}

int validate_args(struct options* parsed_options)
{
    if(!is_valid_ip(parsed_options->address))
    {
        printf("invalid ip address\n");
        print_help();

        return 0;
    }

    if(!valid_number(parsed_options->port))
    {
        printf("invalid port\n");
        print_help();

        return 0;
    }

    return 1;
}

/* return 1 if string contain only digits, else return 0 */
int valid_number(char *ip_str) 
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
int is_valid_ip(char *ip_str_param) 
{ 
    int i, num, dots = 0; 
    char *ptr; 
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