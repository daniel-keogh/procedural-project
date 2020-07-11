#pragma once

#include <stdbool.h>

#ifndef PATIENTINFO_H_INCLUDED
#define PATIENTINFO_H_INCLUDED

void setName(char firstname[], char lastname[]);
void setEmailAddress(char email[]);
bool isValidEmail(char email[]);
void setNextOfKin(char nextOfKinName[]);
void setGender(char* gender);
void setHeight(float* height);
void setWeight(float* weight);
void setAllergies(bool* hasAllergies);
void setSmoking(char* amtCigarettes);
void setDrinking(char* amtAlcohol);
void setExercise(char* amtExercise);
void setBirthYear(int* birthYear);
void setLastAppointmentDate(char lastAppointment[]);
int getCurrentYear();
#endif