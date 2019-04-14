#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NORMAL "\x1B[0m"
#define BLACK "\x1B[30;1m"
#define RED "\x1B[31;1m"
#define GREEN "\x1B[32;1m"
#define YELLOW "\x1B[33;1m"
#define BLUE "\x1B[34;1m"
#define MAGENTA "\x1B[35;1m"
#define CYAN "\x1B[36;1m"
#define WHITE "\x1B[37;1m"

WINDOW *main_win, *input_win, *chat_win, *chat_win_box, *input_win_box, *info_line, *info_line_bottom;

//char * backlog_a = NULL;
//char * backlog_b = NULL;

void initialize_curses();
void resize_handler(int sig);
void w_print_err(const char* buf);
void w_print_info(const char* buf);
void w_print_msg(const char* buf);
void colors();
void draw_chat_win();
void draw_input_win();
void draw_info_lines();
void w_print_system_msg(const char* buf, char* color, char* type);
void show_terminal_to_small();

#endif /* DISPLAY_H */