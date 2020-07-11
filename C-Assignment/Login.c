#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#include "Login.h"

// Enter and then validate user login information.
void login() {
	char usrBuffer[256], c;
	int i;
	bool isValidLogin;

	do {
		char pwdBuffer[256] = { 0 };
		i = 0;

		printf("$ Enter username: ");
		scanf("%s", usrBuffer);
		printf("$ Enter password: ");

		// Replace each character of the password with a '*'
		// Source: Ashraf Bashir - https://stackoverflow.com/a/17130989
		do {
			c = getch();

			if (isprint(c)) {
				pwdBuffer[i++] = c;
				printf("%c", '*');
			}
			else if (c == '\b' && i) {
				pwdBuffer[i--] = '\0';
				printf("%s", "\b \b");
			}
		} while (c != '\r');

		isValidLogin = validateLogin(usrBuffer, pwdBuffer);

		if (isValidLogin)
			printf("\nLogin successful.\n");
		else
			printf("\nTry again.\n\n");
	} while (!isValidLogin);
}

// Validate entered login information.
bool validateLogin(char username[], char password[]) {
	FILE *loginFile = fopen(LOGIN_FILE_NAME, "r"); // open the login file for reading
	int numVals;

	if (loginFile == NULL) {
		printf("\n[Error] Couldn't open the file %s.\n", LOGIN_FILE_NAME);
		exit(0);
	}
	else {
		if (strlen(password) > PASSWORD_LEN - 1 || strlen(username) > USERNAME_LEN - 1) { // username or password entries were too long.
			fclose(loginFile);
			return false;
		}
		else {
			// search the login file for the entered username & password.
			while (!feof(loginFile)) {
				numVals = fscanf(loginFile, "%s %s", userLogin.username, userLogin.password);

				if (numVals == 2) { // each line in the login file should contain two columns only
					if (strcmp(username, userLogin.username) == 0 && strcmp(password, userLogin.password) == 0) { // check if entered username & password match.
						fclose(loginFile);
						return true; // valid login
					}
				}
			}
		}
		fclose(loginFile);
		return false;
	}
}