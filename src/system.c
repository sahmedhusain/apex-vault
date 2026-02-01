#include "header.h"
#include "ui.h"

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

// Helpers for input parsing
int getInt(const char *prompt) {
  char buf[100];
  getInput(prompt, buf, 100);
  return atoi(buf);
}

double getDouble(const char *prompt) {
  char buf[100];
  getInput(prompt, buf, 100);
  return atof(buf);
}

void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  char buf[100];
  FILE *pf = fopen(RECORDS, "a+");

  showHeader("New Record");

  getInput("Enter today's date (mm/dd/yyyy):", buf, 100);
  sscanf(buf, "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

  r.accountNbr = getInt("Enter the account number:");

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

  getInput("Enter the country:", r.country, 100);
  r.phone = getInt("Enter the phone number:");
  r.amount = getDouble("Enter amount to deposit: $");

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
      printf("Amount:         $%.2f\n", r.amount);
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
  accountId = getInt("Enter the account ID to update:");

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
    if (r.id == accountId) {
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
          r.phone = getInt("Enter new phone:");
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
  accountId = getInt("Enter the account ID:");

  FILE *pt = fopen(RECORDS, "r");
  if (pt == NULL) {
    showStatus("Error opening records.", 1);
    waitForKeyPress();
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

      double interest = r.amount * rate / 12;
      printf("\nInterest Rate: %.0f%% \nMonthly Interest Reward: $%.2f\n",
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
  accountId = getInt("Enter the account ID:");

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
    if (r.id == accountId && r.userId == u.id) {
      found = 1;
      if (strstr(r.accountType, "fixed") != NULL) {
        showStatus("Cannot perform transactions on fixed accounts!", 1);
      } else {
        printf("\nAccount Found! Current Balance: $%.2f\n", r.amount);

        const char *transOptions[] = {"Deposit", "Withdraw"};
        int transChoice = showMenu("Transaction Type", transOptions, 2);

        if (transChoice == 1) {
          amount = getDouble("Enter amount to deposit:");
          r.amount += amount;
          showStatus("Deposit successful!", 0);
        } else if (transChoice == 2) {
          amount = getDouble("Enter amount to withdraw:");
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
  accountId = getInt("Enter the account ID to remove:");

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
    if (r.id == accountId) {
      if (r.userId == u.id) {
        found = 1;
        printf("\nAccount ID %d removed.\n", accountId);
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
  accountId = getInt("Enter the account ID to transfer:");

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
    if (r.id == accountId) {
      if (r.userId == u.id) {
        found = 1;
        newOwnerId = getInt("Enter new User ID:");

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
