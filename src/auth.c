#include "header.h"
#include <termios.h>

char *USERS = "./data/users.txt";

void registerMenu(char a[50], char pass[50]) {
  struct termios oflags, nflags;
  system("clear");
  printf(
      "\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t User Registration:");
  scanf("%s", a);

  FILE *fp;
  struct User userChecker;
  if ((fp = fopen(USERS, "r")) != NULL) {
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                  userChecker.password) != EOF) {
      if (strcmp(userChecker.name, a) == 0) {
        printf("\n\n✖ Error: Username '%s' already exists!\n", a);
        fclose(fp);
        printf("\nPress any key to continue...");
        getchar();
        getchar();
        return;
      }
    }
    fclose(fp);
  }

  tcgetattr(fileno(stdin), &oflags);
  nflags = oflags;
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    exit(1);
  }
  printf("\n\n\n\n\n\t\t\t\tEnter the password:");
  scanf("%s", pass);

  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    exit(1);
  }

  int id = 0;
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
    printf("\nError opening users file for writing.\n");
    exit(1);
  }
  fprintf(fp, "%d %s %s\n", id, a, pass);
  fclose(fp);
  printf("\n\n✔ Registration Successful! Your User ID is %d\n", id);
}

void loginMenu(char a[50], char pass[50]) {
  struct termios oflags, nflags;

  system("clear");
  printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
  scanf("%s", a);

  // disabling echo
  tcgetattr(fileno(stdin), &oflags);
  nflags = oflags;
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
  printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
  scanf("%s", pass);

  // restore terminal
  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
};

const char *getPassword(struct User u) {
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen("./data/users.txt", "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF) {
    if (strcmp(userChecker.name, u.name) == 0) {
      fclose(fp);
      char *buff = userChecker.password;
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