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

  struct termios raw = orig_termios;
  // Disable canonical mode and echo for menu navigation
  // We will enable them temporarily for string input
  // raw.c_lflag &= ~(ECHO | ICANON);

  // For now, we only disable canonical mode when reading characters directly
  // But for the global init, we might want to just ensure we can restore state.

  ui_initialized = 1;

  // Hide cursor initially
  printf("\033[?25l");
  clearScreen();
}

void closeUI() { resetTerminal(); }

void clearScreen() { printf("\033[H\033[J"); }

void printCentered(const char *text) {
  // Assuming 80 column width for simplicity, or 80 is standard TUI width
  int width = 80;
  int len = strlen(text);
  int padding = (width - len) / 2;
  if (padding < 0)
    padding = 0;

  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s\n", text);
}

void showHeader(const char *title) {
  clearScreen();
  printf(COLOR_CYAN);
  printCentered(
      "================================================================");
  char buf[100];
  snprintf(buf, sizeof(buf), "ATM MANAGEMENT SYSTEM");
  printCentered(buf);
  printCentered(
      "================================================================");
  printf(COLOR_RESET);
  printf("\n");

  if (title) {
    printf(COLOR_BOLD COLOR_YELLOW);
    printCentered(title);
    printf(COLOR_RESET);
    printf("\n");
  }
}

void showStatus(const char *message, int isError) {
  if (isError) {
    printf(COLOR_RED "\n[ERROR] %s" COLOR_RESET "\n", message);
  } else {
    printf(COLOR_GREEN "\n[SUCCESS] %s" COLOR_RESET "\n", message);
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
      if (i == selected) {
        printf(COLOR_BOLD COLOR_WHITE BG_BLUE "  > %-40s  " COLOR_RESET "\n",
               options[i]);
      } else {
        printf("    %-40s    \n", options[i]);
      }
    }
    printf("\n" COLOR_CYAN
           "Use UP/DOWN arrows to navigate, ENTER to select." COLOR_RESET "\n");

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
      return selected + 1; // Return 1-based index to match existing logic logic
                           // usually (case 1, case 2)
    }
  }
}

void getInput(const char *prompt, char *buffer, int size) {
  printf(COLOR_BOLD "\n%s " COLOR_RESET, prompt);

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
  printf(COLOR_BOLD "\n%s " COLOR_RESET, prompt);
  printf("\033[?25h"); // Show cursor

  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ECHO;  // Turn off echo
  t.c_lflag |= ICANON; // Ensure line buffering is on for enter key
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  if (fgets(buffer, size, stdin) != NULL) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
  }

  // Restore echo
  t.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  printf("\n");        // Move to next line since echo was off
  printf("\033[?25l"); // Hide cursor
}

void waitForKeyPress() {
  printf(COLOR_CYAN "\nPress any key to continue..." COLOR_RESET);
  getch();
}
