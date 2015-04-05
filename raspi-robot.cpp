#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

#include <wiringPi.h>

using namespace std;

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <raspicam/raspicam_cv.h>

#include <StepperMotor28BYJ48.hpp>

int main(void) {
	if (wiringPiSetup() == -1) {
		cerr << "'Failed to setup wiringPi" << endl;
		exit (EXIT_FAILURE);
	}
	StepperMotor28BYJ48 motor(1, 4, 5, 6, 3, StepperMotor28BYJ48::FULL_STEP);
	motor.performDemo();
	return EXIT_SUCCESS;
}
