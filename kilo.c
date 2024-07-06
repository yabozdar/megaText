// We begin with unistd header file.
#include <unistd.h>

// This is the main function of the program.
// To avoid cooked mode, we need raw mode in terminal.
int main() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}
