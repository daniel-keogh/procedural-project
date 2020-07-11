#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "PatientDatabase.h"
#include "PatientInfo.h"
#include "FlushInput.h"

// Load existing patient data from the Patients.txt file and add each entry to the linked list.
void loadPatientInfo(struct patient** top) {
	FILE *patientFile = fopen(PATIENT_FILE_NAME, "r");
	int numVals;
	char nokFirst[NAME_LEN], nokLast[NAME_LEN];

	struct patient* newPatient;

	if (patientFile == NULL) {
		printf("[Error] Couldn't open the file %s.\n", PATIENT_FILE_NAME);
		exit(0);
	}
	else {
		while (!feof(patientFile)) {

			newPatient = (struct patient*)malloc(sizeof(struct patient));

			// read in the next line of the file.
			numVals = fscanf(patientFile, "%d %s %s %d %c %s %s %s %s %f %f %d %c %c %c",
				&newPatient->pps, newPatient->firstName, newPatient->lastName,
				&newPatient->birthYear, &newPatient->gender, newPatient->email,
				nokFirst, nokLast, newPatient->lastAppointment, &newPatient->weight,
				&newPatient->height, &newPatient->hasAllergies, &newPatient->amtCigarettes,
				&newPatient->amtAlcohol, &newPatient->amtExercise);

			if (numVals == NUM_COLS) {
				sprintf(newPatient->nextOfKinName, "%s %s", nokFirst, nokLast); // join the next of kin first & last names to one string.

				// Set the next pointer of the new node to point to the current first node of the list.
				newPatient->NEXT = *top;
				// Change the head pointer to point to the new node, so it is now the first node in the list.
				*top = newPatient;
			}
		}
		fclose(patientFile);
	}
}

// Save patient information to the Patient.txt file.
void savePatientInfo(struct patient* top) {
	FILE *patientFile = fopen(PATIENT_FILE_NAME, "w");

	struct patient* temp = top;

	if (patientFile == NULL) {
		printf("[Error] Couldn't save to the file %s.\n", PATIENT_FILE_NAME);
		return;
	}
	else {
		// print each patient
		while (temp != NULL) {
			fprintf(patientFile, "%d %s %s %d %c %s %s %s %.1f %.1f %d %c %c %c\n",
				temp->pps, temp->firstName, temp->lastName,
				temp->birthYear, temp->gender, temp->email,
				temp->nextOfKinName, temp->lastAppointment, temp->weight,
				temp->height, temp->hasAllergies, temp->amtCigarettes,
				temp->amtAlcohol, temp->amtExercise);

			temp = temp->NEXT;
		}
		fclose(patientFile);
	}
}

// Print information about every patient to the screen.
void displayAllPatients(struct patient* top) {
	struct patient* temp = top;

	if (temp == NULL) {
		printf("There are no patients currently in the database.\n");
		return;
	}
	else {
		while (temp != NULL) {
			printPatientInfo(temp);
			temp = temp->NEXT;
		}
	}
}

// Print information about a single patient to the screen.
void displayPatient(struct patient* top) {
	bool found = false;
	struct patient* temp = top;
	int pps = ppsExists(top);

	if (pps == -1)
		return;
	else {
		while (temp != NULL) {
			if (temp->pps == pps) {
				printPatientInfo(temp);
				return;
			}
			temp = temp->NEXT;
		}
	}
}

void listInOrderOfLastAppointment(struct patient* top) {
	struct patient* temp = top; 
	char* sortedPPS[PPS_LEN];
	char* tempPPS[PPS_LEN];
	int numPatients = 0;

	if (temp == NULL) {
		printf("There are no patients currently in the database.\n");
		return;
	}
	else {
		while (temp != NULL) {
			// count the number of patients & put their PPS numbers into an array to be sorted (sort via pps no. because they're unique)
			sortedPPS[numPatients] = temp->pps;
			numPatients++; 
			temp = temp->NEXT;
		}

		// Sort the dates using the selection sort algorithm.
		int indexMin, i, j;
		for (i = 0; i < numPatients - 1; i++) {

			// set current element as minimum
			indexMin = i;

			// check the element to be minimum
			for (j = i + 1; j < numPatients; j++) {
				// Convert the dates to integers for comparison. 
				if (dateToInt(top, sortedPPS[j]) < dateToInt(top, sortedPPS[indexMin]))
					indexMin = j;
			}

			if (indexMin != i) {
				// swap the elements
				tempPPS[i] = sortedPPS[indexMin];
				sortedPPS[indexMin] = sortedPPS[i];
				sortedPPS[i] = tempPPS[i];
			}
		}

		// Output the patients in order of the last appointment date
		printf("PPSN     Name                                 Email                          Last Appointment\n");
		for (int i = 0; i < numPatients; i++)
			displaySortedPatient(top, sortedPPS[i]);
	}
}

// Since atoi(date) stops reading the input string at the first character that isn't part of a number ('/') it will only sort by year.
int dateToInt(struct patient* top, int pps) {
	struct patient* temp = top;
	char numToConvert[DATE_LEN];
	char date[DATE_LEN + 1];
	int j = 0;

	// get the last appointment date of the patient with the PPS number passed
	while (temp != NULL) {
		if (temp->pps == pps) {
			strcpy(date, temp->lastAppointment);
			break;
		}
		temp = temp->NEXT;
	}

	for (int i = 0; i < DATE_LEN; i++) {
		if (isdigit(date[i])) { // ignore the separators ('/')
			numToConvert[j] = date[i];
			j++;
		}
	}

	numToConvert[j] = '\0'; // make the char array a string
	return atoi(numToConvert);
}

void displaySortedPatient(struct patient* top, int pps) {
	struct patient* temp = top;
	char fullname[FULL_NAME_LEN];
	
	while (temp != NULL) {
		// display this patient if the pps numbers match
		if (pps == temp->pps) {
			sprintf(fullname, "%s %s", temp->firstName, temp->lastName);
			printf("%-8d %-36s %-30s %16s\n", temp->pps, fullname, temp->email, temp->lastAppointment);
			return;
		}
		temp = temp->NEXT;
	}
}

void printPatientInfo(struct patient* patient) {
	printf("%d %s %s %d %c %s %s %s %.1f %.1f %d %c %c %c\n",
		patient->pps, patient->firstName, patient->lastName,
		patient->birthYear, patient->gender, patient->email,
		patient->nextOfKinName, patient->lastAppointment, patient->weight,
		patient->height, patient->hasAllergies, patient->amtCigarettes,
		patient->amtAlcohol, patient->amtExercise);
}

// Prompts the user to enter a PPS number and then checks if it exists in the linked list.
// If it does exist, the PPSN is returned, otherwise a -1 is returned.
int ppsExists(struct patient* top) {
	int pps;
	bool validInput;
	bool found = false;
	struct patient* temp = top;

	// check that the linked list is not empty
	if (temp == NULL) {
		printf("There are no patients currently in the database.\n");
		return -1;
	}
	else {
		while (!found) {
			validInput = false;
			do {
				printf("Enter the patients PPS number (0 to exit): ");
				if (scanf("%d", &pps) == 1)
					validInput = true;
				else {
					printf("Invalid input, try again.\n\n");
					flushInput();
				}
			} while (!validInput);

			if (pps == 0)
				return -1;
			else if (!isPPSRightLen(pps))
				continue;
			else {
				// Search for a patient using the entered PPS number.
				while (temp != NULL) {
					if (temp->pps == pps)
						return pps;
					temp = temp->NEXT;
				}

				printf("Patient not found. Try again.\n\n");
				temp = top;
			}
		}
	}
}

// Search for a patient using their PPSN and then return the node containing that patient
struct patient* findPatientByPPSN(struct patient* top) {
	struct patient* temp = top;
	int pps = ppsExists(top);

	if (pps == -1)
		return NULL;
	else {
		// pps exists, so search the list and return the patient that has that number
		while (temp != NULL) {
			if (temp->pps == pps) {
				return temp;
			}
			temp = temp->NEXT;
		}
	}
}

void deletePatient(struct patient** top) {
	int pps = ppsExists(*top); // get the pps number of the patient to be deleted

	if (pps == -1)
		return;
	else {
		int position = getPosition(*top, pps); // get the patients position in the list

		// delete the patient from the list
		if (position == 0)
			deleteElementAtStart(top);
		else
			deleteElementAtPos(*top, position);

		printf("Patient deleted successfully.\n");
	}
}

// returns the position of a patient in the list
int getPosition(struct patient* top, int pps) {
	struct patient* temp = top;
	int index = 0;

	if (temp->pps == pps)
		return index; // pps matches that of the first patient in the list
	else {
		while (temp != NULL) {
			if (temp->pps == pps) {
				return index;
			}
			index++;
			temp = temp->NEXT;
		}
	}
}

// deletes the first node in the linked list
void deleteElementAtStart(struct patient** top) {
	struct patient* temp = *top;
	*top = temp->NEXT;
	free(temp);
}

// deletes a node from the linked list at a given position
void deleteElementAtPos(struct patient* top, int position) {
	int i;
	struct patient* temp;
	struct patient* prev_temp;

	temp = top;

	for (i = 0; i < position; i++) {
		prev_temp = temp;
		temp = temp->NEXT;
	}

	prev_temp->NEXT = temp->NEXT;
	free(temp);
}

void addPatient(struct patient* top) {
	struct patient* temp = top;
	struct patient* newpatient;

	while (temp->NEXT != NULL) {
		temp = temp->NEXT;
	}

	// allocate memory for the new patient node
	newpatient = (struct patient*)malloc(sizeof(struct patient));

	// Set the patients PPS number
	bool validPPSN = false;
	do {
		printf("Enter the patients PPS number: ");
		if (scanf("%d", &newpatient->pps) == 1)
		{
			if (!isValidPPSN(top, newpatient->pps))
				return;
			validPPSN = true;
		}
		else {
			printf("Invalid input, try again.\n");
			flushInput();
		}
	} while (!validPPSN);

	// set the patients details
	setName(newpatient->firstName, newpatient->lastName);
	setBirthYear(&newpatient->birthYear);
	setGender(&newpatient->gender);
	setEmailAddress(newpatient->email);
	setNextOfKin(newpatient->nextOfKinName);
	setLastAppointmentDate(newpatient->lastAppointment);
	setHeight(&newpatient->height);
	setWeight(&newpatient->weight);
	setAllergies(&newpatient->hasAllergies);
	setSmoking(&newpatient->amtCigarettes);
	setDrinking(&newpatient->amtAlcohol);
	setExercise(&newpatient->amtExercise);

	newpatient->NEXT = NULL;
	temp->NEXT = newpatient;
}

// Returns true if a given PPSN is valid by checking it is the right lenght and doesn't already belong to an existing patient in the list.
bool isValidPPSN(struct patient* top, int ppsn) {
	struct patient* temp = top;

	if (temp == NULL) {
		// the list is empty so no need to check if the PPS already exists
		if (isPPSRightLen(ppsn))
			return true;
		else
			return false;
	}
	else {
		while (temp != NULL) {
			if (temp->pps == ppsn) {
				printf("A patient with that PPSN already exists.\n\n");
				return false;
			}
			else if (isPPSRightLen(ppsn))
				return true;
			else
				return false;
			temp = temp->NEXT;
		}
	}
}

// Shows a separate menu for updating a patient's information
void launchUpdateMenu(struct patient* top) {
	char opt;
	bool cont = true;
	struct patient* patient = findPatientByPPSN(top);

	if (patient == NULL)
		return;
	else {
		printf("\n A. Name.\n");
		printf(" B. Birth Year.\n");
		printf(" C. Gender.\n");
		printf(" D. Email.\n");
		printf(" E. Next of Kin.\n");
		printf(" F. Last appointment date.\n");
		printf(" G. Weight.\n");
		printf(" H. Height.\n");
		printf(" I. Allergies.\n");
		printf(" J. Smoking.\n");
		printf(" K. Drinking.\n");
		printf(" L. Exercise.\n");
		printf(" Q. Quit.\n\n");

		do {
			printf("Select one of the above options: ");
			scanf(" %c", &opt);

			if ((toupper(opt) < 'A' || toupper(opt) > 'L') && toupper(opt) != 'Q') {
				printf("[Error] Please enter one of the options provided.\n");
				continue;
			}

			cont = updateDetails(toupper(opt), patient);
		} while (cont);
	}
}

// calls functions to update patient info depending on the option selected at the update menu.
bool updateDetails(char option, struct patient* patient) {
	switch (option) {
	case 'A':
		setName(patient->firstName, patient->lastName);
		break;
	case 'B':
		setBirthYear(&patient->birthYear);
		break;
	case 'C':
		setGender(&patient->gender);
		break;
	case 'D':
		setEmailAddress(patient->email);
		break;
	case 'E':
		setNextOfKin(patient->nextOfKinName);
		break;
	case 'F':
		setLastAppointmentDate(patient->lastAppointment);
		break;
	case 'G':
		setWeight(&patient->weight);
		break;
	case 'H':
		setHeight(&patient->height);
		break;
	case 'I':
		setAllergies(&patient->hasAllergies);
		break;
	case 'J':
		setSmoking(&patient->amtCigarettes);
		break;
	case 'K':
		setDrinking(&patient->amtAlcohol);
		break;
	case 'L':
		setExercise(&patient->amtExercise);
		break;
	case 'Q':
		return false;
	default:
		break;
	}

	return true;
}

// Validate the length of a given PPS number
bool isPPSRightLen(int pps) {
	int len = numDigits(pps);

	if (pps < 0) {
		printf("Please enter a positive integer.\n\n", PPS_LEN);
		return false;
	}
	else if (len > PPS_LEN) {
		printf("That PPSN is too long (must be %d numbers long).\n\n", PPS_LEN);
		return false;
	}
	else if (len < PPS_LEN)  {
		printf("That PPSN is not long enough (must be %d numbers long).\n\n", PPS_LEN);
		return false;
	}
	else
		return true;
}

// Returns the number of digits in a number. 
int numDigits(int num) {
	int count = 0;
	while (num != 0) {
		num /= 10;
		count++;
	}
	return count;
}