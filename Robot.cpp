#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <thread>

#include "Robot.hpp"

using namespace std;

Robot::Robot() {
	cout << "Starting robot..." << endl;
	if (wiringPiSetup() == -1) {
		cerr << "'Failed to setup wiringPi" << endl;
		exit (EXIT_FAILURE);
	}

	pushMotor = new StepperMotor28BYJ48(7, 0, 2, 3, 3, StepperMotor28BYJ48::FULL_STEP);
	steeringMotor = new StepperMotor28BYJ48(1, 4, 5, 6, 3, StepperMotor28BYJ48::FULL_STEP);
}

Robot::~Robot() {
	delete(pushMotor);
	delete(steeringMotor);
}

void Robot::performDemo() {
	thread t1(&StepperMotor28BYJ48::performDemo, pushMotor);
	thread t2(&StepperMotor28BYJ48::performDemo, steeringMotor);
	t1.join();
	t2.join();
}
