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

int main(void) {
	Robot robot;
	robot.startInKeyboardControl();

	return EXIT_SUCCESS;
}
