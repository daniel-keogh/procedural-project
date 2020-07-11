#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "PatientInfo.h"
#include "PatientDatabase.h"
#include "FlushInput.h"

void setName(char firstname[], char lastname[]) {
	printf("Enter the patients first name: ");
	scanf("%s", firstname);
	printf("Enter the patients last name: ");
	scanf("%s", lastname);
}

void setEmailAddress(char email[]) {
	bool validEmail = false;
	
	do {
		printf("Enter the patients email address: ");
		scanf("%s", email);

		if (!isValidEmail(email)) {
			printf("Invalid email address, try again.\n\n");
		}
		else 
			validEmail = true;
	} while (!validEmail);
}

bool isValidEmail(char email[]) {
	bool containsAt = false;
	bool containsDotCom = false;

	for (int i = 0; i < strlen(email); i++) {
		if (email[i] == '@') {
			containsAt = true;
			break;
		}
	}

	email = strrchr(email, '.'); // returns the last occurrence of '.' or NULL if there is no '.'

	if (email != NULL) {
		if (strcmp(email, ".com") == 0) // check if the address ends with ".com"
			containsDotCom = true;
	}

	if (containsAt && containsDotCom)
		return true;
	else
		return false;
}

void setNextOfKin(char nextOfKinName[]) {
	char fname[NAME_LEN], lname[NAME_LEN];

	printf("Enter the first name of the patients next of kin: ");
	scanf("%s", fname);
	printf("Enter the surname of the patients next of kin: ");
	scanf("%s", lname);

	sprintf(nextOfKinName, "%s %s", fname, lname); // join the two names
}

void setGender(char* gender) {
	do {
		printf("Enter the patients gender (M, F, O): ");
		scanf(" %c", gender);

		*gender = toupper(*gender);
	} while (*gender != 'M' && *gender != 'F' && *gender != 'O');
}

void setHeight(float* height) {
	bool validHeight = false;
	do {
		printf("Enter the patients height (cm): ");
		if (scanf("%f", height) == 1) {
			if (*height > 0)
				validHeight = true;
			else 
				printf("Invalid input, try again.\n\n");
		}
		else {
			printf("Invalid input, try again.\n\n");
			flushInput();
		}
	} while (!validHeight);
}

void setWeight(float* weight) {
	bool validWeight = false;
	do {
		printf("Enter the patients weight (kg): ");
		if (scanf("%f", weight) == 1) {
			if (*weight > 0)
				validWeight = true;
			else
				printf("Invalid input, try again.\n\n");
		}
		else {
			printf("Invalid input, try again.\n\n");
			flushInput();
		}
	} while (!validWeight);
}

void setAllergies(bool* hasAllergies) {
	char ans;

	do {
		printf("Does the patient have any allergies (y/N): ");
		scanf(" %c", &ans);
	} while (toupper(ans) != 'Y' && toupper(ans) != 'N');

	*hasAllergies = (ans == 'Y') ? true : false;
}

void setSmoking(char* amtCigarettes) {
	char ans;

	do {
		printf("Does the patient smoke (y/N): ");
		scanf(" %c", &ans);
	} while (toupper(ans) != 'Y' && toupper(ans) != 'N');

	if (toupper(ans) == 'Y') {
		printf("\nHow many cigarettes do they smoke per day: \n");
		printf(" A. Less than 10.\n");
		printf(" B. More than 10.\n");
		printf("$ ");

		do {
			scanf(" %c", &ans);
			if (toupper(ans) != 'A' && toupper(ans) != 'B')
				printf("Invalid input, try again.\n$ ");				
		} while (toupper(ans) != 'A' && toupper(ans) != 'B');

		*amtCigarettes = toupper(ans);
	}
	else
		*amtCigarettes = '0';
}

void setDrinking(char* amtAlcohol) {
	char ans;

	do {
		printf("Does the patient drink alcohol (y/N): ");
		scanf(" %c", &ans);
	} while (toupper(ans) != 'Y' && toupper(ans) != 'N');

	if (toupper(ans) == 'Y')
	{
		printf("How many units of alcohol do they drink per week: \n");
		printf(" A. Less than 10.\n");
		printf(" B. More than 10.\n");

		do {
			scanf(" %c", &ans);
			if (toupper(ans) != 'A' && toupper(ans) != 'B')
				printf("Invalid input, try again.\n");
		} while (toupper(ans) != 'A' && toupper(ans) != 'B');

		*amtAlcohol = toupper(ans);
	}
	else
		*amtAlcohol = '0';
}

void setExercise(char* amtExercise) {
	char ans;

	printf("How often do they exercise:\n");
	printf(" A. Less than twice per week.\n");
	printf(" B. More than twice per week.\n");
	printf(" C. Never.\n");
	printf("$ ");

	do {
		scanf(" %c", &ans);
		if (toupper(ans) != 'A' && toupper(ans) != 'B' && toupper(ans) != 'C')
			printf("Invalid input, try again.\n$ ");
	} while (toupper(ans) != 'A' && toupper(ans) != 'B' && toupper(ans) != 'C');

	*amtExercise = toupper(ans);
}

void setBirthYear(int* birthYear) {
	bool validYr = false;
	do {
		printf("Enter the patients year of birth: ");
		if (scanf("%d", birthYear) == 1) {
			if (*birthYear > getCurrentYear() || *birthYear < 1900) {
				printf("Invalid year, try again.\n");
				continue;
			}
			validYr = true;
		}
		else {
			printf("Invalid year, try again.\n");
			flushInput();
		}
	} while (!validYr);
}

void setLastAppointmentDate(char lastAppointment[]) {
	int yyyy, mm, dd;
	int daysinmonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	bool validInput = false;
	do {
		printf("Enter the date of the patients last appointment (dd/mm/yyyy): ");
		if (scanf("%d/%d/%d", &dd, &mm, &yyyy) == 3) {

			// validate the date
			// reference: Riskhan - https://stackoverflow.com/a/14950237
			if (yyyy % 400 == 0 || (yyyy % 100 != 0 && yyyy % 4 == 0)) 	// if a leap year, add an extra day to february
				++daysinmonth[1];

			if (mm <= 12 && dd <= daysinmonth[mm - 1] && dd > 0 && yyyy > 1900 && yyyy <= getCurrentYear())
				validInput = true;
			else 
				printf("Invalid date, try again.\n\n");
		}
		else {
			printf("Invalid input, try again.\n\n");
			flushInput();
		}
	} while (!validInput);

	sprintf(lastAppointment, "%04d/%02d/%02d", yyyy, mm, dd); // convert the date to a string (in the form yyyy/mm/dd so it can be sorted)
}

// Returns the current year as an int
// Source: http://forums.devshed.com/programming-42/current-222776.html
// Dave Sinkula - January 26th, 2005
int getCurrentYear() {
	time_t now;
	if (time(&now) != (time_t)(-1)) {
		struct tm *mytime = localtime(&now);
		if (mytime) {
			char year[5];
			if (strftime(year, sizeof year, "%Y", mytime))
				return atoi(year);
		}
	}
	return 0;
}