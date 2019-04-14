#include "display.h"

void initialize_curses()
{
    // Initialize the terminal window, read terminfo
    if ((main_win = initscr()) == NULL) {
        exit(1);
    }
    // Do not echo key presses
    noecho();
    // Read input one char at a time
    cbreak();
    // Capture special keys
    keypad(main_win, 1);
    // Start those colors
    colors();

    if (LINES < 24 || COLS < 76) {
        show_terminal_to_small();
    } else {
        // Draw everything
        draw_chat_win();
        draw_input_win();
        draw_info_lines();
    }
}

/* Draw chat box and window */
void draw_chat_win()
{
    // Create window for chat box, draw said box
    chat_win_box = subwin(main_win, (LINES * 0.8), COLS, 0, 0);
    box(chat_win_box, 0, 0);
    // Draw a slick title on it
    mvwaddch(chat_win_box, 0, (COLS * 0.5) - 6, ACS_RTEE);
    wattron(chat_win_box, COLOR_PAIR(3));
    mvwaddstr(chat_win_box, 0, (COLS * 0.5) - 5, "  cmsg   ");
    wattroff(chat_win_box, COLOR_PAIR(3));
    mvwaddch(chat_win_box, 0, (COLS * 0.5) + 4, ACS_LTEE);
    wrefresh(chat_win_box);
    // Create sub window in box to hold text
    chat_win = subwin(chat_win_box, (LINES * 0.8 - 2), COLS - 2, 1, 1);
    // Enable text scrolling
    scrollok(chat_win, TRUE);

    //if(backlog_a == NULL)
    //     backlog_a = malloc(COLS*LINES*2);
}

/* Print formatted error */
void w_print_err(const char* buf)
{
    w_print_system_msg(buf, (char*)COLOR_PAIR(8), "Error");
}

/* Print formatted info */
void w_print_info(const char* buf)
{
    w_print_system_msg(buf, (char*)COLOR_PAIR(100), "Info");
}

/* Print formatted message */
void w_print_msg(const char* buf)
{
    wattron(chat_win, COLOR_PAIR(1));
    wprintw(chat_win, " %s\n", buf);
    wattroff(chat_win, COLOR_PAIR(1));
    wrefresh(chat_win);

    //append_backlog(buf);
}

/*void append_backlog(const char * buf)
{
    if(strlen(buf) + strlen(backlog_a) > sizeof(backlog_a))
    {
        backlog_b = malloc(sizeof(backlog_a) + sizeof(buf) + 1);
        memcpy(backlog_b,backlog_a, sizeof(backlog_a));
        free(backlog_a);

        strcat(backlog_b, buf);
        backlog_a = malloc(sizeof(backlog_b) * 2);
        memcpy(backlog_a,backlog_b, sizeof(backlog_b));
        free(backlog_b);
    }
    else
    {
        strcat(backlog_a, buf);
    }
    
}*/

void w_print_system_msg(const char* buf, char* color, char* type)
{
    wattron(chat_win, A_BOLD);
    wprintw(chat_win, " ");
    waddch(chat_win, ACS_HLINE);
    waddch(chat_win, ACS_HLINE);
    waddch(chat_win, ACS_HLINE);
    wprintw(chat_win, " ");
    wattron(chat_win, color);
    wprintw(chat_win, type);
    wattroff(chat_win, color);
    wattroff(chat_win, A_BOLD);
    wattron(chat_win, COLOR_PAIR(1));
    wprintw(chat_win, " %s\n", buf);
    wattroff(chat_win, COLOR_PAIR(1));
    wrefresh(chat_win);

    //append_backlog(buf);
}

/* Draw input box and window */
void draw_input_win()
{
    input_win_box = subwin(main_win, (LINES * 0.2) - 1, COLS, (LINES * 0.8) + 1, 0);
    box(input_win_box, 0, 0);
    input_win = subwin(input_win_box, (LINES * 0.2) - 3, COLS - 2, (LINES * 0.8) + 2, 1);
}

/* Draw info lines */
void draw_info_lines()
{
    info_line = subwin(main_win, 1, COLS, (LINES * 0.8), 0);
    wbkgd(info_line, COLOR_PAIR(6));
    wprintw(info_line, " Type /help to print a list of available commands");
    wrefresh(info_line);
    info_line_bottom = subwin(main_win, 1, COLS, LINES - 1, 0);
}

/* A place for all the bad terminals to go */
void show_terminal_to_small()
{
    wbkgd(main_win, COLOR_PAIR(8));
    wattron(main_win, A_BOLD);
    mvwaddstr(main_win, (LINES * 0.5) - 1, (COLS * 0.5) - 3, "TERMINAL");
    mvwaddstr(main_win, (LINES * 0.5), (COLS * 0.5) - 4, "IS TOO SMALL");
    wattroff(main_win, A_BOLD);
    wrefresh(main_win);
    wbkgd(main_win, COLOR_PAIR(1));
}

void colors()
{
    start_color();
    use_default_colors();

    // Initialize color pairs
    init_pair(1, -1, -1);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_RED, -1);
    init_pair(5, COLOR_BLUE, -1);
    init_pair(6, COLOR_MAGENTA, -1);
    init_pair(7, COLOR_GREEN, -1);
    init_pair(8, COLOR_WHITE, COLOR_RED);
}

/* Handle window resizing */
void resize_handler(int sig)
{
    // End current windows
    endwin();
    refresh();
    clear();

    if (LINES < 24 || COLS < 76) {
        show_terminal_to_small();
    } else {
        //Redraw windows
        draw_chat_win();
        draw_input_win();
        draw_info_lines();

        //wprintw(chat_win, backlog_a);

        //Refresh and move cursor to input window
        wrefresh(chat_win);
        wcursyncup(input_win);
        wrefresh(input_win);
    }
}