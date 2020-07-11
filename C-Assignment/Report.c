#include <stdio.h>

#include "Report.h"
#include "PatientInfo.h"
#include "FlushInput.h"

static int criteria;
static char category;

void generateStats(struct patient* top) {
	bool validInput = false;

	do {
		do {
			printf("\nWhich criteria do you want to base the statistics on:\n");
			printf(" 1. How many cigarettes patients smoke per day.\n");
			printf(" 2. How often patients exercise per day.\n");
			printf("$ ");
			if (scanf("%d", &criteria) == 1)
				validInput = true;
			else {
				printf("Invalid input, try again.\n");
				flushInput();
			}
		} while (!validInput);
	} while (criteria != 1 && criteria != 2);

	// Generate the statistics for the entered criteria
	if (criteria == 1) {
		printf("\nHow many cigarettes do they smoke per day: \n");
		printf(" A. Less than 10.\n");
		printf(" B. More than 10.\n");
		printf("$ ");

		do {
			scanf(" %c", &category);
			if (toupper(category) != 'A' && toupper(category) != 'B')
				printf("Invalid input, try again.\n$ ");
		} while (toupper(category) != 'A' && toupper(category) != 'B');

		smokersBMIStats(top, toupper(category));
	}
	else if (criteria == 2) {
		printf("How often do they exercise:\n");
		printf(" A. Less than twice per week.\n");
		printf(" B. More than twice per week.\n");
		printf(" C. Never.\n");
		printf("$ ");

		do {
			scanf(" %c", &category);
			if (toupper(category) != 'A' && toupper(category) != 'B' && toupper(category) != 'C')
				printf("Invalid input, try again.\n$ ");
		} while (toupper(category) != 'A' && toupper(category) != 'B' && toupper(category) != 'C');

		exerciseBMIStats(top, toupper(category));
	}

	printf("\nStatistics generated successfully. Enter 'G' to generate the report file.\n");
}

void smokersBMIStats(struct patient* top, char amtCigs) {
	struct patient* temp = top;
	int numUnderweight = 0, numNormalweight = 0, numOverweight = 0, numObese = 0, numPatients = 0;
	float bmi;

	// find the BMI of each patient in this category and count how many are of each weight 
	while (temp != NULL) {
		if (temp->amtCigarettes == amtCigs) {
			bmi = calcBMI(temp->height, temp->weight);

			if (bmi < UNDERWEIGHT)
				numUnderweight++;
			else if (bmi < NORMALWEIGHT)
				numNormalweight++;
			else if (bmi < OVERWEIGHT)
				numOverweight++;
			else if (bmi >= OVERWEIGHT)
				numObese++;

			numPatients++;
		}

		temp = temp->NEXT;
	}

	// calculate percentages
	smokerStats.underweight = calcPercent(numUnderweight, numPatients);
	smokerStats.normalweight = calcPercent(numNormalweight, numPatients);
	smokerStats.overweight = calcPercent(numOverweight, numPatients);
	smokerStats.obese = calcPercent(numObese, numPatients);
}

void exerciseBMIStats(struct patient* top, char amtEx) {
	struct patient* temp = top;
	int numUnderweight = 0, numNormalweight = 0, numOverweight = 0, numObese = 0, numPatients = 0;
	float bmi;

	// find the BMI of each patient in this category and count how many are of each weight 
	while (temp != NULL) {
		if (temp->amtExercise == amtEx) {
			bmi = calcBMI(temp->height, temp->weight);

			if (bmi < UNDERWEIGHT)
				numUnderweight++;
			else if (bmi < NORMALWEIGHT)
				numNormalweight++;
			else if (bmi < OVERWEIGHT)
				numOverweight++;
			else if (bmi > OVERWEIGHT)
				numObese++;

			numPatients++;
		}

		temp = temp->NEXT;
	}

	// calculate percentages
	exerciseStats.underweight = calcPercent(numUnderweight, numPatients);
	exerciseStats.normalweight = calcPercent(numNormalweight, numPatients);
	exerciseStats.overweight = calcPercent(numOverweight, numPatients);
	exerciseStats.obese = calcPercent(numObese, numPatients);
}

float calcBMI(float height, float weight) {
	// BMI = kg/m^2
	height = height / 100; // convert height from cm to m
	return weight / (height * height);
}

float calcPercent(int numWeight, int numPatients) {
	if (numPatients == 0) // avoid dividing by zero
		return 0;
	else 
		return ((float)numWeight / numPatients) * 100;
}

void generateReport(struct patient* top) {
	FILE *reportFile = fopen(REPORT_FILE_NAME, "w");
	struct patient* temp = top;
	char fullName[FULL_NAME_LEN];

	if (reportFile == NULL) {
		printf("[Error] Couldn't save to the file '%s'.\n", REPORT_FILE_NAME);
		return;
	}
	else {
		fprintf(reportFile, "==============================================================================================================================================================================================================\n");
		fprintf(reportFile, "|                                                                                            ABC Dental Practice Ltd                                                                                         |\n");
		fprintf(reportFile, "==============================================================================================================================================================================================================\n");
		fprintf(reportFile, "|                                                                                                    Report                                                                                                  |\n");
		fprintf(reportFile, "==============================================================================================================================================================================================================\n");
		fprintf(reportFile, "PPSN     Name                                 Birthyear  Gender  Email                          Next of Kin                              Last Appointment  Weight  Height  Allergies  Smokes  Drinks  Exercise\n");

		// output each patients information to the report file
		while (temp != NULL) {
			sprintf(fullName, "%s %s", temp->firstName, temp->lastName);

			fprintf(reportFile, "%-8d %-36s %9d %7c  %-30s %-40s %16s %7.1f %7.1f %10c",
				temp->pps, fullName, temp->birthYear, temp->gender,
				temp->email, temp->nextOfKinName, temp->lastAppointment,
				temp->weight, temp->height, temp->hasAllergies ? 'Y' : 'N');

			// prints 'N/A' instead of a '0' for cigarettes & alcohol
			temp->amtCigarettes == '0' ? fprintf(reportFile, " %7s", "N/A") : fprintf(reportFile, " %7c", temp->amtCigarettes);
			temp->amtAlcohol == '0' ? fprintf(reportFile, " %7s", "N/A") : fprintf(reportFile, " %7c", temp->amtAlcohol);

			fprintf(reportFile, " %9c\n", temp->amtExercise);

			temp = temp->NEXT;
		}

		// stats are omitted if a criteria wasn't previously selected at the menu
		if (criteria == 1) {
			fprintf(reportFile, "\n\nBMI of patients based on the number of cigarettes smoked per day:");

			if (toupper(category) == 'A')
				fprintf(reportFile, "        >10 cigarettes per day\n\n");
			else if (toupper(category) == 'B') 
				fprintf(reportFile, "        <10 cigarettes per day\n\n");

			// output the statistics to the report file
			fprintf(reportFile, "A. Patients with a BMI of less than %.1f: %52.2f%%\n",
				UNDERWEIGHT, smokerStats.underweight);
			fprintf(reportFile, "B. Patients with a BMI of less than %.1f: %52.2f%%\n",
				NORMALWEIGHT, smokerStats.normalweight);
			fprintf(reportFile, "C. Patients with a BMI of less than %.1f: %52.2f%%\n",
				OVERWEIGHT, smokerStats.overweight);
			fprintf(reportFile, "D. Patients with a BMI of more than %.1f: %52.2f%%\n",
				OVERWEIGHT, smokerStats.obese);
		}
		else if (criteria == 2) {
			fprintf(reportFile, "\n\nBMI of patients based on amount of exercise per week:");

			if (toupper(category) == 'A') 
				fprintf(reportFile, "                       >Twice per week\n\n"); 
			else if (toupper(category) == 'B')
				fprintf(reportFile, "                       <Twice per week\n\n");
			else
				fprintf(reportFile, "                                  None\n\n");


			// output the statistics to the report file
			fprintf(reportFile, "A. Patients with a BMI of less than %.1f: %48.2f%%\n",
				UNDERWEIGHT, exerciseStats.underweight);
			fprintf(reportFile, "B. Patients with a BMI of less than %.1f: %48.2f%%\n",
				NORMALWEIGHT, exerciseStats.normalweight);
			fprintf(reportFile, "C. Patients with a BMI of less than %.1f: %48.2f%%\n",
				OVERWEIGHT, exerciseStats.overweight);
			fprintf(reportFile, "D. Patients with a BMI of more than %.1f: %48.2f%%\n",
				OVERWEIGHT, exerciseStats.obese);
		}

		fprintf(reportFile, "==============================================================================================================================================================================================================\n");
		fclose(reportFile);

		printf("Report file generated successfully.\n");
	}
}