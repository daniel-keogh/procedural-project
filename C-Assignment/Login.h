#pragma once

#ifndef LOGIN_H_INCLUDED
#define LOGIN_H_INCLUDED

#define LOGIN_FILE_NAME "Login.txt"
#define USERNAME_LEN 20
#define PASSWORD_LEN 7

struct {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
} userLogin;

void login();
bool validateLogin(char username[], char password[]);

#endif