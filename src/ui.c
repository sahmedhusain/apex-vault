#include "ui.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// ANSI Color Codes
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"
#define COLOR_CYAN "\033[36m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_WHITE "\033[37m"
#define BG_BLUE "\033[44m"

// Key Codes for navigation
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_ENTER 10
#define KEY_BACKSPACE 127

static struct termios orig_termios;
static int ui_initialized = 0;

void resetTerminal() {
  if (ui_initialized) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    // Show cursor
    printf("\033[?25h");
    ui_initialized = 0;
  }
}

void initUI() {
  if (!isatty(STDIN_FILENO))
    return;

  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    perror("tcgetattr");
    exit(1);
  }

  atexit(resetTerminal);

  ui_initialized = 1;

  // Hide cursor initially
  printf("\033[?25l");
  clearScreen();
}

void closeUI() { resetTerminal(); }

void clearScreen() { printf("\033[H\033[J"); }

// Helper to print padding for centering
void printPadding(int len) {
  int width = 80;
  int padding = (width - len) / 2;
  if (padding < 0)
    padding = 0;
  for (int i = 0; i < padding; i++)
    putchar(' ');
}

void printCentered(const char *text) {
  printPadding(strlen(text));
  printf("%s\n", text);
}

void showHeader(const char *title) {
  clearScreen();
  printf(COLOR_CYAN);
  // Manual text centering (8 spaces padding) because strlen() counts bytes, not
  // columns, breaking dynamic centering for UTF-8 box chars
  printf("        "
         "╔══════════════════════════════════════════════════════════════╗\n"
         "        ║                    ATM MANAGEMENT SYSTEM                   "
         "  ║\n"
         "        "
         "╚══════════════════════════════════════════════════════════════╝\n");
  printf(COLOR_RESET);
  printf("\n");

  if (title) {
    printf(COLOR_BOLD COLOR_YELLOW);
    char buf[100];
    snprintf(buf, sizeof(buf), "=== %s ===", title);
    printCentered(buf);
    printf(COLOR_RESET);
    printf("\n");
  }
}

void showStatus(const char *message, int isError) {
  // Rough estimate of displayed length (message length + [ERROR] space)
  int len = strlen(message) + 10;

  if (isError) {
    printPadding(len);
    printf(COLOR_RED "[ERROR] %s" COLOR_RESET "\n", message);
  } else {
    printPadding(len);
    printf(COLOR_GREEN "[SUCCESS] %s" COLOR_RESET "\n", message);
  }
}

// Helper to read a single keypress without waiting for Enter
int getch() {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

int showMenu(const char *title, const char *options[], int count) {
  int selected = 0;
  int ch;

  while (1) {
    showHeader(title);

    printf("\n");
    for (int i = 0; i < count; i++) {
      // Construct the display label
      char label[100];
      if (i == selected) {
        snprintf(label, sizeof(label), "> %s <", options[i]);
      } else {
        strcpy(label, options[i]);
      }

      int menuWidth = 40; // Fixed width for menu bars
      int len = strlen(label);
      int padL = (menuWidth - len) / 2;
      int padR = menuWidth - len - padL;
      if (padL < 0) {
        padL = 0;
        padR = 0;
      }

      // Calculate screen centering for the 40-char block
      int screenPad = (80 - menuWidth) / 2;
      if (screenPad < 0)
        screenPad = 0;

      // Print screen left padding (uncolored)
      for (int k = 0; k < screenPad; k++)
        putchar(' ');

      // Start coloring
      if (i == selected)
        printf(COLOR_BOLD COLOR_WHITE BG_BLUE);

      // Print block left padding
      for (int k = 0; k < padL; k++)
        putchar(' ');

      // Print label
      printf("%s", label);

      // Print block right padding
      for (int k = 0; k < padR; k++)
        putchar(' ');

      // Reset color and newline
      printf(COLOR_RESET "\n");
    }
    printf("\n" COLOR_CYAN);
    printCentered("Use UP/DOWN arrows to navigate, ENTER to select.");
    printf(COLOR_RESET "\n");

    ch = getch();

    if (ch == 27) { // Escape sequence
      getch();      // Skip [
      switch (getch()) {
      case KEY_UP:
        selected--;
        if (selected < 0)
          selected = count - 1;
        break;
      case KEY_DOWN:
        selected++;
        if (selected >= count)
          selected = 0;
        break;
      }
    } else if (ch == '\n' || ch == '\r') {
      return selected + 1;
    }
  }
}

void getInput(const char *prompt, char *buffer, int size) {
  printf(COLOR_BOLD);
  printPadding(strlen(prompt) + 1); // +1 to account for cursor space approx
  printf("%s " COLOR_RESET, prompt);

  // Show cursor for input
  printf("\033[?25h");

  // Ensure we are in canonical mode for input (fgets)
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= (ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  if (fgets(buffer, size, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
  }

  // Hide cursor again
  printf("\033[?25l");
}

void getPasswordInput(const char *prompt, char *buffer, int size) {
  printf(COLOR_BOLD);
  printPadding(strlen(prompt) + 1);
  printf("%s " COLOR_RESET, prompt);
  printf("\033[?25h"); // Show cursor

  int pos = 0;
  while (1) {
    int ch = getch();
    if (ch == '\n' || ch == '\r') {
      buffer[pos] = '\0';
      break;
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
      if (pos > 0) {
        pos--;
        printf("\b \b");
      }
    } else if (pos < size - 1 && isprint(ch)) {
      buffer[pos++] = ch;
      printf("*");
    }
  }

  printf("\n");
  printf("\033[?25l"); // Hide cursor
}

void waitForKeyPress() {
  printf(COLOR_CYAN);
  printCentered("Press any key to continue...");
  printf(COLOR_RESET);
  getch();
}
