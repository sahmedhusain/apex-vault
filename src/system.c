#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
  return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s", &r->id, &r->userId,
                name, &r->accountNbr, &r->deposit.month, &r->deposit.day,
                &r->deposit.year, r->country, &r->phone, &r->amount,
                r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
  fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, u.id, u.name,
          r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
  int option;
  if (notGood == 0) {
    system("clear");
    printf("\n✖ Record not found!!\n");
  invalid:
    printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
    scanf("%d", &option);
    if (option == 0)
      f(u);
    else if (option == 1)
      mainMenu(u);
    else if (option == 2)
      exit(0);
    else {
      printf("Insert a valid operation!\n");
      goto invalid;
    }
  } else {
    printf("\nEnter 1 to go to the main menu and 0 to exit:");
    scanf("%d", &option);
  }
  if (option == 1) {
    system("clear");
    mainMenu(u);
  } else {
    system("clear");
    exit(1);
  }
}

void success(struct User u) {
  int option;
  printf("\n✔ Success!\n\n");
invalid:
  printf("Enter 1 to go to the main menu and 0 to exit!\n");
  scanf("%d", &option);
  system("clear");
  if (option == 1) {
    mainMenu(u);
  } else if (option == 0) {
    exit(1);
  } else {
    printf("Insert a valid operation!\n");
    goto invalid;
  }
}

void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  FILE *pf = fopen(RECORDS, "a+");

noAccount:
  system("clear");
  printf("\t\t\t===== New record =====\n");

  printf("\nEnter today's date(mm/dd/yyyy):");
  scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
  printf("\nEnter the account number:");
  scanf("%d", &r.accountNbr);

  int maxId = -1;
  rewind(pf);
  while (getAccountFromFile(pf, userName, &cr)) {
    if (cr.id > maxId)
      maxId = cr.id;
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
      printf("✖ This Account already exists for this user\n\n");
      goto noAccount;
    }
  }
  r.id = maxId + 1;
  r.userId = u.id;

  printf("\nEnter the country:");
  scanf("%s", r.country);
  printf("\nEnter the phone number:");
  scanf("%d", &r.phone);
  printf("\nEnter amount to deposit: $");
  scanf("%lf", &r.amount);
  printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> "
         "fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 "
         "years)\n\n\tEnter your choice:");
  scanf("%s", r.accountType);

  saveAccountToFile(pf, u, r);

  fclose(pf);
  success(u);
}

void checkAllAccounts(struct User u) {
  char userName[100];
  struct Record r;

  FILE *pf = fopen(RECORDS, "r");

  system("clear");
  printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
  while (getAccountFromFile(pf, userName, &r)) {
    if (strcmp(userName, u.name) == 0) {
      printf("_____________________\n");
      printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone "
             "number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
             r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
             r.country, r.phone, r.amount, r.accountType);
    }
  }
  fclose(pf);
  success(u);
}

void updateAccount(struct User u) {
  int accountId, choice;
  struct Record r;
  char userName[50];
  int found = 0;

  system("clear");
  printf("\t\t\t===== Update Account =====\n");
  printf("Enter the account ID to update: ");
  scanf("%d", &accountId);

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    printf("Error opening files.\n");
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.id == accountId) {
      if (r.userId != u.id) {
        printf("\n✖ Error: You do not own this account (ID: %d)!\n", accountId);
      } else {
        found = 1;
        printf("\nAccount Found!\n");
        printf("Current Country: %s\n", r.country);
        printf("Current Phone: %d\n", r.phone);

        printf("\nWhich field to update?\n1. Country\n2. Phone\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) {
          printf("Enter new country: ");
          scanf("%s", r.country);
        } else if (choice == 2) {
          printf("Enter new phone: ");
          scanf("%d", &r.phone);
        }
        printf("\n✔ Record Updated successfully provided.\n");
      }
    }
    fprintf(tmp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
            userName, r.accountNbr, r.deposit.month, r.deposit.day,
            r.deposit.year, r.country, r.phone, r.amount, r.accountType);
  }
  fclose(pt);
  fclose(tmp);
  remove(RECORDS);
  rename("./data/temp.txt", RECORDS);

  if (!found) {
    printf("\n✖ Account not found or already up to date.\n");
  }
  success(u);
}

void checkAccountDetails(struct User u) {
  int accountId;
  struct Record r;
  char userName[50];
  int found = 0;

  system("clear");
  printf("\t\t\t===== Account Details =====\n");
  printf("Enter the account ID: ");
  scanf("%d", &accountId);

  FILE *pt = fopen(RECORDS, "r");
  if (pt == NULL) {
    printf("Error opening file.\n");
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.id == accountId && r.userId == u.id) {
      found = 1;
      printf("\nAccount number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone "
             "number: %d\nAmount deposited: $%.2f\nType Of Account: %s\n",
             r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
             r.country, r.phone, r.amount, r.accountType);

      double rate = 0.0;
      if (strcmp(r.accountType, "saving") == 0)
        rate = 0.07;
      else if (strcmp(r.accountType, "fixed01") == 0)
        rate = 0.04;
      else if (strcmp(r.accountType, "fixed02") == 0)
        rate = 0.05;
      else if (strcmp(r.accountType, "fixed03") == 0)
        rate = 0.08;
      else if (strcmp(r.accountType, "current") == 0)
        rate = 0.0;

      double interest = r.amount * rate / 12; // Monthly interest
      printf("\nInterest Rate: %.0f%% \nMonthly Interest Reward: $%.2f\n",
             rate * 100, interest);
    }
  }
  fclose(pt);

  if (!found)
    printf("\n✖ Account not found or access denied.\n");
  success(u);
}

void makeTransaction(struct User u) {
  int accountId, choice;
  struct Record r;
  char userName[50];
  double amount;
  int found = 0;

  system("clear");
  printf("\t\t\t===== Make Transaction =====\n");
  printf("Enter the account ID: ");
  scanf("%d", &accountId);

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    printf("Error opening files.\n");
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.id == accountId && r.userId == u.id) {
      found = 1;
      if (strstr(r.accountType, "fixed") != NULL) {
        printf("\n✖ Cannot perform transactions on fixed accounts!\n");
      } else {
        printf("\nAccount Found! Current Balance: $%.2f\n", r.amount);
        printf("1. Deposit\n2. Withdraw\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
          printf("Enter amount to deposit: ");
          scanf("%lf", &amount);
          r.amount += amount;
          printf("\n✔ Deposit successful!\n");
        } else if (choice == 2) {
          printf("Enter amount to withdraw: ");
          scanf("%lf", &amount);
          if (amount > r.amount) {
            printf("\n✖ Insufficient balance!\n");
          } else {
            r.amount -= amount;
            printf("\n✔ Withdrawal successful!\n");
          }
        }
      }
    }
    fprintf(tmp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
            userName, r.accountNbr, r.deposit.month, r.deposit.day,
            r.deposit.year, r.country, r.phone, r.amount, r.accountType);
  }

  fclose(pt);
  fclose(tmp);
  remove(RECORDS);
  rename("./data/temp.txt", RECORDS);

  if (!found)
    printf("\n✖ Account not found or access denied.\n");
  success(u);
}

void removeAccount(struct User u) {
  int accountId;
  struct Record r;
  char userName[50];
  int found = 0;

  system("clear");
  printf("\t\t\t===== Remove Account =====\n");
  printf("Enter the account ID to remove: ");
  scanf("%d", &accountId);

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    printf("Error opening files.\n");
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.id == accountId) {
      if (r.userId == u.id) {
        found = 1;
        printf("\n✔ Account %d removed successfully.\n", accountId);
        continue; // Skip writing this record to temp file
      } else {
        printf("\n✖ Access Denied! You do not own this account.\n");
      }
    }
    fprintf(tmp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
            userName, r.accountNbr, r.deposit.month, r.deposit.day,
            r.deposit.year, r.country, r.phone, r.amount, r.accountType);
  }

  fclose(pt);
  fclose(tmp);
  remove(RECORDS);
  rename("./data/temp.txt", RECORDS);

  if (!found)
    printf("\n✖ Account not found or not owned by you.\n");
  success(u);
}

void transferOwner(struct User u) {
  int accountId, newOwnerId;
  struct Record r;
  char userName[50];
  int found = 0;

  system("clear");
  printf("\t\t\t===== Transfer Ownership =====\n");
  printf("Enter the account ID to transfer: ");
  scanf("%d", &accountId);

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    printf("Error opening files.\n");
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.id == accountId) {
      if (r.userId == u.id) {
        found = 1;
        printf("\nEnter new User ID: ");
        scanf("%d", &newOwnerId);

        // Check if new owner exists
        FILE *uf = fopen("./data/users.txt", "r");
        struct User checker;
        int userFound = 0;
        if (uf != NULL) {
          while (fscanf(uf, "%d %s %s", &checker.id, checker.name,
                        checker.password) != EOF) {
            if (checker.id == newOwnerId) {
              userFound = 1;
              strcpy(userName, checker.name);
              r.userId = newOwnerId;
              printf("\n✔ Ownership transferred to %s (ID: %d)\n", checker.name,
                     newOwnerId);
              break;
            }
          }
          fclose(uf);
        }

        if (!userFound) {
          printf("\n✖ Error: Target User ID not found!\n");
          // Don't update
        }
      } else {
        printf("\n✖ Access Denied! You do not own this account.\n");
      }
    }
    fprintf(tmp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
            userName, r.accountNbr, r.deposit.month, r.deposit.day,
            r.deposit.year, r.country, r.phone, r.amount, r.accountType);
  }

  fclose(pt);
  fclose(tmp);
  remove(RECORDS);
  rename("./data/temp.txt", RECORDS);

  if (!found)
    printf("\n✖ Account not found or not owned by you.\n");
  success(u);
}
