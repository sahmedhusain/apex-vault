#include "header.h"
#include "ui.h"

char *USERS = "./data/users.txt";

void registerMenu(char a[50], char pass[50]) {
  showHeader("User Registration");

  while (1) {
    getAlphaInput("Enter username:", a, 50);

    FILE *fp;
    struct User userChecker;
    int exists = 0;

    if ((fp = fopen(USERS, "r")) != NULL) {
      while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                    userChecker.password) != EOF) {
        if (strcmp(userChecker.name, a) == 0) {
          showStatus("Username already exists!", 1);
          exists = 1;
          break;
        }
      }
      fclose(fp);
    }

    if (!exists)
      break;
  }

  getPasswordInput("Enter password:", pass, 50);

  int id = 0;
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen(USERS, "r")) != NULL) {
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                  userChecker.password) != EOF) {
      if (userChecker.id >= id) {
        id = userChecker.id + 1;
      }
    }
    fclose(fp);
  }

  fp = fopen(USERS, "a");
  if (fp == NULL) {
    showStatus("Error opening users file for writing.", 1);
    exit(1);
  }
  fprintf(fp, "%d %s %s\n", id, a, pass);
  fclose(fp);

  char msg[100];
  snprintf(msg, sizeof(msg), "Registration Successful! Your User ID is %d", id);
  showStatus(msg, 0);
  waitForKeyPress();
}

void loginMenu(char a[50], char pass[50]) {
  showHeader("User Login");
  getInput("Enter username:", a, 50);
  getPasswordInput("Enter password:", pass, 50);
}

const char *getPassword(struct User u) {
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen("./data/users.txt", "r")) == NULL) {
    return "no user found";
  }

  while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                userChecker.password) != EOF) {
    if (strcmp(userChecker.name, u.name) == 0) {
      fclose(fp);
      static char buff[50];
      strcpy(buff, userChecker.password);
      return buff;
    }
  }

  fclose(fp);
  return "no user found";
}

int getUserId(char name[50]) {
  FILE *fp;
  struct User userChecker;
  if ((fp = fopen(USERS, "r")) == NULL) {
    return -1;
  }
  while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                userChecker.password) != EOF) {
    if (strcmp(userChecker.name, name) == 0) {
      fclose(fp);
      return userChecker.id;
    }
  }
  fclose(fp);
  return -1;
}