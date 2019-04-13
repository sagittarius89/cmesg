#include "input.h"

int user_input(char* buffer) {
   int i = 0;
   int ch;

   wmove(input_win, 0, 0);
   wrefresh(input_win);
   // Read 1 char at a time
   while ((ch = getch()) != '\n') {
      // Backspace
      if (ch == 263) 
      {
         if (i > 0) 
         {
            buffer[--i] = '\0';

            wclear(input_win);
            wprintw(input_win,buffer);
            wrefresh(input_win);
         }
         else 
         {
            wclear(input_win);
         }
      }
      else if (ch == KEY_RESIZE) 
      {
        continue;
      }
      else if (ch == KEY_RESIZE || ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT)
      {
        continue;
      }
      // Otherwise put in buffer
      else if (ch != ERR) 
      {
         if (i < BUFFERSIZE - 1) 
         {
            strcat(buffer, (char *)&ch);
            i++;
            wprintw(input_win, (char *)&ch);
            wrefresh(input_win);
         }
         // Unless buffer is full
         else 
         {
            wprintw(input_win, "\b%s", (char *)&ch);
            buffer[(i - 1)] = '\0';
            strcat(buffer, (char *)&ch);
            wrefresh(input_win);
         }
      }
   }
   // Null terminate, clear input window
   buffer[i] = '\0';
   wclear(input_win);
   wrefresh(input_win);
   return i;
}
