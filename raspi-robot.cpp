//============================================================================
// Name        : raspi-robot.cpp
// Author      : Joachim Lengacher
// Version     :
// Copyright   : 
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Robot.hpp"

using namespace std;

int main(int argc, const char *argv[]) {
	Robot robot;
	if (argc > 1) {
		robot.setLineTrackingThreshold(atoi(argv[1]));
	}
	if (argc > 2) {
		robot.setHeadless(true);
	}
	robot.trackLine();

	return EXIT_SUCCESS;
}
