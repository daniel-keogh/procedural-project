#pragma once

#include "PatientDatabase.h"

#ifndef REPORT_H_INCLUDED
#define REPORT_H_INCLUDED

#define REPORT_FILE_NAME "Report.txt"
#define UNDERWEIGHT 18.5
#define NORMALWEIGHT 25.0
#define OVERWEIGHT 30.0

struct {
	float underweight;
	float normalweight;
	float overweight;
	float obese;
} smokerStats;

struct {
	float underweight;
	float normalweight;
	float overweight;
	float obese;
} exerciseStats;

void generateStats(struct patient* top);
void smokersBMIStats(struct patient* top, char amtCigs);
void exerciseBMIStats(struct patient* top, char amtEx);
float calcBMI(float height, float weight);
float calcPercent(int numWeight, int numPatients);
void generateReport(struct patient* top);

#endif