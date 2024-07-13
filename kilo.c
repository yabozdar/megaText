/*** INCLUDES ***/

// We begin with unistd header file.
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** DEFINES ***/

#define CTRL_KEY(k) ((k) & 0x1f)

/*** DATA ***/
// This restores the original attributes of user terminal.
struct termios orig_termios;

/*** TERMINAL ***/

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  // tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
  
}
// To avoid cooked mode, we need raw mode in terminal.
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  // We use atexit() to register disable RawMode function.
  atexit(disableRawMode);
  
  struct termios raw = orig_termios;
  // IXON turns off ctrl-s and ctrl-q commands to terminal.
  // ICRNL fixes ctrl-m and carraige return issues
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // OPOST turns off all output processing
  raw.c_iflag &= ~(OPOST);
  raw.c_iflag |= (CS8);
  // ICANON turns off canonical mode.
  // ISIG turns off ctrl-c and ctrl-z commands to terminal.
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  // A timeout for read() function.
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcgetattr(STDIN_FILENO, &raw) == -1) die("tcgetattr");
}

char editorReadkey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

/*** OUTPUT ***/
void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
}

/*** INPUT ***/

void editorProcessKeypress() {
  char c = editorReadkey();

  switch (c) {
  case CTRL_KEY('q'):
    exit(0);
    break;
  }
}

/*** INIT ***/
// This is the main function of the program.
int main() {
  enableRawMode();

  while (1) {
    editorProcessKeypress();
  }
  return 0;
}
