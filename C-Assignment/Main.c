#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "Login.h"
#include "PatientDatabase.h"
#include "Report.h"

void displayMenu();
void showOptions();
bool handleOption(char option);

struct patient* headPtr = NULL; 

void main() {
	printf("================================================================================\n");
	printf("|                             ABC Dental Practice Ltd                          |\n");
	printf("================================================================================\n");

	login();
	loadPatientInfo(&headPtr);
	displayMenu();
}

void displayMenu() {
	char option;
	bool cont = true;

	showOptions();

	do {
		printf("$ Select an option from the menu above: ");
		scanf(" %c", &option);

		option = toupper(option);

		if ((option < 'A' || option > 'I') && option != 'Q') {
			printf("[Error] Please enter one of the options provided.\n");
			continue;
		}

		cont = handleOption(option);
	} while (cont);
}

void showOptions() {
	printf("================================================================================\n");
	printf("|                                     Menu                                     |\n");
	printf("================================================================================\n");
	printf("| Options:                                                                     |\n");
	printf("|   A. Add patient.                                                            |\n");
	printf("|   B. Display all patient details to screen.                                  |\n");
	printf("|   C. Display a single patient's details.                                     |\n");
	printf("|   D. Update a patient's details.                                             |\n");
	printf("|   E. Delete patient.                                                         |\n");
	printf("|   F. Generate statistics.                                                    |\n");
	printf("|   G. Generate report.                                                        |\n");
	printf("|   H. List all the patients in order of their last appointment.               |\n");
	printf("|   I. Re-print the menu to screen.                                            |\n");
	printf("|   Q. Save & quit.                                                            |\n");
	printf("================================================================================\n");
}

bool handleOption(char option) {
	switch (option) {
	case 'A':
		addPatient(headPtr);
		break;
	case 'B':
		displayAllPatients(headPtr);
		break;
	case 'C':
		displayPatient(headPtr);
		break;
	case 'D':
		launchUpdateMenu(headPtr);
		break;
	case 'E':
		deletePatient(&headPtr);
		break;
	case 'F':
		generateStats(headPtr);
		break;
	case 'G':
		generateReport(headPtr);
		break;
	case 'H':
		listInOrderOfLastAppointment(headPtr);
		break;
	case 'I':
		showOptions();
		break;
	case 'Q':
		savePatientInfo(headPtr);
		return false;
	default:
		break;
	}

	printf("\n");
	return true;
}