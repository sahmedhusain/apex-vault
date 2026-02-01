#ifndef UI_H
#define UI_H

#include <stdio.h>

// Initialize the UI system (raw mode, etc.)
void initUI();

// Restore terminal settings
void closeUI();

// Clear the screen
void clearScreen();

// Display a centered header
void showHeader(const char *title);

// Display a status message (success or error)
// isError: 1 for error (red), 0 for success (green)
void showStatus(const char *message, int isError);

// Show a menu with arrow key navigation
// Returns the index of the selected option
// defaultChoice: the index to highlight initially (usually 0)
int showMenu(const char *title, const char *options[], int count);

// Robust string input handler
// prompt: Text to display
// buffer: Buffer to store input
// size: Size of buffer
void getInput(const char *prompt, char *buffer, int size);

// Masked input for passwords
void getPasswordInput(const char *prompt, char *buffer, int size);

// Pause execution and wait for user keypress
void waitForKeyPress();

#endif
