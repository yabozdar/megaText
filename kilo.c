// We begin with unistd header file.
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// This restores the original attributes of user terminal.
struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
// To avoid cooked mode, we need raw mode in terminal.
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  // We use atexit() to register disable RawMode function.
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  // ICANON turns off canonical mode.
  raw.c_lflag &= ~(ECHO | ICANON | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

// This is the main function of the program.
int main() {
  enableRawMode();

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
    
  }
  return 0;
}
