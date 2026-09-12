#include "header.h"
#include "ui.h"

void mainMenu(struct User u) {
  const char *options[] = {
      "Create a new account", "Update account information",
      "Check accounts",       "Check list of owned account",
      "Make Transaction",     "Remove existing account",
      "Transfer ownership",   "Exit"};

  while (1) {
    int choice = showMenu("Main Menu", options, 8);

    switch (choice) {
    case 1:
      createNewAcc(u);
      break;
    case 2:
      updateAccount(u);
      break;
    case 3:
      checkAccountDetails(u);
      break;
    case 4:
      checkAllAccounts(u);
      break;
    case 5:
      makeTransaction(u);
      break;
    case 6:
      removeAccount(u);
      break;
    case 7:
      transferOwner(u);
      break;
    case 8:
      exit(0);
      break;
    }
  }
}

void initMenu(struct User *u) {
  const char *options[] = {"Login", "Register", "Exit"};

  while (1) {
    int choice = showMenu("Welcome to Apex Vault", options, 3);

    switch (choice) {
    case 1:
      loginMenu(u->name, u->password);
      if (strcmp(u->password, getPassword(*u)) == 0) {
        showStatus("Login Successful!", 0);
        u->id = getUserId(u->name);
        return; // Proceed to main menu
      } else {
        showStatus("Wrong password or Username!", 1);
        waitForKeyPress();
      }
      break;
    case 2:
      registerMenu(u->name, u->password);
      break;
    case 3:
      exit(0);
      break;
    }
  }
}

int main() {
  struct User u;

  initUI();

  initMenu(&u);
  mainMenu(u);

  return 0;
}
