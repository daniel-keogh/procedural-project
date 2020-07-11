#pragma once

#include <stdbool.h>

#ifndef PATIENTDB_H_INCLUDED
#define PATIENTDB_H_INCLUDED

#define PATIENT_FILE_NAME "Patient.txt"
#define NUM_COLS 15
#define NAME_LEN 20
#define FULL_NAME_LEN 40	
#define EMAIL_LEN 30
#define DATE_LEN 10
#define PPS_LEN 8

struct patient
{
	int pps;
	char firstName[FULL_NAME_LEN];
	char lastName[FULL_NAME_LEN];
	int birthYear;
	char gender;
	char email[EMAIL_LEN];
	char nextOfKinName[FULL_NAME_LEN];
	char lastAppointment[DATE_LEN];
	float weight;
	float height;
	bool hasAllergies;
	char amtCigarettes;
	char amtAlcohol;
	char amtExercise;

	struct patient* NEXT;
};

void loadPatientInfo(struct patient** top);
void savePatientInfo(struct patient* top);
void displayAllPatients(struct patient* top);
void displayPatient(struct patient* top);
void listInOrderOfLastAppointment(struct patient* top);
int dateToInt(struct patient* top, int pps);
void displaySortedPatient(struct patient* top, int pps);
void printPatientInfo(struct patient* patient);
int ppsExists(struct patient* top);
struct patient* findPatientByPPSN(struct patient* top);
void deletePatient(struct patient** top);
int getPosition(struct patient* top, int pps);
void deleteElementAtStart(struct patient** top);
void deleteElementAtPos(struct patient* top, int position);
void addPatient(struct patient* top);
bool isValidPPSN(struct patient* top, int ppsn);
void launchUpdateMenu(struct patient* top);
bool updateDetails(char option, struct patient* patient);
bool isPPSRightLen(int pps);
int numDigits(int num);
#endif