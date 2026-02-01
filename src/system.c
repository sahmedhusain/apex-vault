#include "header.h"
#include "ui.h"
#include <ctype.h>

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

void success(struct User u) {
  showStatus("Operation Completed!", 0);
  waitForKeyPress();
}

// Helpers for input validations
int getValidatedInteger(const char *prompt) {
  char buffer[50];
  int value;
  while (1) {
    getInput(prompt, buffer, sizeof(buffer));
    if (strlen(buffer) == 0)
      return -1; // Cancelled
    char *endptr;
    value = strtol(buffer, &endptr, 10);
    if (endptr != buffer && *endptr == '\0') {
      return value;
    }
    showStatus("Invalid input! Please enter a valid number.", 1);
  }
}

double getValidatedFloat(const char *prompt) {
  char buffer[50];
  double value;
  while (1) {
    getInput(prompt, buffer, sizeof(buffer));
    if (strlen(buffer) == 0)
      return -1.0; // Cancelled
    char *endptr;
    value = strtod(buffer, &endptr);
    if (endptr != buffer && *endptr == '\0' && value >= 0) {
      return value;
    }
    showStatus("Invalid input! Please enter a valid positive amount.", 1);
  }
}

int getDateInput(const char *prompt, char *buffer) {
  int m, d, y;
  while (1) {
    getInput(prompt, buffer, 20);
    if (strlen(buffer) == 0)
      return -1; // Cancelled
    if (sscanf(buffer, "%d/%d/%d", &m, &d, &y) == 3) {
      if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 1900 && y <= 2100) {
        return 0; // Success
      }
    }
    showStatus("Invalid date! Format must be mm/dd/yyyy.", 1);
  }
}

int getAlphaInput(const char *prompt, char *buffer, int size) {
  while (1) {
    getInput(prompt, buffer, size);
    if (strlen(buffer) == 0)
      return -1; // Cancelled
    if (strlen(buffer) > 0) {
      int valid = 1;
      for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isalpha(buffer[i]) && buffer[i] != ' ') {
          valid = 0;
          break;
        }
      }
      if (valid)
        return 0;
    }
    showStatus("Invalid input! Use alphabets only.", 1);
  }
}

void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  char buf[100];
  FILE *pf = fopen(RECORDS, "a+");

  showHeader("New Record");

  if (getDateInput("Enter today's date (mm/dd/yyyy):", buf) == -1)
    return;
  sscanf(buf, "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

  if ((r.accountNbr = getValidatedInteger("Enter the account number:")) == -1)
    return;

  int maxId = -1;
  rewind(pf);
  while (getAccountFromFile(pf, userName, &cr)) {
    if (cr.id > maxId)
      maxId = cr.id;
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
      showStatus("This Account already exists for this user", 1);
      fclose(pf);
      waitForKeyPress();
      return;
    }
  }
  r.id = maxId + 1;
  r.userId = u.id;

  if (getAlphaInput("Enter the country:", r.country, 100) == -1)
    return;
  if ((r.phone = getValidatedInteger("Enter the phone number:")) == -1)
    return;
  if ((r.amount = getValidatedFloat("Enter amount to deposit (BHD):")) == -1.0)
    return;

  const char *types[] = {"saving", "current", "fixed01", "fixed02", "fixed03"};
  int typeChoice = showMenu("Choose Account Type", types, 5);
  strcpy(r.accountType, types[typeChoice - 1]);

  saveAccountToFile(pf, u, r);
  fclose(pf);
  showStatus("Account Created Successfully!", 0);
  waitForKeyPress();
}

void checkAllAccounts(struct User u) {
  char userName[100];
  struct Record r;
  FILE *pf = fopen(RECORDS, "r");

  showHeader("All Accounts");
  printf("\t\tUser: %s\n\n", u.name);

  if (pf == NULL) {
    showStatus("No records found.", 1);
    waitForKeyPress();
    return;
  }

  int found = 0;
  while (getAccountFromFile(pf, userName, &r)) {
    if (strcmp(userName, u.name) == 0) {
      found = 1;
      printf("_____________________\n");
      printf("Account number: %d\n", r.accountNbr);
      printf("Deposit Date:   %d/%d/%d\n", r.deposit.day, r.deposit.month,
             r.deposit.year);
      printf("Country:        %s\n", r.country);
      printf("Phone number:   %d\n", r.phone);
      printf("Amount:         BHD %.2f\n", r.amount);
      printf("Type:           %s\n", r.accountType);
    }
  }
  fclose(pf);

  if (!found) {
    printf("\nNo accounts found.\n");
  }

  waitForKeyPress();
}

void updateAccount(struct User u) {
  int accountId, choice;
  struct Record r;
  char userName[50];
  int found = 0;

  showHeader("Update Account");
  if ((accountId = getValidatedInteger("Enter the account number:")) == -1)
    return;

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    showStatus("Error opening files.", 1);
    if (pt)
      fclose(pt);
    if (tmp)
      fclose(tmp);
    waitForKeyPress();
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.accountNbr == accountId) {
      if (r.userId != u.id) {
        showStatus("Error: You do not own this account!", 1);
      } else {
        found = 1;
        printf("\nAccount Found!\n");
        printf("Current Country: %s\n", r.country);
        printf("Current Phone: %d\n", r.phone);

        const char *updOptions[] = {"Update Country", "Update Phone"};
        int updChoice = showMenu("Which field to update?", updOptions, 2);

        if (updChoice == 1) {
          getInput("Enter new country:", r.country, 100);
        } else if (updChoice == 2) {
          if ((r.phone = getValidatedInteger("Enter new phone:")) == -1)
            return;
        }
        showStatus("Record Updated!", 0);
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
    showStatus("Account not found or access denied.", 1);
  }
  waitForKeyPress();
}

void checkAccountDetails(struct User u) {
  int accountId;
  struct Record r;
  char userName[50];
  int found = 0;

  showHeader("Account Details");
  if ((accountId = getValidatedInteger("Enter the account number:")) == -1)
    return;

  FILE *pt = fopen(RECORDS, "r");
  if (pt == NULL) {
    showStatus("Error opening records.", 1);
    waitForKeyPress();
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.accountNbr == accountId && r.userId == u.id) {
      found = 1;
      printf("\nAccount number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone "
             "number: %d\nAmount deposited: BHD %.2f\nType Of Account: %s\n",
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

      double interest = r.amount * rate / 12;
      printf("\nInterest Rate: %.0f%% \nMonthly Interest Reward: BHD %.2f\n",
             rate * 100, interest);
    }
  }
  fclose(pt);

  if (!found)
    showStatus("Account not found or access denied.", 1);

  waitForKeyPress();
}

void makeTransaction(struct User u) {
  int accountId, choice;
  struct Record r;
  char userName[50];
  double amount;
  int found = 0;

  showHeader("Make Transaction");
  if ((accountId = getValidatedInteger("Enter the account number:")) == -1)
    return;

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    showStatus("Error opening files.", 1);
    if (pt)
      fclose(pt);
    if (tmp)
      fclose(tmp);
    waitForKeyPress();
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.accountNbr == accountId && r.userId == u.id) {
      found = 1;
      if (strstr(r.accountType, "fixed") != NULL) {
        showStatus("Cannot perform transactions on fixed accounts!", 1);
      } else {
        printf("\nAccount Found! Current Balance: BHD %.2f\n", r.amount);

        const char *transOptions[] = {"Deposit", "Withdraw"};
        int transChoice = showMenu("Transaction Type", transOptions, 2);

        if (transChoice == 1) {
          double val = getValidatedFloat("Enter amount to deposit:");
          if (val == -1.0)
            return;
          amount = val;
          r.amount += amount;
          showStatus("Deposit successful!", 0);
        } else if (transChoice == 2) {
          double val = getValidatedFloat("Enter amount to withdraw:");
          if (val == -1.0)
            return;
          amount = val;
          if (amount > r.amount) {
            showStatus("Insufficient balance!", 1);
          } else {
            r.amount -= amount;
            showStatus("Withdrawal successful!", 0);
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
    showStatus("Account not found or access denied.", 1);

  waitForKeyPress();
}

void removeAccount(struct User u) {
  int accountId;
  struct Record r;
  char userName[50];
  int found = 0;

  showHeader("Remove Account");
  if ((accountId =
           getValidatedInteger("Enter the account number to remove:")) == -1)
    return;

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    showStatus("Error opening files.", 1);
    if (pt)
      fclose(pt);
    if (tmp)
      fclose(tmp);
    waitForKeyPress();
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.accountNbr == accountId) {
      if (r.userId == u.id) {
        found = 1;
        printf("\nAccount number %d removed.\n", accountId);
        continue; // Skip writing
      } else {
        showStatus("Access Denied! You do not own this account.", 1);
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

  if (found) {
    showStatus("Account removed successfully", 0);
  } else {
    showStatus("Account not found or not owned by you.", 1);
  }
  waitForKeyPress();
}

void transferOwner(struct User u) {
  int accountId, newOwnerId;
  struct Record r;
  char userName[50];
  int found = 0;

  showHeader("Transfer Ownership");
  if ((accountId =
           getValidatedInteger("Enter the account number to transfer:")) == -1)
    return;

  FILE *pt = fopen(RECORDS, "r");
  FILE *tmp = fopen("./data/temp.txt", "w");

  if (pt == NULL || tmp == NULL) {
    showStatus("Error opening files.", 1);
    if (pt)
      fclose(pt);
    if (tmp)
      fclose(tmp);
    waitForKeyPress();
    return;
  }

  while (getAccountFromFile(pt, userName, &r)) {
    if (r.accountNbr == accountId) {
      if (r.userId == u.id) {
        found = 1;
        if ((newOwnerId = getValidatedInteger("Enter new User ID:")) == -1)
          return;

        // Check new owner
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
              printf("\nTransferred to %s (ID: %d)\n", checker.name,
                     newOwnerId);
              break;
            }
          }
          fclose(uf);
        }

        if (!userFound) {
          showStatus("Target User ID not found!", 1);
        } else {
          showStatus("Ownership transferred successfully!", 0);
        }
      } else {
        showStatus("Access Denied! You do not own this account.", 1);
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
    showStatus("Account not found or not owned by you.", 1);

  waitForKeyPress();
}
